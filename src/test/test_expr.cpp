#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <medusa/expression.hpp>
#include <medusa/expression_filter.hpp>
#include <medusa/expression_visitor.hpp>
#include <medusa/expression_simplifier.hpp>
#include <medusa/module.hpp>
#include <medusa/architecture.hpp>

using namespace medusa;

TEST_CASE("parsing", "[expr]")
{
  INFO("Testing expression parser");

  auto& rModMgr = ModuleManager::Instance();
  auto pX86Getter = rModMgr.LoadModule<TGetArchitecture>(".", "x86");
  REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_64_Mode = pX86Disasm->GetModeByName("64-bit");
  REQUIRE(X86_64_Mode != 0);
  auto const pCpuInfo = pX86Disasm->GetCpuInformation();

  auto TestParseExpr = [&](std::string const& rExprStr)
  {
    std::cout << "input expression:  " << rExprStr << std::endl;
    auto Res = Expression::Parse(rExprStr, *pCpuInfo, X86_64_Mode);
    CHECK(Res.size() == 1);
    if (Res.size() < 1)
      return;
    std::cout << "parsed expression: " << Res[0]->ToString() << std::endl;
    CHECK(Res[0]->ToString() == rExprStr);
  };

  TestParseExpr("(Id64(rax) = bv64(0x0000000011223344))");
  TestParseExpr("Var32[alloc] test");
  TestParseExpr("(Var32[use] test = bv32(0xDEADBEEF))");
  TestParseExpr("(Id32(eax) = Var32[use] test)");
  TestParseExpr("Var32[free] test");
  TestParseExpr("(Id64(rcx) = Sym(parm, \"blabla\", 1122334455667788, Var64[use] test))");
  TestParseExpr("(Id32(ecx) = Sym(parm, \"blabla\", aabbccdd))");
  TestParseExpr("(Mem32((Id64(rsp) + bv64(0x0000000000000008))) = Id32(ecx))");
  TestParseExpr("(Var64[use] cheeki = Mem64(bv64(0x0000000140000000)))");
  TestParseExpr("(Var64[use] cheeki = Mem64(Id16(fs):bv64(0x0000000140000000)))"); // FIXME(wisk): see below
  TestParseExpr("(Var64[use] cheeki = Addr64(bv64(0x0000000000000120)))");
  TestParseExpr("(Var64[use] cheeki = Addr64(Id16(fs):bv64(0x0000000000000120)))"); // FIXME(wisk): parsing base doesn't work
  TestParseExpr("(Var64[use] cheeki = Var64[use] breeki)");
  TestParseExpr("(Id8(al) = (bv8(0xFF) * bv8(0x00)))");
  TestParseExpr("(Id16(ax) = (bv16(0x1234) & ~(bv16(0xFF00))))");
}

TEST_CASE("const", "[expr]")
{
  INFO("Testing creation of BitVectorExpression");
  auto spConstExpr = Expr::MakeBitVector(16, 0x1234);
  CHECK(spConstExpr != nullptr);
}

TEST_CASE("x86 id normalization", "[expr]")
{
  auto& rModMgr = ModuleManager::Instance();
  auto pX86Getter = rModMgr.LoadModule<TGetArchitecture>(".", "x86");
  REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_32_Mode = pX86Disasm->GetModeByName("32-bit");
  REQUIRE(X86_32_Mode != 0);

  auto const pCpuInfo = pX86Disasm->GetCpuInformation();
  auto AH = pCpuInfo->ConvertNameToIdentifier("ah");
  auto BL = pCpuInfo->ConvertNameToIdentifier("bl");
  auto EAX = pCpuInfo->ConvertNameToIdentifier("eax");
  auto EBX = pCpuInfo->ConvertNameToIdentifier("ebx");
  auto spEaxExpr = Expr::MakeBitVector(32, 0x11223344);
  auto spEbxExpr = Expr::MakeBitVector(32, 0xffeeddcc);

  auto spExpr = Expr::MakeAssign(Expr::MakeId(AH, pCpuInfo), Expr::MakeId(BL, pCpuInfo));
  std::cout << "assign: " << spExpr->ToString() << std::endl;

  NormalizeIdentifier NrmId(*pCpuInfo, X86_32_Mode);
  spExpr = spExpr->Visit(&NrmId);
  std::cout << "assign (id normalized): " << spExpr->ToString() << std::endl;

  EvaluateVisitor EvalVst(Document(), 0x0, X86_32_Mode, false);
  EvalVst.SetId(EAX, spEaxExpr);
  EvalVst.SetId(EBX, spEbxExpr);
  spExpr->Visit(&EvalVst);
  auto spRes = expr_cast<BitVectorExpression>(EvalVst.GetResultExpression());
  REQUIRE(spRes != nullptr);
  std::cout << "res: " << spRes->ToString() << std::endl;
  CHECK(spRes->GetInt().ConvertTo<u32>() == 0x1122cc44);

  delete pX86Disasm;
}

