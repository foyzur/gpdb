//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		codegen.h
//
//	@doc:
//		Base class for all the code generators
//
//---------------------------------------------------------------------------
#ifndef BASE_CODEGEN_H_
#define BASE_CODEGEN_H_

#include <string>
#include <vector>

#include "codegen/utils/code_generator.h"
#include "codegen/utils/function_wrappers.h"

extern "C"
{
#include "utils/elog.h"
}

namespace code_gen
{
class CodeGeneratorManager;

class CodeGen
{
private:
	static long unique_counter_;
	std::string func_name_;
	bool is_generated_;

protected:
	static std::string GenerateUniqueName(const std::string& prefix);

public:
	virtual bool GenerateCode(CodeGeneratorManager* manager, gpcodegen::CodeGenerator* code_generator) {
		is_generated_ = GenerateCodeImpl(manager, code_generator);
	}

	virtual bool GenerateCodeImpl(CodeGeneratorManager* manager, gpcodegen::CodeGenerator* code_generator) = 0;

	//
	// sets the chosen function pointer to the regular version
	virtual bool SetToRegular() = 0;
	// sets the chosen function pointer to the code gened version
	virtual bool SetToGenerated(gpcodegen::CodeGenerator* code_generator) = 0;
	// prepare for a new code generation; previous one is no longer valid
	virtual void Reset() = 0;

	// returns the generated unique function name
	virtual std::string GetFuncName()
	{
		return func_name_;
	}

	virtual const char* GetFunctionPrefix() = 0;

	bool IsGenerated() {
		return is_generated_;
	}

	explicit CodeGen(const std::string& prefix):func_name_(CodeGen::GenerateUniqueName(prefix)), is_generated_(false)
	{

	}
	virtual ~CodeGen() = default;
};

template <class FuncPtrType>
class BasicCodeGen: public CodeGen
{
private:
	FuncPtrType regular_func_ptr_;
	FuncPtrType* ptr_to_chosen_func_ptr_;

protected:
	explicit BasicCodeGen(const std::string& prefix, FuncPtrType regular_func_ptr, FuncPtrType* ptr_to_chosen_func_ptr):
		CodeGen(prefix), regular_func_ptr_(regular_func_ptr), ptr_to_chosen_func_ptr_(ptr_to_chosen_func_ptr) {
		// initialize the usable function pointer to the regular version
		SetToRegular(regular_func_ptr, ptr_to_chosen_func_ptr);
	}

public:
	static bool SetToRegular(FuncPtrType regular_func_ptr, FuncPtrType* ptr_to_chosen_func_ptr)
	{
		*ptr_to_chosen_func_ptr = regular_func_ptr;
		return true;
	}

	// sets the chosen function pointer to the regular version
	virtual bool SetToRegular()
	{
		assert(nullptr != regular_func_ptr_);
		SetToRegular(regular_func_ptr_, ptr_to_chosen_func_ptr_);
		return true;
	}

	FuncPtrType GetRegularFuncPointer()
	{
		return regular_func_ptr_;
	}

	// sets the chosen function pointer to the code gened version
	virtual bool SetToGenerated(gpcodegen::CodeGenerator* code_generator)
	{
		if (false == IsGenerated()) {
			assert(*ptr_to_chosen_func_ptr_ == regular_func_ptr_);
			return false;
		}

		elog(WARNING, "SetToGenerated: %p, %s", code_generator, GetFuncName().c_str());

		auto compiled_func_ptr = gpcodegen::FunctionTraitWrapper<FuncPtrType>::GetFunctionPointerHelper(code_generator, GetFuncName());
		elog(WARNING, "compiled_func_ptr: %p", compiled_func_ptr);
		//auto compiled_func_ptr = code_generator->GetFunctionPointer<traits::return_type, traits::arg_type>(func_name_);
		assert(nullptr != compiled_func_ptr);

		if (nullptr != compiled_func_ptr)
		{
			*ptr_to_chosen_func_ptr_ = compiled_func_ptr;
			return true;
		}

		return false;
	}

	// prepare for a new code generation; previous one is no longer valid
	virtual void Reset()
	{
		SetToRegular();
	}

	virtual ~BasicCodeGen() = default;
};


}

#endif  // BASE_CODEGEN_H_
