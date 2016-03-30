//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		codegen_manager.h
//
//	@doc:
//		Object that manage all CodeGen and CodeGenUtils
//
//---------------------------------------------------------------------------

#ifndef CODEGEN_MANAGER_H_
#define CODEGEN_MANAGER_H_

#include <memory>
#include <vector>

#include "codegen/utils/macros.h"
#include "codegen/codegen_wrapper.h"

namespace gpcodegen
{
/** \addtogroup gpcodegen
 *  @{
 */

// Forward declaration of CodeGenUtils to manage llvm module
class CodeGenUtils;

// Forward declaration of a CodeGenInterface that will be managed by manager
class CodeGenInterface;

/**
 * @brief Object that manages all code gen.
 **/
class CodeGenManager
{
public:
  /**
   * @brief Constructor.
   *
   **/
	explicit CodeGenManager();

	~CodeGenManager() = default;

	/**
   * @brief Enroll a code generator with manager
   *
   * @note Manager manage the memory of enrolled generator.
   *
   * @param funcLifespan Based on life span corresponding CodeGen_Utils will be used to generate
   * @param generator    Generator that needs to be enrolled with manager.
   * @return true on successful enrollment
   **/
	bool EnrollCodeGenerator(CodeGenFuncLifespan funcLifespan, CodeGenInterface* generator);

	/**
   * @brief Make all enrolled generators to generate code
   *
   * @return true on successful enrollment
   **/
	bool GenerateCode();

	/**
   * @brief Compile all the generated functions. On success, set compiled function
   *        to be called instead of regular functions
   *
   * @return true on successful compilation or return false
   **/
	bool PrepareGeneratedFunctions();

	// notifies that the underlying operator has a parameter change
	bool NotifyParameterChange();

	// Invalidate all generated functions
	bool InvalidateGeneratedFunctions();

private:
  // CodeGenUtils provides a facade to LLVM subsystem
  std::unique_ptr<gpcodegen::CodeGenUtils> codegen_utils_;

  // list of all enrolled code generators
  std::vector<std::unique_ptr<CodeGenInterface>> enrolled_code_generators_;

  DISALLOW_COPY_AND_ASSIGN(CodeGenManager);

};

/** @} */

}
#endif  // CODEGEN_MANAGER_H_
