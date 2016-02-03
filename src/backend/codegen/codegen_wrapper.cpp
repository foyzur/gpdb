//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		init_codegen.cpp
//
//	@doc:
//		C wrappers for initialization of code generator.
//
//---------------------------------------------------------------------------

#include "balerion/code_generator.h"
#include "codegen/codegen_wrapper.h"
#include "codegen/codegen_manager.h"
#include "codegen/slot_deform_tuple_codegen.h"

#include "postgres.h"
#include "c.h"
#include "access/htup.h"
#include "access/tupmacs.h"
#include "catalog/pg_attribute.h"
#include "executor/tuptable.h"

using namespace code_gen;

// the current code generator manager that oversees all code generators
static void* ActiveCodeGeneratorManager = nullptr;

// Perform global set-up tasks for Balerion codegen library. Returns 0 on
// success, nonzero on error.
int InitCodeGen() {
  return balerion::CodeGenerator::InitializeGlobal() ? 0 : 1;
}

// creates a manager for an operator
void* CodeGeneratorManager_Create() {
	return new CodeGeneratorManager();
}

// calls all the registered CodeGenFuncInfo to generate code
bool CodeGeneratorManager_GenerateCode(void* manager) {
	return static_cast<CodeGeneratorManager*>(manager)->GenerateCode();
}

// compiles and prepares all the code gened function pointers
bool CodeGeneratorManager_PrepareGeneratedFunctions(void* manager) {
	return static_cast<CodeGeneratorManager*>(manager)->PrepareGeneratedFunctions();
}

// notifies a manager that the underlying operator has a parameter change
bool CodeGeneratorManager_NotifyParameterChange(void* manager) {
	// parameter change notification is not supported yet
	assert(false);
	return false;
}

// destroys a manager for an operator
void CodeGeneratorManager_Destroy(void* manager) {
	delete (static_cast<CodeGeneratorManager*>(manager));
}

void* GetActiveCodeGeneratorManager() {
	return ActiveCodeGeneratorManager;
}

void SetActiveCodeGeneratorManager(void* manager) {
	ActiveCodeGeneratorManager = manager;
}

void* SlotDeformTupleCodeGen_Enroll(TupleTableSlot* slot, SlotDeformTupleFn regular_func_ptr,
		SlotDeformTupleFn* ptr_to_regular_func_ptr)
{
	SlotDeformTupleCodeGen* generator = new SlotDeformTupleCodeGen(slot,
			regular_func_ptr, ptr_to_regular_func_ptr);
	CodeGeneratorManager* manager = static_cast<CodeGeneratorManager*>(
			GetActiveCodeGeneratorManager());
	assert(nullptr != manager);
	bool is_enrolled = manager->EnrollCodeGenerator(CodeGenFuncLifespan_Parameter_Invariant,
			generator);
	assert(is_enrolled);
	return generator;
}

