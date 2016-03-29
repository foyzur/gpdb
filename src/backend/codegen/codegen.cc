//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		codegen.cc
//
//	@doc:
//		Implementation of codegen interface's static function
//
//---------------------------------------------------------------------------
#include "codegen/codegen.h"

using namespace gpcodegen;

long CodeGen::unique_counter_ = 0;

std::string CodeGen::GenerateUniqueName(const std::string& prefix)
{
	return prefix + std::to_string(unique_counter_++);
}
