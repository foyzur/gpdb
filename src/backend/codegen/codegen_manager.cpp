//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		codegen_manager.cpp
//
//	@doc:
//		Implementation of a code generator manager
//
//---------------------------------------------------------------------------

#include <cstdint>
#include <string>

#include "balerion/clang_compiler.h"
#include "balerion/code_generator.h"
#include "balerion/utility.h"
#include "balerion/instance_method_wrappers.h"

#include "codegen/codegen_manager.h"
#include "codegen/codegen.h"

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
//#include "access/htup.h"
//#include "access/tupmacs.h"
//#include "c.h"
#include "catalog/pg_attribute.h"
extern "C"
{
#include "utils/elog.h"
}

using namespace code_gen;

CodeGeneratorManager::CodeGeneratorManager() {
	code_generator_.reset(new balerion::CodeGenerator("test_module"));
}

bool CodeGeneratorManager::EnrollCodeGenerator(CodeGenFuncLifespan funcLifespan, CodeGen* generator) {

	assert(funcLifespan == CodeGenFuncLifespan_Parameter_Invariant);
	assert(nullptr != generator);
	enrolled_code_generators_.emplace_back(generator);
	return true;
}

bool CodeGeneratorManager::GenerateCode() {
	for(auto& generator : enrolled_code_generators_) {
		generator->GenerateCode(this, code_generator_.get());
	}

	return true;
}

bool CodeGeneratorManager::PrepareGeneratedFunctions() {
	elog(WARNING, "Compiling everything: %p", code_generator_.get());
	bool compilation_status = code_generator_->PrepareForExecution(balerion::CodeGenerator::OptimizationLevel::kNone, true);

	if (!compilation_status)
	{
		elog(WARNING, "Cannot compile");
		return compilation_status;
	}

	balerion::CodeGenerator* llvm_helper = code_generator_.get();
	for(auto& generator : enrolled_code_generators_) {
		generator->SetToGenerated(llvm_helper);
	}

	return true;
}

// notifies that the underlying operator has a parameter change
bool CodeGeneratorManager::NotifyParameterChange() {
	// no support for parameter change yet
	assert(false);
	return false;
}

// Invalidate all generated functions
bool CodeGeneratorManager::InvalidateGeneratedFunctions() {
	// no support for invalidation of generated function
	assert(false);
	return false;
}
