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
#include "codegen/codegen_interface.h"

using namespace gpcodegen;

long CodeGenInterface::unique_counter_ = 0;

std::string CodeGenInterface::GenerateUniqueName(const std::string& prefix)
{
	return prefix + std::to_string(unique_counter_++);
}
