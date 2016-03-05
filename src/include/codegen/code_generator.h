//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		code_generator.h
//
//	@doc:
//		C wrappers for various code generators
//
//---------------------------------------------------------------------------
#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_

typedef void (*SlotDeformTupleFn) (char*, void*);

#ifdef __cplusplus
extern "C" {
#endif

char* GenerateSlotDeformTuple(void *object, void* manager, void* code_gen);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // CODE_GENERATOR_H_
