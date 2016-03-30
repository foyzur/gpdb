//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		codegen_interface.h
//
//	@doc:
//		Interface for all code generator
//
//---------------------------------------------------------------------------
#ifndef CODEGEN_INTERFACE_H_
#define CODEGEN_INTERFACE_H_

#include <string>
#include <vector>

#include "utils/codegen_utils.h"

namespace gpcodegen
{
class CodeGenManager;

class CodeGenInterface
{
private:
	static long unique_counter_;

protected:
	static std::string GenerateUniqueName(const std::string& prefix);

public:
	virtual ~CodeGenInterface() = default;

	virtual bool GenerateCode(CodeGenManager* manager, gpcodegen::CodeGenUtils* codegen_utils) = 0;
	// sets the chosen function pointer to the regular version
	virtual bool SetToRegular() = 0;
	// sets the chosen function pointer to the code gened version
	virtual bool SetToGenerated(gpcodegen::CodeGenUtils* codegen_utils) = 0;
	// prepare for a new code generation; previous one is no longer valid
	virtual void Reset() = 0;

	// returns the generated unique function name
	virtual std::string GetFuncName() const = 0;
	virtual const char* GetFunctionPrefix() const = 0;
	virtual bool IsGenerated() const = 0;
};

}

#endif  // CODEGEN_INTERFACE_H_
