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

/** \addtogroup gpcodegen
 *  @{
 */


/**
 * @brief Base code gen with common implementation that other code gen can use.
 *
 * @tparm function type for regular / generated functions
 **/
template <class FuncPtrType>
class BaseCodeGen: public CodeGenInterface
{

public:

  /**
   * @brief Destructor. Make sure when it get destroyed, we sets up the corresponding
   *        caller to use regular version of the generated function.
   **/
  virtual ~BaseCodeGen() {
	  SetToRegular(regular_func_ptr_, ptr_to_chosen_func_ptr_);
  }

  /**
   * @brief Generates specialized code at run time.
   *
   *
   * @param codegen_utils Utility to ease the code generation process.
   * @return true on successful generation.
   **/
  virtual bool GenerateCode(gpcodegen::CodeGenUtils* codegen_utils) override final {
		is_generated_ = DoCodeGeneration(codegen_utils);
		return is_generated_;
	}

  /**
   * @brief Sets up the caller to use the corresponding regular version of the
   *        generated function.
   *
   * @return true on setting to regular version.
   **/
	virtual bool SetToRegular() override final {
		assert(nullptr != regular_func_ptr_);
		SetToRegular(regular_func_ptr_, ptr_to_chosen_func_ptr_);
		return true;
	}

	/**
   * @brief Sets up the caller to use the generated function instead of the
   *        regular version.
   *
   * @param codegen_utils Facilitates in obtaining the function pointer from
   *        the compiled module.
   * @return true on successfully setting to generated functions.
   **/
	virtual bool SetToGenerated(gpcodegen::CodeGenUtils* codegen_utils) override final {
		if (false == IsGenerated()) {
			assert(*ptr_to_chosen_func_ptr_ == regular_func_ptr_);
			return false;
		}

		elog(WARNING, "SetToGenerated: %p, %s", codegen_utils, GetUniqueFuncName().c_str());
		auto compiled_func_ptr = codegen_utils->GetFunctionPointerTypeDef<FuncPtrType>(GetUniqueFuncName());

		elog(WARNING, "compiled_func_ptr: %p", compiled_func_ptr);

		if (nullptr != compiled_func_ptr) {
			*ptr_to_chosen_func_ptr_ = compiled_func_ptr;
			return true;
		}
		return false;
	}

	/**
   * @brief Resets the state of the generator, including reverting back to
   *        regular version of the function.
   *
   **/
	virtual void Reset() override final {
		SetToRegular();
	}

	/**
   * @return Unique function name of the generated function.
   *
   **/
	virtual const std::string& GetUniqueFuncName() const override final {
		return unique_func_name_;
	}

	/**
   * @return true if the generation is successful.
   *
   **/
	virtual bool IsGenerated() const override final {
		return is_generated_;
	}

	/**
   * @return regular version of the corresponding generated function.
   *
   **/
	FuncPtrType GetRegularFuncPointer() {
		return regular_func_ptr_;
	}

	/**
   * @brief Sets up the caller to use the corresponding regular version of the
   *        generated function.
   *
   * @param regular_func_ptr       Regular version of the generated function.
   * @param ptr_to_chosen_func_ptr Reference to caller.
   *
   * @return true on setting to regular version.
   **/
	static bool SetToRegular(FuncPtrType regular_func_ptr, FuncPtrType* ptr_to_chosen_func_ptr) {
		*ptr_to_chosen_func_ptr = regular_func_ptr;
		return true;
	}

protected:

	/**
   * @brief Constructor
   *
   * @param orig_func_name         Original function name,
   * @param regular_func_ptr       Regular version of the generated function.
   * @param ptr_to_chosen_func_ptr Reference to caller.
   *
   **/
  explicit BaseCodeGen(const std::string& orig_func_name, FuncPtrType regular_func_ptr,
                       FuncPtrType* ptr_to_chosen_func_ptr):
    unique_func_name_(CodeGenInterface::GenerateUniqueName(orig_func_name)),
    regular_func_ptr_(regular_func_ptr),
    ptr_to_chosen_func_ptr_(ptr_to_chosen_func_ptr), is_generated_(false) {

    // Initialize the caller to use regular version of generated function.
    SetToRegular(regular_func_ptr, ptr_to_chosen_func_ptr);
  }

  // a template method design pattern to be overridden by the sub-class to implement the actual code generation
  /**
   * @brief Generates specialized code at run time.
   *
   * @note  This is being called from GenerateCode and derived class will implement actual
   *        code generation
   * @param codegen_utils Utility to ease the code generation process.
   * @return true on successful generation.
   **/
  virtual bool DoCodeGeneration(gpcodegen::CodeGenUtils* codegen_utils) = 0;

private:
  std::string unique_func_name_;
  FuncPtrType regular_func_ptr_;
  FuncPtrType* ptr_to_chosen_func_ptr_;
  bool is_generated_;
};
/** @} */
}

#endif  // BASE_CODEGEN_H_
