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

#include "codegen/utils/codegen_utils.h"
#include "codegen/utils/utility.h"
#include "codegen/codegen_manager.h"
#include "codegen/codegen_wrapper.h"
#include "codegen/codegen_interface.h"
#include "codegen/base_codegen.h"


namespace gpcodegen {

typedef int (*SumFunc) (int x, int y);
typedef void (*DummyFn)(int x);

int SumFuncRegular(int x, int y) {
  return x + y;
}

void DummyFnRegular(int x) {
  return;
}

SumFunc func_ptr = nullptr;
SumFunc failed_func_ptr = nullptr;
DummyFn uncompiled_func_ptr = nullptr;

class SumCodeGenerator : public BaseCodeGen<SumFunc> {
  public:
  explicit SumCodeGenerator(SumFunc regular_func_ptr,
                             SumFunc* ptr_to_regular_func_ptr) :
                             BaseCodeGen(kAddFuncNamePrefix,
                                         regular_func_ptr,
                                         ptr_to_regular_func_ptr) {

  }

  virtual ~SumCodeGenerator() = default;

 protected:
  virtual bool DoCodeGeneration(gpcodegen::CodeGenUtils* codegen_utils)
  override final {
    llvm::Function* add2_func
       = codegen_utils->CreateFunctionTypeDef<SumFunc>(GetUniqueFuncName());
    llvm::BasicBlock* add2_body = codegen_utils->CreateBasicBlock("body",
                                                                   add2_func);
    codegen_utils->ir_builder()->SetInsertPoint(add2_body);
    llvm::Value* add2_sum = codegen_utils->ir_builder()->CreateAdd(
       ArgumentByPosition(add2_func, 0),
       ArgumentByPosition(add2_func, 1));
    codegen_utils->ir_builder()->CreateRet(add2_sum);

    return true;
  }
  private:
  static constexpr char kAddFuncNamePrefix[] = "SumFunc";
};

class FailingCodeGenerator : public BaseCodeGen<SumFunc> {
 public:
   explicit FailingCodeGenerator(SumFunc regular_func_ptr,
                               SumFunc* ptr_to_regular_func_ptr):
                               BaseCodeGen(kFailingFuncNamePrefix,
                                           regular_func_ptr,
                                           ptr_to_regular_func_ptr)
   {

   }

   virtual ~FailingCodeGenerator() = default;

 protected:
   virtual bool DoCodeGeneration(gpcodegen::CodeGenUtils* codegen_utils)
       override final
       {
         return false;
       }

 private:

   static constexpr char kFailingFuncNamePrefix[] = "SumFuncFailing";
};

class UncompilableCodeGenerator : public BaseCodeGen<DummyFn> {
 public:
   explicit UncompilableCodeGenerator(DummyFn regular_func_ptr,
                                      DummyFn* ptr_to_regular_func_ptr) :
                             BaseCodeGen(kUncompilableFuncNamePrefix,
                                         regular_func_ptr,
                                         ptr_to_regular_func_ptr)
   {

   }

   virtual ~UncompilableCodeGenerator() = default;

 protected:
   virtual bool DoCodeGeneration(gpcodegen::CodeGenUtils* codegen_utils)
       override final {
     llvm::Function* dummy_func
                = codegen_utils->CreateFunctionTypeDef<DummyFn>(GetUniqueFuncName());
     llvm::BasicBlock* dummy_func_body = codegen_utils->CreateBasicBlock("body",
                                                                  dummy_func);
     codegen_utils->ir_builder()->SetInsertPoint(dummy_func_body);
     llvm::Value* int_value = codegen_utils->GetConstant(4);
     codegen_utils->ir_builder()->CreateRet(int_value);
     return true;

   }

 private:

   static constexpr char kUncompilableFuncNamePrefix[] = "UncompilableFunc";
};

constexpr char SumCodeGenerator::kAddFuncNamePrefix[];
constexpr char FailingCodeGenerator::kFailingFuncNamePrefix[];
constexpr char UncompilableCodeGenerator::kUncompilableFuncNamePrefix[];

// Test environment to handle global per-process initialization tasks for all
// tests.
class CodeGenManagerTestEnvironment : public ::testing::Environment {
 public:
  virtual void SetUp() {
    ASSERT_TRUE(InitCodeGen() == 0);
  }
};

class CodeGenManagerTest : public ::testing::Test {
 protected:

  virtual void SetUp() {
    manager_.reset(new CodeGenManager());
  }

  template <typename ClassType, typename FuncType>
  void EnrollCodeGen(FuncType reg_func, FuncType* ptr_to_chosen_func) {
    ClassType* code_gen = new ClassType(reg_func, ptr_to_chosen_func);
    ASSERT_TRUE(reg_func == *ptr_to_chosen_func);
    ASSERT_TRUE(manager_->EnrollCodeGenerator(CodeGenFuncLifespan_Parameter_Invariant,
                                              code_gen));
  }

  std::unique_ptr<CodeGenManager> manager_;
};

TEST_F(CodeGenManagerTest, EnrollCodeGeneratorTest) {
  func_ptr = nullptr;
  EnrollCodeGen<SumCodeGenerator, SumFunc>(SumFuncRegular, &func_ptr);
  EXPECT_EQ(1, manager_->GetEnrollmentCount());
}

TEST_F(CodeGenManagerTest, EnrollCodeGeneratorExceptionTest) {
  SumFunc* local_func_ptr = new SumFunc();
  EnrollCodeGen<SumCodeGenerator, SumFunc>(SumFuncRegular, local_func_ptr);
  //EnrollCodeGen<SumCodeGenerator, SumFunc>(SumFuncRegular, local_func_ptr);
  //EnrollCodeGen<SumCodeGenerator, SumFunc>(SumFuncRegular, local_func_ptr);
  //EXPECT_EQ(1, manager_->GetEnrollmentCount());
  delete local_func_ptr;
  local_func_ptr = nullptr;
  manager_.reset(nullptr);
}

TEST_F(CodeGenManagerTest, FailMeSrsly) {
  SumFunc* local_func_ptr = new SumFunc();
  //std::unique_ptr<SumFunc> up(local_func_ptr);

  delete local_func_ptr;
  auto test = *local_func_ptr;
  //up.reset(nullptr);
}

TEST_F(CodeGenManagerTest, GenerateCodeTest) {

  // With no generator it should return false
  ASSERT_FALSE(manager_->GenerateCode());

  // Test if generation happens successfully
  func_ptr = nullptr;
  EnrollCodeGen<SumCodeGenerator, SumFunc>(SumFuncRegular, &func_ptr);
  EXPECT_EQ(1, manager_->GenerateCode());

  // Test if generation fails with FailingCodeGenerator
  failed_func_ptr = nullptr;
  EnrollCodeGen<FailingCodeGenerator, SumFunc>(SumFuncRegular, &failed_func_ptr);
  EXPECT_EQ(1, manager_->GenerateCode());

  // Test if generation pass with UncompiledCodeGenerator
  uncompiled_func_ptr = nullptr;
  EnrollCodeGen<UncompilableCodeGenerator, DummyFn>(DummyFnRegular, &uncompiled_func_ptr);
  EXPECT_EQ(2, manager_->GenerateCode());
}

} //namespace gpcodegen

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  AddGlobalTestEnvironment(new gpcodegen::CodeGenManagerTestEnvironment);
  return RUN_ALL_TESTS();
}
