#define BOOST_TEST_MODULE TestExpression
#include <boost/test/unit_test.hpp>

#include <medusa/expression.hpp>
#include <medusa/expression_filter.hpp>
#include <medusa/expression_visitor.hpp>
#include <medusa/expression_simplifier.hpp>
#include <medusa/module.hpp>
#include <medusa/architecture.hpp>

using namespace medusa;

BOOST_AUTO_TEST_SUITE(expression_test_suite)

BOOST_AUTO_TEST_CASE(expr_const_test_case)
{
  BOOST_MESSAGE("Testing creation of BitVectorExpression");
  auto spConstExpr = Expr::MakeBitVector(16, 0x1234);
  BOOST_CHECK(spConstExpr != nullptr);
}

BOOST_AUTO_TEST_CASE(expr_x86_id_nrm_eval)
{
  auto& rModMgr = ModuleManager::Instance();
  auto pX86Getter = rModMgr.LoadModule<TGetArchitecture>(".", "x86");
  BOOST_REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_32_Mode = pX86Disasm->GetModeByName("32-bit");
  BOOST_REQUIRE(X86_32_Mode != 0);

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
  BOOST_REQUIRE(spRes != nullptr);
  std::cout << "res: " << spRes->ToString() << std::endl;
  BOOST_CHECK(spRes->GetInt().ConvertTo<u32>() == 0x1122cc44);

  delete pX86Disasm;
}

BOOST_AUTO_TEST_CASE(expr_x86_64_id_nrm_eval)
{
  auto& rModMgr = ModuleManager::Instance();
  auto pX86Getter = rModMgr.LoadModule<TGetArchitecture>(".", "x86");
  BOOST_REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_64_Mode = pX86Disasm->GetModeByName("64-bit");
  BOOST_REQUIRE(X86_64_Mode != 0);

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
  BOOST_REQUIRE(spRes != nullptr);
  std::cout << "res: " << spRes->ToString() << std::endl;
  BOOST_CHECK(spRes->GetInt().ConvertTo<u32>() == 0x11223344);

  delete pX86Disasm;
}

BOOST_AUTO_TEST_CASE(expr_x86_id_nrm_trk_eval)
{
  auto& rModMgr = ModuleManager::Instance();
  auto pX86Getter = rModMgr.LoadModule<TGetArchitecture>(".", "x86");
  BOOST_REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_32_Mode = pX86Disasm->GetModeByName("32-bit");
  BOOST_REQUIRE(X86_32_Mode != 0);

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

  auto spRes = expr_cast<BitVectorExpression>(SymVst.FindExpression(Expr::MakeId(EAX, pX86Disasm->GetCpuInformation())));
  BOOST_REQUIRE(spRes != nullptr);
  std::cout << "res: " << spRes->ToString() << std::endl;
  BOOST_REQUIRE(spRes->GetInt().ConvertTo<u32>() == 0x000000EE);

  delete pX86Disasm;
}

BOOST_AUTO_TEST_CASE(expr_var)
{
  auto spAllocVarExpr = Expr::MakeVar("test", VariableExpression::Alloc, 32);
  auto spAssignVarExpr = Expr::MakeAssign(Expr::MakeVar("test", VariableExpression::Use), Expr::MakeBitVector(32, 0x11223344));
  auto spFreeVarExpr = Expr::MakeVar("test", VariableExpression::Free);

  std::cout << spAllocVarExpr->ToString() << std::endl;
  std::cout << spAssignVarExpr->ToString() << std::endl;
  std::cout << spFreeVarExpr->ToString() << std::endl;
}

