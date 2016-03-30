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

/** \addtogroup gpcodegen
 *  @{
 */

// Forward declaration of manager
class CodeGenManager;

/**
 * @brief Interface for all code generators.
 **/
class CodeGenInterface
{
public:

	virtual ~CodeGenInterface() = default;

	/**
   * @brief Generates specialized code at run time.
   *
   *
   * @param codegen_utils Utility to ease the code generation process.
   * @return true on successful generation.
   **/
	virtual bool GenerateCode(gpcodegen::CodeGenUtils* codegen_utils) = 0;

	/**
   * @brief Sets up the caller to use the corresponding regular version of the generated function.
   *
   *
   * @return true on setting to regular version.
   **/
	virtual bool SetToRegular() = 0;

	/**
   * @brief Sets up the caller to use the generated function instead of the regular version.
   *
   * @param codegen_utils Facilitates in obtaining the function pointer from the compiled module.
   * @return true on successfully setting to generated functions
   **/
	virtual bool SetToGenerated(gpcodegen::CodeGenUtils* codegen_utils) = 0;

	/**
   * @brief Resets the state of the generator, including reverting back to regular
   *        version of the function.
   *
   **/
	virtual void Reset() = 0;

	/**
   * @return Unique function name of the generated function.
   *
   **/
	virtual std::string GetUniqueFuncName() const = 0;

	virtual const char* GetOrigFunctionName() const = 0;
	virtual bool IsGenerated() const = 0;

protected:
  static std::string GenerateUniqueName(const std::string& prefix);

private:
  static long unique_counter_;


};

/** @} */

}

#endif  // CODEGEN_INTERFACE_H_
