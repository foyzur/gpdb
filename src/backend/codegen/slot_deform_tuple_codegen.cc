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
template <typename ReturnType, typename... ArgumentTypes>
ReturnType wrap_func(char* func_name, void* f, ArgumentTypes... arg) {
	elog(INFO, "Calling wrapped function: %s", func_name);
	reinterpret_cast<ReturnType (*)(ArgumentTypes...)>(f)(arg ...);
}

template <typename ReturnType, typename... ArgumentTypes>
void MakeWrapperFunction(CodeGen *code_gen, gpcodegen::CodeGenerator* code_generator,
    ReturnType (*external_function)(ArgumentTypes...),
    const std::string& wrapper_function_name) {
  // Register 'external_function' in 'code_generator_' and check that it has
  // the expected type-signature.

  llvm::Function* llvm_external_function
      = code_generator->RegisterExternalFunction(&wrap_func<ReturnType, ArgumentTypes...>);
  assert(llvm_external_function != nullptr);
  //wrap_func<void, const std::string&>(elog_wrapper, "test");
  // Create a wrapper function in 'code_generator_' with the same
  // type-signature that merely forwards its arguments to the external
  // function as-is.
  llvm::Function* wrapper_function
      = code_generator->CreateFunction<ReturnType, ArgumentTypes...>(
          wrapper_function_name);

  llvm::BasicBlock* wrapper_function_body
      = code_generator->CreateBasicBlock("wrapper_fn_body",
                                          wrapper_function);

  code_generator->ir_builder()->SetInsertPoint(wrapper_function_body);
  std::vector<llvm::Value*> forwarded_args;

  llvm::Value* func_name_llvm = code_generator->GetConstant(code_gen->GetFunctionPrefix());

  forwarded_args.push_back(func_name_llvm);
  // push the function to call inside wrapper
  forwarded_args.push_back(code_generator->GetConstant(reinterpret_cast<void*>(external_function)));

  for (llvm::Argument& arg : wrapper_function->args()) {
    forwarded_args.push_back(&arg);
  }
  llvm::CallInst* call = code_generator->ir_builder()->CreateCall(
      llvm_external_function,
      forwarded_args);

  // Return the result of the call, or void if the function returns void.
  if (std::is_same<ReturnType, void>::value) {
    code_generator->ir_builder()->CreateRetVoid();
  } else {
    code_generator->ir_builder()->CreateRet(call);
  }
}

bool SlotDeformTupleCodeGen::DoCodeGeneration(CodeGeneratorManager* manager,
			gpcodegen::CodeGenerator* code_generator) {
	elog(WARNING, "GenerateCode: %p, %s", code_generator, GetFuncName().c_str());

	MakeWrapperFunction(this, code_generator, GetRegularFuncPointer(), GetFuncName());

	return true;
}
