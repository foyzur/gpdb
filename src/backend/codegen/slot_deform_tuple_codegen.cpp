//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		code_generator.cpp
//
//	@doc:
//		Contains different code generators
//
//---------------------------------------------------------------------------
#include "codegen/slot_deform_tuple_codegen.h"
#include <cstdint>
#include <string>

#include "balerion/clang_compiler.h"
#include "balerion/code_generator.h"
#include "balerion/utility.h"
#include "balerion/instance_method_wrappers.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Casting.h"

#include "postgres.h"
#include "c.h"
#include "access/htup.h"
#include "access/tupmacs.h"
#include "catalog/pg_attribute.h"
#include "executor/tuptable.h"

using namespace code_gen;

constexpr char SlotDeformTupleCodeGen::kSlotDeformTupleNamePrefix[];

SlotDeformTupleCodeGen::SlotDeformTupleCodeGen(TupleTableSlot* slot,
			SlotDeformTupleFn regular_func_ptr, SlotDeformTupleFn* ptr_to_regular_func_ptr):
		BasicCodeGen(kSlotDeformTupleNamePrefix, regular_func_ptr, ptr_to_regular_func_ptr),
		slot_(slot) {

}

bool SlotDeformTupleCodeGen::GenerateCode(CodeGeneratorManager* manager,
			balerion::CodeGenerator* code_generator) {
	TupleDesc tupleDesc = slot_->tts_tupleDescriptor;

	int natts = tupleDesc->natts;

	static_assert(sizeof(Datum) == sizeof(int64), "Expecting 64 bit datum");

	if (tupleDesc->natts != 1 || tupleDesc->attrs[0]->attlen != sizeof(int32))
	{
		return false;
	}

	// void slot_deform_tuple_func(char* data_start_adress, void* values, void* isnull)
    llvm::Function* slot_deform_tuple_func
  	  = function_traits<SlotDeformTupleFn>::CreateFunctionHelper(code_generator, GetFuncName());

    // BasicBlocks for function entry.
    llvm::BasicBlock* entry_block = code_generator->CreateBasicBlock(
  	  "entry", slot_deform_tuple_func);

    llvm::Value* input = balerion::ArgumentByPosition(slot_deform_tuple_func, 0);
    llvm::Value* out_values = balerion::ArgumentByPosition(slot_deform_tuple_func, 1);

	auto irb =
			code_generator->ir_builder();

	irb->SetInsertPoint(entry_block);

    llvm::Value* true_const = code_generator->GetConstant(true);
    llvm::Value* datum_size_const = code_generator->GetConstant(sizeof(Datum));
    llvm::Value* bool_size_const = code_generator->GetConstant(sizeof(bool));

	int off = 0;

	Form_pg_attribute *att = tupleDesc->attrs;
	for (int attnum = 0; attnum < natts; attnum++) {
		Form_pg_attribute thisatt = att[attnum];
		off = att_align(off, thisatt->attalign);

		if (thisatt->attlen < 0) {
			// TODO: Cleanup code generator
			return false;
		}

		// Load tp + off
		// store value
		// store isnull = true
		// add value + sizeof(Datum)
		// add isnull + sizeof(bool)

		// The next address of the input array where we need to read.
		llvm::Value* next_address_load =
			irb->CreateInBoundsGEP(input,
				{code_generator->GetConstant(off)});

		llvm::Value* next_address_store =
			irb->CreateInBoundsGEP(out_values,
				{code_generator->GetConstant(attnum)});

		llvm::Value* colVal = nullptr;

		// Load the value from the calculated input address.
		switch(thisatt->attlen) {
		case sizeof(char):
			// Read 1 byte at next_address_load
			colVal = irb->CreateLoad(next_address_load);
			// store colVal into out_values[attnum]
			break;
		case sizeof(int16):
			colVal = irb->CreateLoad(code_generator->GetType<int16>(),
					irb->CreateBitCast(next_address_load, code_generator->GetType<int16*>()));
			break;
		case sizeof(int32):
			colVal = irb->CreateLoad(code_generator->GetType<int32>(),
					irb->CreateBitCast(next_address_load, code_generator->GetType<int32*>()));
			break;
		case sizeof(int64):
			colVal = irb->CreateLoad(code_generator->GetType<int64>(),
					irb->CreateBitCast(next_address_load, code_generator->GetType<int64*>()));
			break;
		default:
			//TODO Cleanup
			return false;
		}

		llvm::Value* int64ColVal = irb->CreateZExt(colVal, code_generator->GetType<int64>());
		irb->CreateStore(int64ColVal, next_address_store);

		llvm::LoadInst* load_instruction =
			code_generator->ir_builder()->CreateLoad(next_address_load, "input");

		off += thisatt->attlen;
	}

    irb->CreateRetVoid();

//	/*
//	 * Save state for next execution
//	 */
//	slot->PRIVATE_tts_nvalid = attnum;
//	slot->PRIVATE_tts_off = off;
//	slot->PRIVATE_tts_slow = slow;
    return true;
}
