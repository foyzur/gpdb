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
#include "codegen/codegen_manager.h"
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
//#include "access/htup.h"
//#include "access/tupmacs.h"
//#include "c.h"
#include "catalog/pg_attribute.h"


CodeGeneratorManager::CodeGeneratorManager() {
	code_generator_.reset(new balerion::CodeGenerator("test_module"));
}

CodeGeneratorManager::~CodeGeneratorManager() {
	allCodeGenedFucInfo.clear();
}

CodeGenFuncInfo* CodeGeneratorManager::RegisterCodeGenerator(CodeGenFuncLifespan funcLifespan, void* object,
		CodeGeneratorCallback generator, void* regular_func_pointer, void** called_func_pointer_addr) {

	assert(funcLifespan == CodeGenFuncLifespan_Parameter_Invariant);
//	std::unique_ptr<CodeGenFuncInfo>* code_gen_func_inf =
//			std::unique_ptr<CodeGenFuncInfo>(new CodeGenFuncInfo(object,
//					generator, regular_func_pointer, called_func_pointer_addr));
	allCodeGenedFucInfo.emplace_back(object,
			generator, regular_func_pointer, called_func_pointer_addr);
	return allCodeGenedFucInfo.back();
}

bool CodeGeneratorManager::GenerateCode() {
	for(auto code_gen_func_info : allCodeGenedFucInfo) {
		code_gen_func_info.get()->GenerateCode(this, code_generator_.get());
	}
}