TEST_CASE("x86-64 id normalization", "[expr]")
{
  auto& rModMgr = ModuleManager::Instance();
  auto pX86Getter = rModMgr.LoadModule<TGetArchitecture>(".", "x86");
  REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_64_Mode = pX86Disasm->GetModeByName("64-bit");
  REQUIRE(X86_64_Mode != 0);

  auto const pCpuInfo = pX86Disasm->GetCpuInformation();
  auto EAX = pCpuInfo->ConvertNameToIdentifier("eax");
  auto RAX = pCpuInfo->ConvertNameToIdentifier("rax");

  auto spExpr = Expr::MakeAssign(Expr::MakeId(EAX, pCpuInfo), Expr::MakeBitVector(32, 0x11223344));
  std::cout << "assign: " << spExpr->ToString() << std::endl;

  NormalizeIdentifier NrmId(*pCpuInfo, X86_64_Mode);
  spExpr = spExpr->Visit(&NrmId);
  std::cout << "assign (id normalized): " << spExpr->ToString() << std::endl;

  EvaluateVisitor EvalVst(Document(), 0x0, X86_64_Mode, false);
  EvalVst.SetId(RAX, Expr::MakeBitVector(64, 0xffffffffffffffff));
  spExpr->Visit(&EvalVst);
  auto spRes = expr_cast<BitVectorExpression>(EvalVst.GetResultExpression());
  REQUIRE(spRes != nullptr);
  std::cout << "res: " << spRes->ToString() << std::endl;
  CHECK(spRes->GetInt().ConvertTo<u32>() == 0x11223344);

  delete pX86Disasm;
}

TEST_CASE("x86 id normalization, track and evaluation", "[expr]")
{
  auto& rModMgr = ModuleManager::Instance();
  auto pX86Getter = rModMgr.LoadModule<TGetArchitecture>(".", "x86");
  REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_32_Mode = pX86Disasm->GetModeByName("32-bit");
  REQUIRE(X86_32_Mode != 0);

  auto const pCpuInfo = pX86Disasm->GetCpuInformation();
  auto AL = pCpuInfo->ConvertNameToIdentifier("al");
  auto BH = pCpuInfo->ConvertNameToIdentifier("bh");
  auto BX = pCpuInfo->ConvertNameToIdentifier("bx");
  auto EAX = pCpuInfo->ConvertNameToIdentifier("eax");
  auto EBX = pCpuInfo->ConvertNameToIdentifier("ebx");
  auto spAl = Expr::MakeId(AL, pCpuInfo);
  auto spBh = Expr::MakeId(BH, pCpuInfo);
  auto spBx = Expr::MakeId(BX, pCpuInfo);
  auto spEax = Expr::MakeId(EAX, pCpuInfo);

  NormalizeIdentifier NrmId(*pCpuInfo, X86_32_Mode);

  // EAX = 0x11223344
  auto spInitEax = Expr::MakeAssign(spEax, Expr::MakeBitVector(spEax->GetBitSize(), 0x11223344));
  std::cout << "init eax: " << spInitEax->ToString() << std::endl;

  spInitEax = spInitEax->Visit(&NrmId);
  std::cout << "init eax (id normalized): " << spInitEax->ToString() << std::endl;

  // BX = 0xAA55
  auto spInitBx = Expr::MakeAssign(spBx, Expr::MakeBitVector(spBx->GetBitSize(), 0xAA55));
  std::cout << "init bx: " << spInitBx->ToString() << std::endl;

  spInitBx = spInitBx->Visit(&NrmId);
  std::cout << "init bx (id normalized): " << spInitBx->ToString() << std::endl;

  // EAX = zero_extend_32(AL + BH)
  auto spOp0 = Expr::MakeBinOp(OperationExpression::OpAdd, spAl, spBh);
  auto spOp1 = Expr::MakeBinOp(OperationExpression::OpZext, spOp0, Expr::MakeBitVector(spEax->GetBitSize(), spEax->GetBitSize()));
  auto spExpr = Expr::MakeAssign(spEax, spOp1);

  std::cout << "oper: " << spExpr->ToString() << std::endl;

  auto spNrmExpr = spExpr->Visit(&NrmId);
  std::cout << "oper (id normalized): " << spNrmExpr->ToString() << std::endl;

  SymbolicVisitor SymVst(Document(), X86_32_Mode);

  SymVst.UpdateAddress(*pX86Disasm, Address(Address::LogicalType, 0x0, 0x0, 16, 32));
  auto spInitEbx = Expr::MakeAssign(
    Expr::MakeId(EBX, pX86Disasm->GetCpuInformation()),
    Expr::MakeBitVector(BitVector(32, 0x0)));
  spInitEbx->Visit(&SymVst);
  SymVst.UpdateAddress(*pX86Disasm, Address(Address::LogicalType, 0x0, 0x1, 16, 32));
  spInitEax->Visit(&SymVst);
  SymVst.UpdateAddress(*pX86Disasm, Address(Address::LogicalType, 0x0, 0x2, 16, 32));
  spInitBx->Visit(&SymVst);
  SymVst.UpdateAddress(*pX86Disasm, Address(Address::LogicalType, 0x0, 0x3, 16, 32));
  spNrmExpr->Visit(&SymVst);

  std::cout << SymVst.ToString() << std::endl;

  auto spRes = expr_cast<BitVectorExpression>(SymVst.GetValue(Expr::MakeId(EAX, pX86Disasm->GetCpuInformation())));
  REQUIRE(spRes != nullptr);
  std::cout << "res: " << spRes->ToString() << std::endl;
  REQUIRE(spRes->GetInt().ConvertTo<u32>() == 0x000000EE);

  delete pX86Disasm;
}

