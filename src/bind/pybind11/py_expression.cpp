#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/expression.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

// get string representation of an identifier expression
char const* GetExprIdIdentifier(IdentifierExpression *e)
{
  return e->GetCpuInformation()->ConvertIdentifierToName(e->GetId());
}

// Type of operation expression: get enumeration name from value
OperationExpression::Type expr_op_get_name(OperationExpression::Type t) { return t; }

void PydusaExpression(py::module& rMod)
{

  // base expression class

  py::class_<Expression, Expression::SPType>(rMod, "Expression")
    .def("__str__", &Expression::ToString)
    .def_property_readonly("size", &Expression::GetBitSize)
    ;

  // constant expression class inherited from base expression class

  py::class_<BitVectorExpression, BitVectorExpression::SPType>(rMod, "BitVectorExpression", py::base<Expression>())
    .def("__str__", &BitVectorExpression::ToString)
    .def_property_readonly("value", &BitVectorExpression::GetInt)
    ;

  // identifier expression class inherited from base expression class

  py::class_<IdentifierExpression, IdentifierExpression::SPType>(rMod, "IdentifierExpression", py::base<Expression>())
    .def("__str__", &IdentifierExpression::ToString)
    .def_property_readonly("id", &GetExprIdIdentifier)
    ;

  // assignment expression class inherited from base expression class

  py::class_<AssignmentExpression, AssignmentExpression::SPType>(rMod, "AssignmentExpression", py::base<Expression>())
    .def("__str__", &AssignmentExpression::ToString)
    .def_property_readonly("dst", &AssignmentExpression::GetDestinationExpression)
    .def_property_readonly("src", &AssignmentExpression::GetSourceExpression)
    ;

  // memory expression class inherited from base expression class

  py::class_<MemoryExpression, MemoryExpression::SPType>(rMod, "MemoryExpression", py::base<Expression>())
    .def("__str__", &MemoryExpression::ToString)
    .def_property_readonly("expr", &MemoryExpression::GetAddressExpression)
    ;

  // operation expression class inherited from base expression class

  py::class_<OperationExpression, OperationExpression::SPType>(rMod, "OperationExpression", py::base<Expression>())
    .def("__str__", &OperationExpression::ToString)
    .def_property_readonly("op", &OperationExpression::GetOperation)
    ;

  // UnaryOperationExpression expression class inherited from operation expression class

  py::class_<UnaryOperationExpression, UnaryOperationExpression::SPType>(rMod, "UnaryOperationExpression", py::base<Expression>())
    .def("__str__", &UnaryOperationExpression::ToString)
    ;

  // BinaryOperationExpression expression class inherited from operation expression class

  py::class_<BinaryOperationExpression, BinaryOperationExpression::SPType>(rMod, "BinaryOperationExpression", py::base<Expression>())
    .def("__str__", &BinaryOperationExpression::ToString)
    ;

  // helpers used to create instance of a specific expression type

  //py::def("expr_make_const", Expr::MakeInt);
  rMod.def("expr_make_id", Expr::MakeId);

  // exposing enumerations

  py::enum_<OperationExpression::Type>(rMod, "expr_op_type")
    .value("OP_UNK", OperationExpression::OpUnk)
    .value("OP_NOT", OperationExpression::OpNot)
    .value("OP_NEG", OperationExpression::OpNeg)
    .value("OP_SWAP", OperationExpression::OpSwap)
    .value("OP_BSF", OperationExpression::OpBsf)
    .value("OP_BSR", OperationExpression::OpBsr)
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
    .value("OP_CLEARBITS", OperationExpression::OpClearBits)
    .value("OP_BCAST", OperationExpression::OpBcast)
    ;
  rMod.def("expr_op_get_name", expr_op_get_name);
}