BOOST_AUTO_TEST_CASE(expr_flt)
{
  using namespace Pattern;

  ExpressionFilter ExprFlt(OR(Any("a"), Any("b")));
  auto spOrExpr = Expr::MakeBinOp(OperationExpression::OpOr, Expr::MakeBitVector(BitVector(32, 0x11223344)), Expr::MakeBitVector(BitVector(32, 0xaabbccdd)));
  BOOST_REQUIRE(ExprFlt.Execute(spOrExpr));
  auto spExprA = expr_cast<BitVectorExpression>(ExprFlt.GetExpression("a"));
  BOOST_REQUIRE(spExprA != nullptr);
  BOOST_REQUIRE(spExprA->GetInt().ConvertTo<u32>() == 0x11223344);
  auto spExprB = expr_cast<BitVectorExpression>(ExprFlt.GetExpression("b"));
  BOOST_REQUIRE(spExprB != nullptr);
  BOOST_REQUIRE(spExprB->GetInt().ConvertTo<u32>() == 0xaabbccdd);
}

BOOST_AUTO_TEST_CASE(expr_push_pop)
{
  auto& rModMgr = ModuleManager::Instance();
  auto pX86Getter = rModMgr.LoadModule<TGetArchitecture>(".", "x86");
  BOOST_REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_32_Mode = pX86Disasm->GetModeByName("32-bit");
  BOOST_REQUIRE(X86_32_Mode != 0);

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

  SymVst.UpdateAddress(*pX86Disasm, Address(Address::FlatType, 0x0, 0x0, 16, 32));
  for (auto const& rspExpr : PushFF)
  {
    rspExpr->Visit(&SymVst);
  }

  SymVst.UpdateAddress(*pX86Disasm, Address(Address::FlatType, 0x0, 0x2, 16, 32));
  for (auto const& rspExpr : PopEax)
  {
    rspExpr->Visit(&SymVst);
  }

  // Result must be EAX = 0xFFFFFFFF
  auto spRes = expr_cast<BitVectorExpression>(SymVst.FindExpression(Expr::MakeId(EAX, pX86Disasm->GetCpuInformation())));

  BOOST_REQUIRE(spRes != nullptr);
  BOOST_REQUIRE(spRes->GetInt().ConvertTo<u32>() == 0xFFFFFFFF);

  delete pX86Disasm;
}

