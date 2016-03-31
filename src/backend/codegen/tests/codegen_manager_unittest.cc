//---------------------------------------------------------------------------
//  Greenplum Database
//  Copyright 2016 Pivotal Software, Inc.
//
//  @filename:
//    code_generator_unittest.cc
//
//  @doc:
//    Unit tests for codegen_manager.cc
//
//  @test:
//
//---------------------------------------------------------------------------

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <initializer_list>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "codegen/utils/codegen_utils.h"
#include "codegen/utils/annotated_type.h"
#include "codegen/utils/instance_method_wrappers.h"
#include "codegen/utils/utility.h"
#include "gtest/gtest.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Casting.h"

#include "codegen/codegen_wrapper.h"
#include "codegen/codegen_interface.h"
#include "codegen/slot_deform_tuple_codegen.h"

#include "gtest/gtest.h"


namespace gpcodegen {

void dummy_slot_deform_tuple(TupleTableSlot *slot, int natts) {
   return;
}

// Test environment to handle global per-process initialization tasks for all
// tests.
class CodeGenManagerTestEnvironment : public ::testing::Environment {
 public:
  virtual void SetUp() {
    ASSERT_TRUE(CodeGenUtils::InitializeGlobal());
  }
};

class CodeGenManagerTest : public ::testing::Test {
 protected:

  virtual void SetUp() {
    client_ = nullptr;
    generator_.reset(new SlotDeformTupleCodeGen(nullptr, dummy_slot_deform_tuple, &client_));
  }

  SlotDeformTupleFn client_;
  std::unique_ptr<CodeGenInterface> generator_;
};

TEST_F(CodeGenManagerTest, InitializationTest) {
  EXPECT_EQ(client_, nullptr);
  //ASSERT_NE(generator_, nullptr);
}

}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  AddGlobalTestEnvironment(new gpcodegen::CodeGenManagerTestEnvironment);
  return RUN_ALL_TESTS();
}
