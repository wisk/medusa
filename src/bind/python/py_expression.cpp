#include "py_exception.hpp"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <medusa/expression.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

// get string representation of an identifier expression
char const* GetExprIdIdentifier(IdentifierExpression *e)
{
  return e->GetCpuInformation()->ConvertIdentifierToName(e->GetId());
}

// Type of operation expression: get enumeration name from value
OperationExpression::Type expr_op_get_name(OperationExpression::Type t) { return t; }

void PydusaExpression(void)
{

  // base expression class

  bp::class_<Expression, boost::noncopyable>("Expression", bp::no_init)
    .def("__str__", &Expression::ToString)
    .add_property("size", &Expression::GetBitSize)
    ;
  bp::register_ptr_to_python<Expression::SPType>();
  bp::class_<Expression::VSPType>("ExprVec")
    .def(bp::vector_indexing_suite<Expression::VSPType>())
    ;

  // constant expression class inherited from base expression class

  bp::class_<IntegerExpression, bp::bases<Expression>>("IntegerExpression", bp::no_init)
    .def("__str__", &IntegerExpression::ToString)
    .add_property("value", &IntegerExpression::GetConstant)
    ;
  bp::register_ptr_to_python<IntegerExpression::SPType>();

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

  //bp::def("expr_make_const", Expr::MakeConstInt);
  bp::def("expr_make_id", Expr::MakeId);

  // exposing enumerations

  bp::enum_<OperationExpression::Type>("expr_op_type")
    .value("OP_UNK", OperationExpression::OpUnk)
    .value("OP_NOT", OperationExpression::OpNot)
    .value("OP_NEG", OperationExpression::OpNeg)
    .value("OP_SWAP", OperationExpression::OpSwap)
    .value("OP_BSF", OperationExpression::OpBsf)
    .value("OP_BSR", OperationExpression::OpBsr)
    .value("OP_XCHG", OperationExpression::OpXchg)
    .value("OP_AND", OperationExpression::OpAnd)
    .value("OP_OR", OperationExpression::OpOr)
    .value("OP_XOR", OperationExpression::OpXor)
    .value("OP_LLS", OperationExpression::OpLls)
    .value("OP_LRS", OperationExpression::OpLrs)
    .value("OP_ARS", OperationExpression::OpArs)
    .value("OP_ROL", OperationExpression::OpRol)
    .value("OP_ADD", OperationExpression::OpAdd)
    .value("OP_SUB", OperationExpression::OpSub)
    .value("OP_MUL", OperationExpression::OpMul)
    .value("OP_SDIV", OperationExpression::OpSDiv)
    .value("OP_UDIV", OperationExpression::OpUDiv)
    .value("OP_SMOD", OperationExpression::OpSMod)
    .value("OP_UMOD", OperationExpression::OpUMod)
    .value("OP_SEXT", OperationExpression::OpSext)
    .value("OP_ZEXT", OperationExpression::OpZext)
    .value("OP_INSERTBITS", OperationExpression::OpInsertBits)
    .value("OP_EXTRACTBITS", OperationExpression::OpExtractBits)
    .value("OP_BCAST", OperationExpression::OpBcast)
    ;
  bp::def("expr_op_get_name", expr_op_get_name);
}
