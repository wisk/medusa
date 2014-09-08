#define BOOST_TEST_MODULE TestExpression
#include <boost/test/unit_test.hpp> 

#include <medusa/expression.hpp>

BOOST_AUTO_TEST_SUITE(expression_test_suite)

BOOST_AUTO_TEST_CASE(expr_const_test_case)
{
  BOOST_MESSAGE("Testing creation of ConstantExpression");
  auto pConstExpr = medusa::Expr::MakeConst(16, 0x1234);
  BOOST_CHECK(pConstExpr != nullptr);
}

BOOST_AUTO_TEST_SUITE_END()