//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.
//
//	@filename:
//		slot_deform_tuple_codegen.h
//
//	@doc:
//		Headers for slot_deform_tuple codegen
//
//---------------------------------------------------------------------------

#ifndef SLOT_DEFORM_TUPLE_CODEGEN_H_
#define SLOT_DEFORM_TUPLE_CODEGEN_H_

#include "codegen/codegen.h"
#include "codegen/codegen_wrapper.h"


namespace code_gen
{
class SlotDeformTupleCodeGen: public BasicCodeGen <SlotDeformTupleFn> {
public:
	explicit SlotDeformTupleCodeGen(TupleTableSlot* slot,
			SlotDeformTupleFn regular_func_ptr,
			SlotDeformTupleFn* ptr_to_regular_func_ptr);

	virtual ~SlotDeformTupleCodeGen() = default;

	virtual bool DoCodeGeneration(CodeGeneratorManager* manager,
			gpcodegen::CodeGenerator* code_generator) override final;

	virtual const char* GetFunctionPrefix() const override final {
	  return kSlotDeformTupleNamePrefix;
	}

private:
	static constexpr char kSlotDeformTupleNamePrefix[] = "slot_deform_tuple";
	TupleTableSlot* slot_;
};

}
#endif // SLOT_DEFORM_TUPLE_CODEGEN_H_
