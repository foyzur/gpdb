//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		code_generator.cpp
//
//	@doc:
//		Contains different code generators
//
//---------------------------------------------------------------------------
#include "codegen/codegen.h"

using namespace gpcodegen;

long CodeGen::unique_counter_ = 0;

std::string CodeGen::GenerateUniqueName(const std::string& prefix)
{
	return prefix + std::to_string(unique_counter_++);
}
