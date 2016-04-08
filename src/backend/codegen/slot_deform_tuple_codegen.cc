//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		codegen_utils.cpp
//
//	@doc:
//		Contains different code generators
//
//---------------------------------------------------------------------------
#include "codegen/slot_deform_tuple_codegen.h"
#include <cstdint>
#include <string>

#include "codegen/utils/clang_compiler.h"
#include "codegen/utils/utility.h"
#include "codegen/utils/instance_method_wrappers.h"
#include "codegen/utils/codegen_utils.h"

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
#include "access/htup.h"
#include "executor/tuptable.h"

extern "C" {
#include "utils/elog.h"
}

using namespace gpcodegen;

constexpr char SlotDeformTupleCodeGen::kSlotDeformTupleNamePrefix[];

SlotDeformTupleCodeGen::SlotDeformTupleCodeGen(TupleTableSlot* slot,
		SlotDeformTupleFn regular_func_ptr,
		SlotDeformTupleFn* ptr_to_regular_func_ptr) :
				BaseCodeGen(kSlotDeformTupleNamePrefix, regular_func_ptr,
						ptr_to_regular_func_ptr), slot_(slot) {

}

static void ElogWrapper(const char* func_name) {
	elog(INFO, "Calling wrapped function: %s", func_name);
}

llvm::Function* SlotDeformTupleCodeGen::GenerateSimpleSlotDeformTuple(
		gpcodegen::CodeGenUtils* codegen_utils) {
	llvm::Function* llvm_elog_wrapper = codegen_utils->RegisterExternalFunction(
			ElogWrapper);
	assert(llvm_elog_wrapper != nullptr);

	auto regular_func_pointer = GetRegularFuncPointer();
	llvm::Function* llvm_regular_function =
			codegen_utils->RegisterExternalFunction(regular_func_pointer);
	assert(llvm_regular_function != nullptr);

	llvm::Function* llvm_function = codegen_utils->CreateFunctionTypeDef<
			decltype(regular_func_pointer)>(GenerateUniqueName(GetUniqueFuncName()));

	llvm::BasicBlock* function_body = codegen_utils->CreateBasicBlock("fn_body",
			llvm_function);

	codegen_utils->ir_builder()->SetInsertPoint(function_body);
	llvm::Value* func_name_llvm = codegen_utils->GetConstant(
			GetOrigFuncName().c_str());
	codegen_utils->ir_builder()->CreateCall(llvm_elog_wrapper,
			{ func_name_llvm });

	std::vector<llvm::Value*> forwarded_args;

	for (llvm::Argument& arg : llvm_function->args()) {
		forwarded_args.push_back(&arg);
	}

	llvm::CallInst* call = codegen_utils->ir_builder()->CreateCall(
			llvm_regular_function, forwarded_args);

	// Return the result of the call, or void if the function returns void.
	if (std::is_same<
			gpcodegen::codegen_utils_detail::FunctionTypeUnpacker<
			decltype(regular_func_pointer)>::R, void>::value) {
		codegen_utils->ir_builder()->CreateRetVoid();
	} else {
		codegen_utils->ir_builder()->CreateRet(call);
	}

	return llvm_function;
}


extern void slot_deform_tuple(TupleTableSlot *slot, int natts);