BOOST_AUTO_TEST_CASE(expr_x86_jmp_tbl)
{
  using namespace medusa;

  Log::SetLog([](std::string const& rMsg)
  {
    std::cout << rMsg << std::flush;
  });

  BOOST_TEST_MESSAGE("Using samples path \"" SAMPLES_DIR "\"");

  Medusa Core;

  auto const pSample = SAMPLES_DIR "/exe/jmp_tbl.pe.x86-32";
  std::cout << "disassembling program: " << pSample << std::endl;

  try
  {
    auto spFileBinStrm = std::make_shared<FileBinaryStream>(pSample);
    BOOST_REQUIRE(Core.NewDocument(
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
    BOOST_REQUIRE(0);
  }

  Core.WaitForTasks();

  auto const& rDoc = Core.GetDocument();
  auto const& rStartAddr = rDoc.GetAddressFromLabelName("start");
  auto spStartCell = rDoc.GetCell(rStartAddr);

  auto spArch = ModuleManager::Instance().GetArchitecture(spStartCell->GetArchitectureTag());
  BOOST_REQUIRE(spArch != nullptr);

  SymbolicVisitor SymVst(Core.GetDocument(), spStartCell->GetMode());

  auto SymExec = [&](Address const& rAddr)
  {
    BOOST_REQUIRE(SymVst.UpdateAddress(*spArch, rAddr));
    auto spInsn = std::dynamic_pointer_cast<Instruction>(Core.GetCell(rAddr));
    BOOST_REQUIRE(spInsn != nullptr);
    for (auto const& rspExpr : spInsn->GetSemantic())
    {
      rspExpr->Visit(&SymVst);
    }
  };

  SymExec(rDoc.MakeAddress(0x0000, 0x00401006));
  SymExec(rDoc.MakeAddress(0x0000, 0x00401009));
  SymExec(rDoc.MakeAddress(0x0000, 0x0040100c));
  std::cout << SymVst.ToString() << std::endl;

  auto pCpuInfo = spArch->GetCpuInformation();
  auto EIP = pCpuInfo->ConvertNameToIdentifier("eip");
  BOOST_REQUIRE(EIP != 0);
  auto spExpr = SymVst.FindExpression(Expr::MakeId(EIP, pCpuInfo));
  BOOST_REQUIRE(spExpr != nullptr);
  std::cout << spExpr->ToString() << std::endl;

  auto spTernExpr = expr_cast<TernaryConditionExpression>(spExpr);
  BOOST_REQUIRE(spTernExpr != nullptr);

  using namespace Pattern;

  ExpressionFilter ExprFlt_CmpA(Ternary("expr", EQ((
    OR(
    /**/BCAST(
    /****/LRS(
    /******/XOR(
    /********/XOR(
    /**********/XOR(
    /************/Any("op0"),
    /************/Any("op1")),
    /**********/Any("res")),
    /********/AND(
    /**********/XOR(
    /************/Any("op0"),
    /************/Any("res")),
    /**********/XOR(
    /************/Any("op0"),
    /************/Any("op1")))),
    /******/Any("shift_value")),
    /****/Any("cast_bitsize")),
    /**/Ternary("zf",
    /****/EQ(Any("zf_op0"), Any("zero")),
    /****/Any("zf_true"), Any("zf_false")))),

    Any("int1_0")), Any("true"), Any("false")));
  BOOST_REQUIRE(ExprFlt_CmpA.Execute(spTernExpr));

  auto expr   = ExprFlt_CmpA.GetExpression("expr");
  auto op0    = ExprFlt_CmpA.GetExpression("op0");
  auto op1    = ExprFlt_CmpA.GetExpression("op1");
  auto res    = ExprFlt_CmpA.GetExpression("res");
  auto _true  = ExprFlt_CmpA.GetExpression("true");
  auto _false = ExprFlt_CmpA.GetExpression("false");

  std::cout
    << "expr:  " << expr->ToString() << std::endl
    << "op0:   " << op0->ToString() << std::endl
    << "op1:   " << op1->ToString() << std::endl
    << "res:   " << res->ToString() << std::endl
    << "true:  " << _true->ToString() << std::endl
    << "false: " << _false->ToString() << std::endl
    ;

  ExpressionRewriter ExprRwr(spExpr);
  BOOST_REQUIRE(ExprRwr.Execute());

  std::cout << spExpr->ToString() << std::endl;

  std::cout << std::string(80, '#') << std::endl;

  SymVst.UpdateExpression(Expr::MakeId(EIP, pCpuInfo), [&](Expression::SPType& rspExpr)
  {
    ExpressionRewriter ER(rspExpr);
    ER.Execute();
    return true;
  });

  std::cout << SymVst.ToString() << std::endl;
  std::cout << std::string(80, '#') << std::endl;

  SymExec(rDoc.MakeAddress(0x0000, 0x00401012));
  std::cout << SymVst.ToString() << std::endl;
}

//BOOST_AUTO_TEST_CASE(expr_tostr)
//{
//  Expression::SPType spExpr;
//  Expression::SPType spLastExpr;
//  for (auto i = 0UL; i < 0x11; ++i)
//  {
//    if (spLastExpr != nullptr)
//      spExpr = Expr::MakeBinOp(OperationExpression::OpAdd, spExpr, spLastExpr);
//    else
//      spExpr = Expr::MakeBinOp(OperationExpression::OpAdd, Expr::MakeBitVector(BitVector(0x8, 0x00)), Expr::MakeBitVector(BitVector(0x8, 0x00)));
//    spLastExpr = spExpr;
//  }
//  std::cout << spExpr->ToString() << std::endl;
//}

BOOST_AUTO_TEST_SUITE_END()