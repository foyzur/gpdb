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
#ifndef CODEGEN_FUNC_INFO_H_
#define CODEGEN_FUNC_INFO_H_

#include "codegen/codegen_manager_c.h"

class CodeGeneratorManager;
class balerion::CodeGenerator;

class CodeGenFuncInfo
{
private:
	//std::string func_name_; // e.g., “slot_deform_tuple”
	std::unique_ptr<char[]> func_name_;
	void* object_;  // points to the object that contains all info necessary for generating code of the function func_name
	CodeGeneratorCallback code_generator_callback_; // points to the function that can generate the code
	void* regular_func_pointer_; // e.g., pointer to the regular (pre-compiled) slot_deform_tuple
	void** called_func_pointer_addr_; // e.g., pointer to the function pointer that can point to either codegened or regular slot_deform_tuple

public:
	bool GenerateCode(CodeGeneratorManager* manager, balerion::CodeGenerator* code_generator) {
		//typedef char* (*CodeGeneratorCallback)(void *object, void* manager, void* code_gen);
		func_name_.reset(code_generator_callback_(object_, manager, code_generator));
		return nullptr != func_name_;
	}
	bool Invalidate();
	bool UseRegular();
	bool UseGenerated();
	CodeGenFuncInfo(void* object, CodeGeneratorCallback generator,
		void* regular_func_pointer, void** called_func_pointer_addr) :
				object_(object),
				code_generator_callback_(generator),
				regular_func_pointer_(regular_func_pointer),
				called_func_pointer_addr_(called_func_pointer_addr)
				{
					func_name_.reset(nullptr);
				}

};

#endif  // CODEGEN_FUNC_INFO_H_
