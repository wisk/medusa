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

BOOST_AUTO_TEST_CASE(expr_x86_id_nrm)
{
  auto& rModMgr = ModuleManager::Instance();
  auto pX86Getter = rModMgr.LoadModule<TGetArchitecture>(".", "x86");
  BOOST_REQUIRE(pX86Getter != nullptr);
  auto pX86Disasm = pX86Getter();

  auto const X86_16_Mode = pX86Disasm->GetModeByName("16-bit");
  auto const X86_32_Mode = pX86Disasm->GetModeByName("32-bit");
  auto const X86_64_Mode = pX86Disasm->GetModeByName("64-bit");
  BOOST_REQUIRE(X86_16_Mode != 0);
  BOOST_REQUIRE(X86_32_Mode != 0);
  BOOST_REQUIRE(X86_64_Mode != 0);

  auto const pCpuInfo = pX86Disasm->GetCpuInformation();
  auto AL = pCpuInfo->ConvertNameToIdentifier("al");
  auto BH = pCpuInfo->ConvertNameToIdentifier("bh");
  auto EAX = pCpuInfo->ConvertNameToIdentifier("eax");
  auto spAl = Expr::MakeId(AL, pCpuInfo);
  auto spBh = Expr::MakeId(BH, pCpuInfo);
  auto spEax = Expr::MakeId(EAX, pCpuInfo);

  auto spProlog = Expr::MakeAssign(spEax, Expr::MakeConst(spEax->GetSizeInBit(), 0x11223344));

  auto spOp0 = Expr::MakeBinOp(OperationExpression::OpAdd, spAl, spBh);
  auto spOp1 = Expr::MakeBinOp(OperationExpression::OpZext, spOp0, Expr::MakeConst(spEax->GetSizeInBit(), spEax->GetSizeInBit()));
  auto spExpr = Expr::MakeAssign(spEax, spOp1);

  std::cout << spExpr->ToString() << std::endl;

  NormalizeIdentifier NrmId(*pCpuInfo, X86_32_Mode);
  auto spNrmExpr = spExpr->Visit(&NrmId);
  std::cout << spNrmExpr->ToString() << std::endl;

  Track::Context TrkCtxt;
  TrackVisitor TrkVst0(0x10000, TrkCtxt);
  auto spTrkProlog = spProlog->Visit(&TrkVst0);
  TrackVisitor TrkVst1(0x10002, TrkCtxt);
  auto spTrkNrm = spNrmExpr->Visit(&TrkVst1);

  std::cout << spTrkProlog->ToString() << std::endl;
  std::cout << spTrkNrm->ToString() << std::endl;

  TrackedIdMerger TIM(spTrkNrm, Expression::VSPType{ spTrkProlog });
  BOOST_REQUIRE(TIM.Execute());

  std::cout << spTrkNrm->ToString() << std::endl;

  delete pX86Disasm;
}

BOOST_AUTO_TEST_SUITE_END()