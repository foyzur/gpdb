//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		base_codegen.h
//
//	@doc:
//		Base class for all the code generators with common implementation
//
//---------------------------------------------------------------------------
#ifndef BASE_CODEGEN_H_
#define BASE_CODEGEN_H_

#include <string>
#include <vector>

#include "codegen/utils/codegen_utils.h"
#include "codegen/codegen_interface.h"


extern "C"
{
#include "utils/elog.h"
}

namespace gpcodegen
{
class CodeGeneratorManager;

template <class FuncPtrType>
class BaseCodeGen: public CodeGenInterface
{
private:
	std::string func_name_;
	FuncPtrType regular_func_ptr_;
	FuncPtrType* ptr_to_chosen_func_ptr_;
	bool is_generated_;

protected:
	explicit BaseCodeGen(const std::string& prefix, FuncPtrType regular_func_ptr, FuncPtrType* ptr_to_chosen_func_ptr):
		func_name_(CodeGenInterface::GenerateUniqueName(prefix)), regular_func_ptr_(regular_func_ptr),
		ptr_to_chosen_func_ptr_(ptr_to_chosen_func_ptr), is_generated_(false) {
		// initialize the usable function pointer to the regular version
		SetToRegular(regular_func_ptr, ptr_to_chosen_func_ptr);
	}

public:

  virtual ~BaseCodeGen() {
	  SetToRegular(regular_func_ptr_, ptr_to_chosen_func_ptr_);
  }

  // a template method design pattern to be overridden by the sub-class to implement the actual code generation
	virtual bool DoCodeGeneration(CodeGeneratorManager* manager, gpcodegen::CodeGenUtils* codegen_utils) = 0;

	virtual bool GenerateCode(CodeGeneratorManager* manager, gpcodegen::CodeGenUtils* codegen_utils) override final {
		is_generated_ = DoCodeGeneration(manager, codegen_utils);
	}

	// Sets the chosen function pointer to the regular version.
	virtual bool SetToRegular() override final {
		assert(nullptr != regular_func_ptr_);
		SetToRegular(regular_func_ptr_, ptr_to_chosen_func_ptr_);
		return true;
	}

	// sets the chosen function pointer to the code gened version
	virtual bool SetToGenerated(gpcodegen::CodeGenUtils* codegen_utils) override final {
		if (false == IsGenerated()) {
			assert(*ptr_to_chosen_func_ptr_ == regular_func_ptr_);
			return false;
		}

		elog(WARNING, "SetToGenerated: %p, %s", codegen_utils, GetFuncName().c_str());
		auto compiled_func_ptr = codegen_utils->GetFunctionPointerTypeDef<FuncPtrType>(GetFuncName());

		elog(WARNING, "compiled_func_ptr: %p", compiled_func_ptr);
		//auto compiled_func_ptr = codegen_utils->GetFunctionPointer<traits::return_type, traits::arg_type>(func_name_);
		//assert(nullptr != compiled_func_ptr);

		if (nullptr != compiled_func_ptr)
		{
			*ptr_to_chosen_func_ptr_ = compiled_func_ptr;
			return true;
		}

		return false;
	}

	// prepare for a new code generation; previous one is no longer valid
	virtual void Reset() override final {
		SetToRegular();
	}

	// returns the generated unique function name
	virtual std::string GetFuncName() const override final {
		return func_name_;
	}

	virtual bool IsGenerated() const override final {
		return is_generated_;
	}

	FuncPtrType GetRegularFuncPointer() {
		return regular_func_ptr_;
	}

	static bool SetToRegular(FuncPtrType regular_func_ptr, FuncPtrType* ptr_to_chosen_func_ptr) {
		*ptr_to_chosen_func_ptr = regular_func_ptr;
		return true;
	}
};


}

#endif  // BASE_CODEGEN_H_
