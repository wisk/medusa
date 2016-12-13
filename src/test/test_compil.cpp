#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <medusa/expression.hpp>
#include <medusa/module.hpp>
#include <medusa/compilation.hpp>

TEST_CASE("compilation", "[compil_llvm]")
{
  using namespace medusa;

  auto& rModMgr = ModuleManager::Instance();
  auto pCompilLlvmGetter = rModMgr.LoadModule<TGetCompiler>(".", "llvm");
  REQUIRE(pCompilLlvmGetter != nullptr);
  auto pCompilLlvm = pCompilLlvmGetter();
  REQUIRE(pCompilLlvm);

  auto BitSize = 0x20ul;

  auto spAllocVarP0  = Expr::MakeVar("parm0", VariableExpression::Alloc, BitSize);
  auto spAllocVarP1  = Expr::MakeVar("parm1", VariableExpression::Alloc, BitSize);
  auto spAllocVarRes = Expr::MakeVar("res", VariableExpression::Alloc, BitSize);

  auto _spVarP0  = Expr::MakeVar("parm0", VariableExpression::Use, BitSize);
  auto _spVarP1  = Expr::MakeVar("parm1", VariableExpression::Use, BitSize);
  auto _spVarRes = Expr::MakeVar("res", VariableExpression::Use, BitSize);

  auto _spBv_0x100 = Expr::MakeBitVector(BitSize, 0x100);
  auto _spBv_0x200 = Expr::MakeBitVector(BitSize, 0x200);
  auto _spBv_0x300 = Expr::MakeBitVector(BitSize, 0x300);

  auto spInitP0 = Expr::MakeAssign(Expr::MakeVar("parm0", VariableExpression::Use, BitSize), _spBv_0x100);
  auto spInitP1 = Expr::MakeAssign(Expr::MakeVar("parm1", VariableExpression::Use, BitSize), _spBv_0x200);

  auto _spAdd = Expr::MakeBinOp(OperationExpression::OpAdd, Expr::MakeBitVector(BitSize, 0x11), Expr::MakeBitVector(BitSize, 0x22));
  auto _spMul = Expr::MakeBinOp(OperationExpression::OpMul, _spVarP0, _spVarP1);
  auto _spDiv = Expr::MakeBinOp(OperationExpression::OpFDiv, _spMul, _spBv_0x300);
  auto _spBsf = Expr::MakeUnOp(OperationExpression::OpBsf, _spDiv);
  auto _spLastVal = _spBsf;

  auto spResVal = Expr::MakeAssign(_spVarRes, _spLastVal);

  auto spFreeVarP0  = Expr::MakeVar("parm0", VariableExpression::Free, BitSize);
  auto spFreeVarP1  = Expr::MakeVar("parm1", VariableExpression::Free, BitSize);
  auto spFreeVarRes = Expr::MakeVar("res", VariableExpression::Free, BitSize);

  auto spWriteMem = Expr::MakeAssign(Expr::MakeMem(0x20, nullptr, Expr::MakeBitVector(0x40, 0x11223344)), Expr::MakeBitVector(0x20, 0xdeadbeef));

  REQUIRE(pCompilLlvm->AddCode("start", Expression::VSPType{
    spAllocVarP0, spAllocVarP1, spAllocVarRes,
    spInitP0, spInitP1,
    spResVal,
    spFreeVarP0, spFreeVarP1, spFreeVarRes,
    spWriteMem,
  }));

  std::vector<u8> CompiledData;
  REQUIRE(pCompilLlvm->Compile(CompiledData));
}