TEST_CASE("variable", "[expr]")
{
  auto spAllocVarExpr = Expr::MakeVar("test", VariableExpression::Alloc, 32);
  auto spAssignVarExpr = Expr::MakeAssign(Expr::MakeVar("test", VariableExpression::Use), Expr::MakeBitVector(32, 0x11223344));
  auto spFreeVarExpr = Expr::MakeVar("test", VariableExpression::Free);

  std::cout << spAllocVarExpr->ToString() << std::endl;
  std::cout << spAssignVarExpr->ToString() << std::endl;
  std::cout << spFreeVarExpr->ToString() << std::endl;
}

TEST_CASE("float", "[expr]")
{
  using namespace Pattern;

  ExpressionFilter ExprFlt(OR(Any("a"), Any("b")));
  auto spOrExpr = Expr::MakeBinOp(OperationExpression::OpOr, Expr::MakeBitVector(BitVector(32, 0x11223344)), Expr::MakeBitVector(BitVector(32, 0xaabbccdd)));
  REQUIRE(ExprFlt.Execute(spOrExpr));
  auto spExprA = expr_cast<BitVectorExpression>(ExprFlt.GetExpression("a"));
  REQUIRE(spExprA != nullptr);
  REQUIRE(spExprA->GetInt().ConvertTo<u32>() == 0x11223344);
  auto spExprB = expr_cast<BitVectorExpression>(ExprFlt.GetExpression("b"));
  REQUIRE(spExprB != nullptr);
  REQUIRE(spExprB->GetInt().ConvertTo<u32>() == 0xaabbccdd);
}

