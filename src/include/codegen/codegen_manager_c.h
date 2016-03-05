//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		codegen_manager_c.h
//
//	@doc:
//		Headers for codegen manager's C counterpart
//
//---------------------------------------------------------------------------
#ifndef CODEGEN_MANAGER_C_H_
#define CODEGEN_MANAGER_C_H_

// prototype for code generator callback. It will return the generated function name
typedef char* (*CodeGeneratorCallback)(void *object, void* manager, void* code_gen);

typedef enum CodeGenFuncLifespan
{
	// does not depend on parameter changes
	CodeGenFuncLifespan_Parameter_Invariant,
	// has to be regenerated as the parameter changes
	CodeGenFuncLifespan_Parameter_Variant
} CodeGenFuncLifespan;

// the current code generator manager that oversees all code generators
void* ActiveCodeGeneratorManager;

/*
 * START_CODE_GENERATOR_MANAGER would switch to the specified code generator manager,
 * saving the oldCodeGeneratorManager. Must be paired with END_CODE_GENERATOR_MANAGER
 */
#define START_CODE_GENERATOR_MANAGER(newManager)  \
	do { \
		void *oldManager = NULL; \
		Assert(newManager != NULL); \
		oldManager = ActiveCodeGeneratorManager; \
		ActiveCodeGeneratorManager = newManager;\
/*
 * END_CODE_GENERATOR_MANAGER would restore the previous code generator manager that was
 * active at the time of START_CODE_GENERATOR_MANAGER call
 */
#define END_CODE_GENERATOR_MANAGER()  \
		ActiveCodeGeneratorManager = oldManager;\
	} while (0);

#ifdef __cplusplus
extern "C" {
#endif

// creates a manager for an operator
void* CodeGeneratorManager_Create();

// returns the pointer to the CodeGenFuncInfo
void* CodeGeneratorManager_Register(CodeGenFuncLifespan funcLifespan, void* object,
		CodeGeneratorCallback generator, void* regular_func_pointer, void** called_func_pointer_addr);

// calls all the registered CodeGenFuncInfo to generate code
bool CodeGeneratorManager_GenerateCode(void* manager);

// compiles and prepares all the code gened function pointers
bool CodeGeneratorManager_PrepareGeneratedFunctions(void* manager);

// notifies a manager that the underlying operator has a parameter change
bool CodeGeneratorManager_NotifyParameterChange(void* manager);

// destroys a manager for an operator
void CodeGeneratorManager_Destroy(void* manager);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // CODEGEN_MANAGER_C_H_
