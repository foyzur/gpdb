//---------------------------------------------------------------------------
//  Greenplum Database
//  Copyright 2016 Pivotal Software, Inc.
//
//  @filename:
//    function_wrappers.h
//
//  @doc:
//    Templated helper class that provides a static helper method which
//    unpack the return type, argument types and call the respective method
//
//  @test:
//
//
//---------------------------------------------------------------------------

#ifndef GPCODEGEN_FUNCTION_WRAPPER_H_
#define GPCODEGEN_FUNCTION_WRAPPER_H_

#include "llvm/IR/Function.h"

namespace gpcodegen {

/** \addtogroup gpcodegen
 *  @{
 */

/**
 * @brief Templated helper class that provides a static method which
 *        unpack the return type, argument types and call the respective
 *        method
 *
 * @tparam FunctionType Function type.
 * @tparam MethodPtr A pointer-to-method.
 **/
template <typename FunctionType>
class FunctionTraitWrapper;

/**
 * @brief Partial specialization of FunctionTraitWrapper
 * GetFunctionPointerHelper - Call codegnerator's GetFunctionPointer
 * CreateFunction
 **/
template<typename ReturnType, typename... ArgumentTypes>
class FunctionTraitWrapper<ReturnType(*)(ArgumentTypes...)>
{
public:
	static auto GetFunctionPointerHelper(gpcodegen::CodeGenerator* code_generator, const std::string& func_name)
    -> ReturnType (*)(ArgumentTypes...) {
		return code_generator->GetFunctionPointer<ReturnType, ArgumentTypes...>(func_name);
	}

	static llvm::Function* CreateFunctionHelper(gpcodegen::CodeGenerator* code_generator, const std::string& func_name) {
		return code_generator->CreateFunction<ReturnType, ArgumentTypes...>(func_name);
	}
};

/** @} */

} //namespace gpcodegen

#endif /* GPCODEGEN_METHOD_WRAPPER_H_ */
