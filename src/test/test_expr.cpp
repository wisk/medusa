#define BOOST_TEST_MODULE TestExpression
#include <boost/test/unit_test.hpp> 

#include <medusa/expression.hpp>
#include <medusa/expression_visitor.hpp>
#include <medusa/expression_simplifier.hpp>
#include <medusa/module.hpp>
#include <medusa/architecture.hpp>

using namespace medusa;

BOOST_AUTO_TEST_SUITE(expression_test_suite)

BOOST_AUTO_TEST_CASE(expr_const_test_case)
{
  BOOST_MESSAGE("Testing creation of ConstantExpression");
  auto spConstExpr = Expr::MakeConst(16, 0x1234);
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
  auto spEaxExpr = Expr::MakeConst(32, 0x11223344);
  auto spEbxExpr = Expr::MakeConst(32, 0xffeeddcc);

  auto spExpr = Expr::MakeAssign(Expr::MakeId(AH, pCpuInfo), Expr::MakeId(BL, pCpuInfo));
  std::cout << "assign: " << spExpr->ToString() << std::endl;

  NormalizeIdentifier NrmId(*pCpuInfo, X86_32_Mode);
  spExpr = spExpr->Visit(&NrmId);
  std::cout << "assign (id normalized): " << spExpr->ToString() << std::endl;

  EvaluateVisitor EvalVst(Document(), 0x0, X86_32_Mode, false);
  EvalVst.SetId(EAX, spEaxExpr);
  EvalVst.SetId(EBX, spEbxExpr);
  spExpr->Visit(&EvalVst);
  auto spRes = expr_cast<ConstantExpression>(EvalVst.GetResultExpression());
  BOOST_REQUIRE(spRes != nullptr);
  std::cout << "res: " << spRes->ToString() << std::endl;
  BOOST_CHECK(spRes->GetConstant() == 0x1122cc44);

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

  auto spExpr = Expr::MakeAssign(Expr::MakeId(EAX, pCpuInfo), Expr::MakeConst(32, 0x11223344));
  std::cout << "assign: " << spExpr->ToString() << std::endl;

  NormalizeIdentifier NrmId(*pCpuInfo, X86_64_Mode);
  spExpr = spExpr->Visit(&NrmId);
  std::cout << "assign (id normalized): " << spExpr->ToString() << std::endl;

  EvaluateVisitor EvalVst(Document(), 0x0, X86_64_Mode, false);
  EvalVst.SetId(RAX, Expr::MakeConst(64, 0xffffffffffffffff));
  spExpr->Visit(&EvalVst);
  auto spRes = expr_cast<ConstantExpression>(EvalVst.GetResultExpression());
  BOOST_REQUIRE(spRes != nullptr);
  std::cout << "res: " << spRes->ToString() << std::endl;
  BOOST_CHECK(spRes->GetConstant() == 0x11223344);

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
  auto spInitEax = Expr::MakeAssign(spEax, Expr::MakeConst(spEax->GetSizeInBit(), 0x11223344));
  std::cout << "init eax: " << spInitEax->ToString() << std::endl;

  spInitEax = spInitEax->Visit(&NrmId);
  std::cout << "init eax (id normalized): " << spInitEax->ToString() << std::endl;

  // BX = 0xAA55
  auto spInitBx = Expr::MakeAssign(spBx, Expr::MakeConst(spBx->GetSizeInBit(), 0xAA55));
  std::cout << "init bx: " << spInitBx->ToString() << std::endl;

  spInitBx = spInitBx->Visit(&NrmId);
  std::cout << "init bx (id normalized): " << spInitBx->ToString() << std::endl;

  // EAX = zero_extend_32(AL + BH)
  auto spOp0 = Expr::MakeBinOp(OperationExpression::OpAdd, spAl, spBh);
  auto spOp1 = Expr::MakeBinOp(OperationExpression::OpZext, spOp0, Expr::MakeConst(spEax->GetSizeInBit(), spEax->GetSizeInBit()));
  auto spExpr = Expr::MakeAssign(spEax, spOp1);

  std::cout << "oper: " << spExpr->ToString() << std::endl;

  auto spNrmExpr = spExpr->Visit(&NrmId);
  std::cout << "oper (id normalized): " << spNrmExpr->ToString() << std::endl;

  Track::Context TrkCtxt;
  TrackVisitor TrkVst0(0x10000, TrkCtxt);
  auto spTrkInitEax = spInitEax->Visit(&TrkVst0);
  TrackVisitor TrkVst1(0x10001, TrkCtxt);
  auto spTrkInitBx = spInitBx->Visit(&TrkVst1);
  TrackVisitor TrkVst2(0x10002, TrkCtxt);
  auto spTrkNrm = spNrmExpr->Visit(&TrkVst2);

  std::cout << spTrkInitEax->ToString() << std::endl;
  std::cout << spTrkInitBx->ToString() << std::endl;
  std::cout << spTrkNrm->ToString() << std::endl;

  TrackedIdMerger TIM(spTrkNrm, Expression::VSPType{ spTrkInitEax, spTrkInitBx });
  BOOST_REQUIRE(TIM.Execute());

  std::cout << spTrkNrm->ToString() << std::endl;

  EvaluateVisitor EvalVst(Document(), 0x0, X86_32_Mode, false);
  EvalVst.SetId(EBX, Expr::MakeConst(32, 0xaabbccdd));
  spTrkNrm->Visit(&EvalVst);
  auto spRes = EvalVst.GetResultExpression();
  BOOST_REQUIRE(spRes);
  std::cout << "res: " << spRes->ToString() << std::endl;

  delete pX86Disasm;
}

BOOST_AUTO_TEST_SUITE_END()