//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		codegen_manager.h
//
//	@doc:
//		Headers for codegen manager
//
//---------------------------------------------------------------------------
#ifndef CODEGEN_MANAGER_H_
#define CODEGEN_MANAGER_H_

#include "codegen/codegen_manager_c.h"
#include "codegen/codegen_func_info.h"

class CodeGeneratorManager
{
private:
	std::unique_ptr<balerion::CodeGenerator> code_generator_; // CodeGenerator provides a facade to LLVM subsystem
	std::vector<std::unique_ptr<CodeGenFuncInfo>> allCodeGenedFucInfo; // List of pointers to codegened functions’ info };

public:
	// Constructor
	CodeGeneratorManager();

	// Register a new code generator
	CodeGenFuncInfo* RegisterCodeGenerator(CodeGenFuncLifespan funcLifespan, void* object,
		CodeGeneratorCallback generator, void* regular_func_pointer, void** called_func_pointer_addr);

	// Ask all the generators to generate code
	bool GenerateCode();

	// Compile and prepare all the generated functions
	bool PrepareGeneratedFunctions();

	// notifies that the underlying operator has a parameter change
	bool CodeGeneratorManager_NotifyParameterChange(void* manager);

	// Invalidate all generated functions
	bool InvalidateGeneratedFunctions();
};

#endif  // CODEGEN_MANAGER_H_
