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

#include "codegen/utils/clang_compiler.h"
#include "codegen/utils/code_generator.h"
#include "codegen/utils/utility.h"
#include "codegen/utils/instance_method_wrappers.h"

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

using namespace gpcodegen;

constexpr char SlotDeformTupleCodeGen::kSlotDeformTupleNamePrefix[];

SlotDeformTupleCodeGen::SlotDeformTupleCodeGen(TupleTableSlot* slot,
			SlotDeformTupleFn regular_func_ptr, SlotDeformTupleFn* ptr_to_regular_func_ptr):
		BasicCodeGen(kSlotDeformTupleNamePrefix, regular_func_ptr, ptr_to_regular_func_ptr),
		slot_(slot)
{

}

//template <const char* func_name, typename ReturnType, typename... ArgumentTypes>
static void ElogWrapper(const char* func_name) {
	elog(INFO, "Calling wrapped function: %s", func_name);
}


void SlotDeformTupleCodeGen::MakeWrapperFunction(gpcodegen::CodeGenerator* code_generator) {
  // Register 'external_function' in 'code_generator_' and check that it has
  // the expected type-signature.

  llvm::Function* llvm_elog_wrapper
      = code_generator->RegisterExternalFunction(ElogWrapper);
  assert(llvm_elog_wrapper != nullptr);

  auto regular_func_pointer = GetRegularFuncPointer();
  llvm::Function* llvm_regular_function
      = code_generator->RegisterExternalFunction(regular_func_pointer);
  assert(llvm_regular_function != nullptr);


  //wrap_func<void, const std::string&>(elog_wrapper, "test");
  // Create a wrapper function in 'code_generator_' with the same
  // type-signature that merely forwards its arguments to the external
  // function as-is.
  llvm::Function* llvm_function
      = code_generator->CreateFunctionTypeDef<decltype(regular_func_pointer)>(GetFuncName());

  llvm::BasicBlock* function_body
      = code_generator->CreateBasicBlock("fn_body",
    		  llvm_function);

  code_generator->ir_builder()->SetInsertPoint(function_body);
  llvm::Value* func_name_llvm = code_generator->GetConstant(this->GetFunctionPrefix());
  code_generator->ir_builder()->CreateCall(
        llvm_elog_wrapper, {func_name_llvm});

  std::vector<llvm::Value*> forwarded_args;

  for (llvm::Argument& arg : llvm_function->args()) {
    forwarded_args.push_back(&arg);
  }

  llvm::CallInst* call = code_generator->ir_builder()->CreateCall(
	  llvm_regular_function,
      forwarded_args);

  // Return the result of the call, or void if the function returns void.
  if (std::is_same<gpcodegen::code_generator_detail::FunctionTypeUnpacker<decltype(regular_func_pointer)>::R, void>::value) {
    code_generator->ir_builder()->CreateRetVoid();
  } else {
    code_generator->ir_builder()->CreateRet(call);
  }
}

bool SlotDeformTupleCodeGen::DoCodeGeneration(CodeGeneratorManager* manager,
			gpcodegen::CodeGenerator* code_generator) {
	elog(WARNING, "GenerateCode: %p, %s", code_generator, GetFuncName().c_str());

	MakeWrapperFunction(code_generator);

	return true;
}