bool SlotDeformTupleCodeGen::GenerateSlotDeformTuple(gpcodegen::CodeGenUtils* codegen_utils) {
	TupleDesc tupleDesc = slot_->tts_tupleDescriptor;
	int natts = tupleDesc->natts;

	auto irb = codegen_utils->ir_builder();


//	bool		hasnulls = HeapTupleHasNulls(tuple);
//
//	if (hasnulls)
//	{
//		return false;
//	}

	COMPILE_ASSERT(sizeof(Datum) == sizeof(int64));

	if (tupleDesc->natts != 1 || tupleDesc->attrs[0]->attlen != sizeof(int32))
	{
		return false;
	}


//	  template <typename MemberType,
//	            typename StructType,
//	            typename... PointerToMemberTypes>
//	  void CheckGetPointerToMemberConstant(
//	      std::vector<std::uintptr_t>* pointer_check_addresses,
//	      const StructType* external_struct,
//	      const std::size_t expected_offset,
//	      PointerToMemberTypes&&... pointers_to_members) {
//	    llvm::Constant* llvm_ptr_to_struct
//	        = codegen_utils_->GetConstant(external_struct);
//
//	    // Generate a function that returns the (constant) address of the member
//	    // field inside the struct.
//	    llvm::Function* global_member_accessor_fn
//	        = codegen_utils_->CreateFunction<std::uintptr_t>(
//	            GlobalConstantAccessorName(pointer_check_addresses->size()));
//	    llvm::BasicBlock* global_member_accessor_fn_body
//	        = codegen_utils_->CreateBasicBlock("body", global_member_accessor_fn);
//	    codegen_utils_->ir_builder()->SetInsertPoint(
//	        global_member_accessor_fn_body);
//	    llvm::Value* member_ptr = codegen_utils_->GetPointerToMember(
//	        llvm_ptr_to_struct,
//	        std::forward<PointerToMemberTypes>(pointers_to_members)...);
//	    llvm::Value* member_address = codegen_utils_->ir_builder()->CreatePtrToInt(
//	        member_ptr,
//	        codegen_utils_->GetType<std::uintptr_t>());
//	    codegen_utils_->ir_builder()->CreateRet(member_address);
//
//	    // Verify accessor function is well-formed.
//	    EXPECT_FALSE(llvm::verifyFunction(*global_member_accessor_fn));
//
//	    pointer_check_addresses->emplace_back(
//	        reinterpret_cast<std::uintptr_t>(external_struct) + expected_offset);
//	  }

	// void slot_deform_tuple_func(char* data_start_adress, void* values, void* isnull)
    llvm::Function* slot_deform_tuple_func
  	  = codegen_utils->CreateFunction<void, TupleTableSlot*, int>(
  			  "slot_deform_tuple_gen");

    // BasicBlocks for function entry.
    llvm::BasicBlock* entry_block = codegen_utils->CreateBasicBlock(
  	  "entry", slot_deform_tuple_func);

//    llvm::Value* input = gpcodegen::ArgumentByPosition(slot_deform_tuple_func, 0);
//    llvm::Value* out_values = gpcodegen::ArgumentByPosition(slot_deform_tuple_func, 1);

	irb->SetInsertPoint(entry_block);


	// Find the start of input data byte array
	llvm::Value* llvm_slot = codegen_utils->GetConstant(slot_);
	llvm::Value* llvm_heap_tup = irb->CreateLoad(codegen_utils->GetPointerToMember(llvm_slot, &TupleTableSlot::PRIVATE_tts_heaptuple));
	// This is our tup
	llvm::Value* llvm_t_data = irb->CreateLoad(codegen_utils->GetPointerToMember(llvm_heap_tup, &HeapTupleData::t_data));
	llvm::Value* llvm_t_hoff = irb->CreateLoad(codegen_utils->GetPointerToMember(llvm_t_data, &HeapTupleHeaderData::t_hoff));
	llvm::Value* llvm_input_start = irb->CreateInBoundsGEP(llvm_t_data, {llvm_t_hoff});

	llvm::Value* llvm_t_infomask = codegen_utils->GetPointerToMember(llvm_t_data, &HeapTupleHeaderData::t_infomask);

	// Find the start of the output datum array
	llvm::Value* llvm_out_values = irb->CreateLoad(codegen_utils->GetPointerToMember(llvm_slot, &TupleTableSlot::PRIVATE_tts_values));
	llvm::Value* llvm_out_is_null = irb->CreateLoad(codegen_utils->GetPointerToMember(llvm_slot, &TupleTableSlot::PRIVATE_tts_isnull));

	llvm::Value* input = llvm_input_start;
	llvm::Value* out_values = llvm_out_values;




	llvm::Value* llvm_loaded_t_infomask = irb->CreateLoad(llvm_t_infomask);
	uint16 null_mask = HEAP_HASNULL;
	llvm::Value* llvm_heap_tuple_has_null = irb->CreateAnd(llvm_loaded_t_infomask, codegen_utils->GetConstant(null_mask));


	// llvm_heap_tuple_has_null != 0 means we have nulls
	uint16 uint16_zero = 0;
	  llvm::Value* llvm_has_null = irb->CreateICmpNE(
			  llvm_heap_tuple_has_null,
		  codegen_utils->GetConstant(uint16_zero));


	  llvm::BasicBlock* fallback_case = codegen_utils->CreateBasicBlock(
	      "fallback_case",
		  slot_deform_tuple_func);
	  llvm::BasicBlock* gen_code_case = codegen_utils->CreateBasicBlock(
	      "gen_code_case",
		  slot_deform_tuple_func);

	  irb->CreateCondBr(llvm_has_null,
			  fallback_case,
			  gen_code_case);

	  irb->SetInsertPoint(fallback_case);


	  //======================
	  llvm::Function* llvm_fallback_func = GenerateSimpleSlotDeformTuple(codegen_utils);

		std::vector<llvm::Value*> forwarded_args;

		for (llvm::Argument& arg : slot_deform_tuple_func->args()) {
			forwarded_args.push_back(&arg);
		}

		llvm::CallInst* call = irb->CreateCall(
				llvm_fallback_func, forwarded_args);

		// Return the result of the call, or void if the function returns void.
		irb->CreateRet(call);

	  // =====================

	  irb->SetInsertPoint(gen_code_case);


	// Question: Do we use code generator's slot pointer or do we use runtime parameter's slot pointer?
	// Ans: Use code gen's to make it a constant
	// Steps:
	// Extract heap tuple from slot
	// Extract t_data field from heap tuple and store it into tup (start of the data that begins with a header and followed by bytes)
	// Extract t_hoff field from the tup
	// Calculate start of data bytes tup + tup->h_off => This is our input

	// Now we need a handle to the output array
	// slot->PRIVATE_tts_values is our output. But who allocates it? ExecSetSlotDescriptor already allocated it, before passing it to the enroller



    llvm::Value* true_const = codegen_utils->GetConstant(true);
    llvm::Value* datum_size_const = codegen_utils->GetConstant(sizeof(Datum));
    llvm::Value* bool_size_const = codegen_utils->GetConstant(sizeof(bool));

	int off = 0;

	Form_pg_attribute *att = tupleDesc->attrs;
	for (int attnum = 0; attnum < natts; attnum++)
	{
		Form_pg_attribute thisatt = att[attnum];
		off = att_align(off, thisatt->attalign);

		if (thisatt->attlen < 0)
		{
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
				{codegen_utils->GetConstant(off)});

		llvm::Value* next_address_store =
			irb->CreateInBoundsGEP(out_values,
				{codegen_utils->GetConstant(attnum)});

		llvm::Value* colVal = nullptr;

		// Load the value from the calculated input address.
		switch(thisatt->attlen)
		{
		case sizeof(char):
			// Read 1 byte at next_address_load
			colVal = irb->CreateLoad(next_address_load);
			// store colVal into out_values[attnum]
			break;
		case sizeof(int16):
			colVal = irb->CreateLoad(codegen_utils->GetType<int16>(),
					irb->CreateBitCast(next_address_load, codegen_utils->GetType<int16*>()));
			break;
		case sizeof(int32):
			colVal = irb->CreateLoad(codegen_utils->GetType<int32>(),
					irb->CreateBitCast(next_address_load, codegen_utils->GetType<int32*>()));
			break;
		case sizeof(int64):
			colVal = irb->CreateLoad(codegen_utils->GetType<int64>(),
					irb->CreateBitCast(next_address_load, codegen_utils->GetType<int64*>()));
			break;
		default:
			//TODO Cleanup
			return false;
		}

		llvm::Value* int64ColVal = irb->CreateZExt(colVal, codegen_utils->GetType<int64>());
		irb->CreateStore(int64ColVal, next_address_store);

		llvm::LoadInst* load_instruction =
			irb->CreateLoad(next_address_load, "input");

		off += thisatt->attlen;
	}

	irb->CreateMemSet(llvm_out_is_null, codegen_utils->GetConstant(0), natts * sizeof(*slot_->PRIVATE_tts_isnull), 1);
    irb->CreateRetVoid();

//	/*
//	 * Save state for next execution
//	 */
//	slot->PRIVATE_tts_nvalid = attnum;
//	slot->PRIVATE_tts_off = off;
//	slot->PRIVATE_tts_slow = slow;

    return true;
}


bool SlotDeformTupleCodeGen::DoCodeGeneration(CodeGenUtils* codegen_utils) {
	//elog(WARNING, "GenerateCode: %p, %s", codegen_utils, GetUniqueFuncName().c_str());

	//GenerateSimpleSlotDeformTuple(codegen_utils);
	GenerateSlotDeformTuple(codegen_utils);

	return true;
}