TEST_CASE("push pop", "[expr]")
{
  auto& rModMgr = ModuleManager::Instance();
  auto pX86Getter = rModMgr.LoadModule<TGetArchitecture>(".", "x86");
  REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_32_Mode = pX86Disasm->GetModeByName("32-bit");
  REQUIRE(X86_32_Mode != 0);

  auto const pCpuInfo = pX86Disasm->GetCpuInformation();
  auto SS = pCpuInfo->ConvertNameToIdentifier("ss");
  auto ESP = pCpuInfo->ConvertNameToIdentifier("esp");
  auto EAX = pCpuInfo->ConvertNameToIdentifier("eax");
  auto spSs = Expr::MakeId(SS, pCpuInfo);
  auto spEsp = Expr::MakeId(ESP, pCpuInfo);
  auto spEax = Expr::MakeId(EAX, pCpuInfo);

  NormalizeIdentifier NrmId(*pCpuInfo, X86_32_Mode);

  // PUSH byte 0xFF
  auto spAllocVar_PushedVal = Expr::MakeVar("pushed_value", VariableExpression::Alloc, 32);
  auto spWriteVar_PushedVal = Expr::MakeAssign(
    Expr::MakeVar("pushed_value", VariableExpression::Use),
    Expr::MakeBinOp(OperationExpression::OpSext, Expr::MakeBitVector(BitVector(8, 0xFF)), Expr::MakeBitVector(BitVector(32, 32))));
  auto spDecStk = Expr::MakeAssign(
    spEsp,
    Expr::MakeBinOp(OperationExpression::OpSub, spEsp, Expr::MakeBitVector(BitVector(32, 4))));
  auto spWriteStk = Expr::MakeAssign(
    Expr::MakeMem(32, spSs, spEsp),
    Expr::MakeVar("pushed_value", VariableExpression::Use));
  auto spFreeVar_PushedVal = Expr::MakeVar("pushed_value", VariableExpression::Free);

  // POP EAX
  auto spAllocVar_PoppedVal = Expr::MakeVar("popped_value", VariableExpression::Alloc, 32);
  auto spReadVar_PoppedVar = Expr::MakeAssign(
    Expr::MakeVar("popped_value", VariableExpression::Use),
    Expr::MakeMem(32, spSs, spEsp));
  auto spIncStk = Expr::MakeAssign(
    spEsp,
    Expr::MakeBinOp(OperationExpression::OpAdd, spEsp, Expr::MakeBitVector(BitVector(32, 4))));
  auto spReadStk = Expr::MakeAssign(
    spEax,
    Expr::MakeVar("popped_value", VariableExpression::Use));
  auto spFreeVar_PoppedVal = Expr::MakeVar("popped_value", VariableExpression::Free);


  Expression::VSPType PushFF = {
    spAllocVar_PushedVal, spWriteVar_PushedVal, spDecStk, spWriteStk, spFreeVar_PushedVal,
  };

  Expression::VSPType PopEax = {
    spAllocVar_PoppedVal, spReadVar_PoppedVar, spIncStk, spReadStk, spFreeVar_PoppedVal,
  };

  SymbolicVisitor SymVst(Document(), X86_32_Mode);

  SymVst.UpdateAddress(*pX86Disasm, Address(Address::LinearType, 0x0, 0x0, 16, 32));
  for (auto const& rspExpr : PushFF)
  {
    rspExpr->Visit(&SymVst);
  }

  SymVst.UpdateAddress(*pX86Disasm, Address(Address::LinearType, 0x0, 0x2, 16, 32));
  for (auto const& rspExpr : PopEax)
  {
    rspExpr->Visit(&SymVst);
  }

  // Result must be EAX = 0xFFFFFFFF
  auto spRes = expr_cast<BitVectorExpression>(SymVst.GetValue(Expr::MakeId(EAX, pX86Disasm->GetCpuInformation())));

  REQUIRE(spRes != nullptr);
  REQUIRE(spRes->GetInt().ConvertTo<u32>() == 0xFFFFFFFF);

  delete pX86Disasm;
}

