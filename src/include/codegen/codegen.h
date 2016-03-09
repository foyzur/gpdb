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

#include "balerion/code_generator.h"

namespace code_gen {

template<typename FunctionType>
struct function_traits;

template<typename ReturnType, typename... ArgumentTypes>
struct function_traits<ReturnType(*)(ArgumentTypes...)> {
public:
	static auto GetFunctionPointerHelper(balerion::CodeGenerator* code_generator, const std::string& func_name)
    -> ReturnType (*)(ArgumentTypes...) {
		return code_generator->GetFunctionPointer<ReturnType, ArgumentTypes...>(func_name);
	}

	static llvm::Function* CreateFunctionHelper(balerion::CodeGenerator* code_generator, const std::string& func_name) {
		return code_generator->CreateFunction<ReturnType, ArgumentTypes...>(func_name);
	}
};

class CodeGeneratorManager;

class CodeGen {
private:
	static long unique_counter_;

protected:
	static std::string GenerateUniqueName(const std::string& prefix);

public:
	virtual bool GenerateCode(CodeGeneratorManager* manager, balerion::CodeGenerator* code_generator) = 0;
	//
	// sets the chosen function pointer to the regular version
	virtual bool SetToRegular() = 0;
	// sets the chosen function pointer to the code gened version
	virtual bool SetToGenerated(balerion::CodeGenerator* code_generator) = 0;
	// prepare for a new code generation; previous one is no longer valid
	virtual void Reset() = 0;

	// returns the generated unique function name
	virtual std::string GetFuncName() = 0;

	explicit CodeGen() = default;
	virtual ~CodeGen() = default;
};

template <class FuncPtrType>
class BasicCodeGen: public CodeGen {
private:
	FuncPtrType regular_func_ptr_;
	FuncPtrType* ptr_to_chosen_func_ptr_;
	std::string func_name_;

protected:
	explicit BasicCodeGen(const std::string& prefix,
			FuncPtrType regular_func_ptr, FuncPtrType* ptr_to_chosen_func_ptr):
		regular_func_ptr_(regular_func_ptr), ptr_to_chosen_func_ptr_(ptr_to_chosen_func_ptr) {
		func_name_ = CodeGen::GenerateUniqueName(prefix);
		// initialize the usable function pointer to the regular version
		*ptr_to_chosen_func_ptr = regular_func_ptr;
	}

public:
	// sets the chosen function pointer to the regular version
	virtual bool SetToRegular() {
		assert(nullptr != regular_func_ptr_);
		*ptr_to_chosen_func_ptr_ = regular_func_ptr_;
		return true;
	}

	// sets the chosen function pointer to the code gened version
	virtual bool SetToGenerated(balerion::CodeGenerator* code_generator) {
		auto compiled_func_ptr = function_traits<FuncPtrType>::GetFunctionPointerHelper(code_generator, func_name_);
		//auto compiled_func_ptr = code_generator->GetFunctionPointer<traits::return_type, traits::arg_type>(func_name_);
		assert(nullptr != compiled_func_ptr);

		if (nullptr != compiled_func_ptr) {
			*ptr_to_chosen_func_ptr_ = compiled_func_ptr;
			return true;
		}

		return false;
	}

	// prepare for a new code generation; previous one is no longer valid
	virtual void Reset() {
		SetToRegular();
	}

	virtual std::string GetFuncName() {
		return func_name_;
	}

	virtual ~BasicCodeGen() = default;
};


}

#endif  // BASE_CODEGEN_H_
