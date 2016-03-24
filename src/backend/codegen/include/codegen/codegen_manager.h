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

#include <memory>
#include <vector>

#include "codegen/codegen_wrapper.h"

namespace gpcodegen {
	class CodeGenerator;
}

namespace code_gen
{

class CodeGen;

class CodeGeneratorManager
{
private:
	// CodeGenerator provides a facade to LLVM subsystem
	std::unique_ptr<gpcodegen::CodeGenerator> code_generator_;

	// list of all enrolled code generators
	std::vector<std::unique_ptr<CodeGen>> enrolled_code_generators_;

public:
	// Constructor
	explicit CodeGeneratorManager();
	~CodeGeneratorManager() = default;

	// Enroll a new code generator
	bool EnrollCodeGenerator(CodeGenFuncLifespan funcLifespan, CodeGen* generator);

	// Ask all the generators to generate code
	bool GenerateCode();

	// Compile and prepare all the generated functions
	bool PrepareGeneratedFunctions();

	// notifies that the underlying operator has a parameter change
	bool NotifyParameterChange();

	// Invalidate all generated functions
	bool InvalidateGeneratedFunctions();
};

}
#endif  // CODEGEN_MANAGER_H_