TEST_CASE("x86 call register", "[expr]")
{
  using namespace medusa;

  Log::SetLog([](std::string const& rMsg)
  {
    std::cout << rMsg << std::flush;
  });

  INFO("Using samples path \"" SAMPLES_DIR "\"");

  Medusa Core;

  auto const pSample = SAMPLES_DIR "/exe/call_reg.pe.x86-32";
  std::cout << "disassembling program: " << pSample << std::endl;

  try
  {
    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
    REQUIRE(Core.NewDocument(
      spFileBinStrm, true,
      [&](Path& rDbPath, std::list<Medusa::Filter> const&)
    {
      rDbPath = "call_reg.pe.x86-32_";
      return true;
    }));
  }
  catch (Exception const& e)
  {
    std::cerr << e.What() << std::endl;
    REQUIRE(0);
  }

  Core.WaitForTasks();

  auto const& rDoc = Core.GetDocument();
  auto const& rStartAddr = rDoc.GetAddressFromLabelName("start");
  auto spStartCell = rDoc.GetCell(rStartAddr);

  auto spArch = ModuleManager::Instance().GetArchitecture(spStartCell->GetArchitectureTag());
  REQUIRE(spArch != nullptr);

  SymbolicVisitor SymVst(Core.GetDocument(), spStartCell->GetMode());

  auto SymExec = [&](Address const& rAddr)
  {
    REQUIRE(SymVst.UpdateAddress(*spArch, rAddr));
    auto spInsn = std::dynamic_pointer_cast<Instruction>(rDoc.GetCell(rAddr));
    REQUIRE(spInsn != nullptr);
    for (auto const& rspExpr : spInsn->GetSemantic())
    {
      rspExpr->Visit(&SymVst);
    }
  };

  auto pCpuInfo = spArch->GetCpuInformation();
  auto EIP = pCpuInfo->ConvertNameToIdentifier("eip");
  auto EAX = pCpuInfo->ConvertNameToIdentifier("eax");
  REQUIRE(EIP != 0);
  REQUIRE(EAX != 0);

  std::vector<Address> Addrs =
  {
    rDoc.MakeAddress(0x0000, 0x00401000),
    rDoc.MakeAddress(0x0000, 0x00401001),
    rDoc.MakeAddress(0x0000, 0x00401007),
    rDoc.MakeAddress(0x0000, 0x00401009),
    rDoc.MakeAddress(0x0000, 0x0040100E),
    rDoc.MakeAddress(0x0000, 0x00401013),
    rDoc.MakeAddress(0x0000, 0x00401015),
    rDoc.MakeAddress(0x0000, 0x00401017),
    rDoc.MakeAddress(0x0000, 0x00401019),
    rDoc.MakeAddress(0x0000, 0x0040101E),
    rDoc.MakeAddress(0x0000, 0x00401023),
    rDoc.MakeAddress(0x0000, 0x00401025),
    rDoc.MakeAddress(0x0000, 0x00401027),
    rDoc.MakeAddress(0x0000, 0x00401029),
    rDoc.MakeAddress(0x0000, 0x0040102E),
    rDoc.MakeAddress(0x0000, 0x00401033),
    rDoc.MakeAddress(0x0000, 0x00401035),
    rDoc.MakeAddress(0x0000, 0x00401037),
    rDoc.MakeAddress(0x0000, 0x00401039),
    rDoc.MakeAddress(0x0000, 0x0040103A),
  };

  for (auto const& rAddr : Addrs)
  {
    SymExec(rAddr);
    int NumOfPathFound;
    SymVst.FindAllPaths(NumOfPathFound, *spArch, [&](Expression::SPType spDstExpr, Expression::VSPType spCondExprs)
    {
      std::cout << "DST: " << spDstExpr->ToString();
      for (auto spCondExpr : spCondExprs)
        std::cout << ", EXPR: " << spCondExpr->ToString();
        std::cout << std::endl;
    });

    //auto spExpr = SymVst.FindExpression(Expr::MakeId(EIP, pCpuInfo));
    //REQUIRE(spExpr != nullptr);
    //std::cout << spExpr->ToString() << std::endl;
  }

  std::cout << SymVst.ToString() << std::endl;
}

TEST_CASE("x86 jump table", "[expr]")
{
  using namespace medusa;

  Log::SetLog([](std::string const& rMsg)
  {
    std::cout << rMsg << std::flush;
  });

  INFO("Using samples path \"" SAMPLES_DIR "\"");

  Medusa Core;

  auto const pSample = SAMPLES_DIR "/exe/jmp_tbl.pe.x86-32";
  std::cout << "disassembling program: " << pSample << std::endl;

  try
  {
    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
    REQUIRE(Core.NewDocument(
      spFileBinStrm, true,
      [&](Path& rDbPath, std::list<Medusa::Filter> const&)
    {
      rDbPath = "jmp_tbl.pe.x86-32_";
      return true;
    }));
  }
  catch (Exception const& e)
  {
    std::cerr << e.What() << std::endl;
    REQUIRE(0);
  }

  auto& rDoc = Core.GetDocument();

  auto FuncAddr = rDoc.MakeAddress(0x0000, 0x00401000);
  REQUIRE(Core.AddTask("symbolic disassemble", FuncAddr));

  Core.WaitForTasks();

  auto spFunc = rDoc.GetMultiCell(FuncAddr);
  REQUIRE(spFunc != nullptr);

  auto spFuncGraph = spFunc->GetGraph();
  REQUIRE(spFuncGraph != nullptr);

  GraphData GD;
  REQUIRE(Core.FormatGraph(*spFuncGraph, GD));
}

TEST_CASE("operator", "[expr]")
{
  auto spFlt = Expr::MakeBitVector(BitVector(1.0f));
  auto spImm = Expr::MakeBitVector(BitVector(32, 0x3f800000)); // 1.0

  auto spRes = spFlt + spImm;

  REQUIRE(spRes != nullptr);
  std::cout << spRes->ToString() << std::endl;
}