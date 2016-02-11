//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		init_codegen.h
//
//	@doc:
//		C wrappers for initialization of Balerion codegen library
//
//---------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

int InitCodeGen();

void* ConstructCodeGenerator();

void PrepareForExecution(void* code_generator);

void DestructCodeGenerator(void* code_generator);

int (*GetDummyFunction(void* code_generator)) (int);

#ifdef __cplusplus
}  // extern "C"
#endif
