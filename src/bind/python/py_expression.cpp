#include "py_exception.hpp"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <medusa/expression.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

// get size in bits of a constant expression
char const* GetExprIdIdentifier(IdentifierExpression *e)
{
  return e->GetCpuInformation()->ConvertIdentifierToName(e->GetId());
}

void PydusaExpression(void)
{

  // base expression class

  bp::class_<Expression, boost::noncopyable>("Expression", bp::no_init)
    .def("__str__", &Expression::ToString)
    .add_property("size", &Expression::GetSizeInBit)
    ;
  bp::register_ptr_to_python<Expression::SPType>();
  bp::class_<Expression::VSPType>("ExprVec")
    .def(bp::vector_indexing_suite<Expression::VSPType>())
    ;

  // constant expression class inherited from base expression class

  bp::class_<ConstantExpression, bp::bases<Expression>>("ConstantExpression", bp::no_init)
    .def("__str__", &ConstantExpression::ToString)
    .add_property("value", &ConstantExpression::GetConstant)
    ;
  bp::register_ptr_to_python<ConstantExpression::SPType>();

  // identifier expression class inherited from base expression class

  bp::class_<IdentifierExpression, bp::bases<Expression>>("IdentifierExpression", bp::no_init)
    .def("__str__", &IdentifierExpression::ToString)
    .add_property("id", &GetExprIdIdentifier)
    ;
  bp::register_ptr_to_python<IdentifierExpression::SPType>();

  // assignment expression class inherited from base expression class

  bp::class_<AssignmentExpression, bp::bases<Expression>>("AssignmentExpression", bp::no_init)
    .def("__str__", &AssignmentExpression::ToString)
    .add_property("dst", &AssignmentExpression::GetDestinationExpression)
    .add_property("src", &AssignmentExpression::GetSourceExpression)
    ;
  bp::register_ptr_to_python<AssignmentExpression::SPType>();

  // memory expression class inherited from base expression class

  bp::class_<MemoryExpression, bp::bases<Expression>>("MemoryExpression", bp::no_init)
    .def("__str__", &MemoryExpression::ToString)
    .add_property("expr", &MemoryExpression::GetAddressExpression)
    ;
  bp::register_ptr_to_python<MemoryExpression::SPType>();

  // operation expression class inherited from base expression class

  bp::class_<OperationExpression, boost::noncopyable, bp::bases<Expression>>("OperationExpression", bp::no_init)
    .def("__str__", &OperationExpression::ToString)
    .add_property("op", &OperationExpression::GetOperation)
    ;
  bp::register_ptr_to_python<OperationExpression::SPType>();

  // UnaryOperationExpression expression class inherited from operation expression class

  bp::class_<UnaryOperationExpression, bp::bases<OperationExpression>>("UnaryOperationExpression", bp::no_init)
    .def("__str__", &UnaryOperationExpression::ToString)
    ;
  bp::register_ptr_to_python<UnaryOperationExpression::SPType>();

  // BinaryOperationExpression expression class inherited from operation expression class

  bp::class_<BinaryOperationExpression, bp::bases<OperationExpression>>("BinaryOperationExpression", bp::no_init)
    .def("__str__", &BinaryOperationExpression::ToString)
    ;
  bp::register_ptr_to_python<BinaryOperationExpression::SPType>();
  
  // helpers used to create instance of a specific expression type

  bp::def("expr_make_const", Expr::MakeConst);
  bp::def("expr_make_id", Expr::MakeId);

}