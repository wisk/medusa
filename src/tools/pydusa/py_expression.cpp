#include "pydusa.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <medusa/expression.hpp>
#include <medusa/expression_visitor.hpp>

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

  py::class_<Expression, Expression::SPType> pyExpr(rMod, "Expression");
  pyExpr
    .def("__str__", &Expression::ToString)
    .def("update_child", &Expression::UpdateChild)
    .def("compare", &Expression::Compare)
    .def("visit", &Expression::Visit)
    .def_property_readonly("bit_size", &Expression::GetBitSize)
    ;

  py::enum_<Expression::CompareType>(pyExpr, "Compare")
    .value("UNKNOWN", Expression::CmpUnknown)
    .value("DIFFERENT", Expression::CmpDifferent)
    .value("SAME_EXPRESSION", Expression::CmpSameExpression)
    .value("IDENTICAL", Expression::CmpIdentical)
    ;

  // condition expression class inherited from base expression class

  auto const& rConditionExpressionClass = py::class_<ConditionExpression, ConditionExpression::SPType>(rMod, "ConditionExpression", py::base<Expression>())
    .def("__str__", &ConditionExpression::ToString)
    .def_property_readonly("type", &ConditionExpression::GetType)
    .def_property_readonly("condition", &ConditionExpression::GetCondition)
    .def_property_readonly("opposite_condition", &ConditionExpression::GetOppositeCondition)
    .def_property_readonly("referenced_expression", &ConditionExpression::GetReferenceExpression)
    .def_property_readonly("tested_expression", &ConditionExpression::GetTestExpression)
    ;

  auto const& rTernaryConditionExpressionClass = py::class_<TernaryConditionExpression, TernaryConditionExpression::SPType>(rMod, "TernaryConditionExpression", py::base<ConditionExpression>())
    .def_property_readonly("true_expression", &TernaryConditionExpression::GetTrueExpression)
    .def_property_readonly("false_expression", &TernaryConditionExpression::GetFalseExpression)
    ;

  auto const& rIfElseConditionExpressionClass = py::class_<IfElseConditionExpression, IfElseConditionExpression::SPType>(rMod, "IfElseConditionExpression", py::base<ConditionExpression>())
    .def_property_readonly("then_expression", &IfElseConditionExpression::GetThenExpression)
    .def_property_readonly("else_expression", &IfElseConditionExpression::GetElseExpression)
    ;

  auto const& rWhileConditionExpressionClass = py::class_<WhileConditionExpression, WhileConditionExpression::SPType>(rMod, "WhileConditionExpression", py::base<ConditionExpression>())
    .def_property_readonly("body_expression", &WhileConditionExpression::GetBodyExpression)
    ;

  // constant expression class inherited from base expression class

  auto const& rBitVectorExpressionClass = py::class_<BitVectorExpression, BitVectorExpression::SPType>(rMod, "BitVectorExpression", py::base<Expression>())
    .def("__str__", &BitVectorExpression::ToString)
    .def_property_readonly("value", &BitVectorExpression::GetInt)
    ;

  // identifier expression class inherited from base expression class

  auto const& rIdentifierExpressionClass = py::class_<IdentifierExpression, IdentifierExpression::SPType>(rMod, "IdentifierExpression", py::base<Expression>())
    .def("__str__", &IdentifierExpression::ToString)
    .def_property_readonly("identifier", &GetExprIdIdentifier)
    ;

  auto const& rTrackExpressionClass = py::class_<TrackExpression, TrackExpression::SPType>(rMod, "TrackExpression", py::base<Expression>())
    .def("__str__", &TrackExpression::ToString)
    .def_property_readonly("tracked_expression", &TrackExpression::GetTrackedExpression)
    .def_property_readonly("tracked_address", &TrackExpression::GetTrackAddress)
    .def_property_readonly("tracked_position", &TrackExpression::GetTrackPosition)
    ;

  // assignment expression class inherited from base expression class

  auto const& rAssignmentExpressionClass = py::class_<AssignmentExpression, AssignmentExpression::SPType>(rMod, "AssignmentExpression", py::base<Expression>())
    .def("__str__", &AssignmentExpression::ToString)
    .def_property_readonly("destination_expression", &AssignmentExpression::GetDestinationExpression)
    .def_property_readonly("source_expression", &AssignmentExpression::GetSourceExpression)
    ;

  // memory expression class inherited from base expression class

  auto const& rMemoryExpressionClass = py::class_<MemoryExpression, MemoryExpression::SPType>(rMod, "MemoryExpression", py::base<Expression>())
    .def("__str__", &MemoryExpression::ToString)
    .def_property_readonly("access_bit_size", &MemoryExpression::GetAccessSizeInBit)
    .def_property_readonly("base_expression", &MemoryExpression::GetBaseExpression)
    .def_property_readonly("offset_expression", &MemoryExpression::GetOffsetExpression)
    .def_property_readonly("is_deferencable", &MemoryExpression::IsDereferencable)
    ;

  // symbolic expression class inherited from base expression class
  
  auto const& rSymbolicExpressionClass = py::class_<SymbolicExpression, SymbolicExpression::SPType>(rMod, "SymbolicExpression", py::base<Expression>())
    .def("__str__", &SymbolicExpression::ToString)
    .def_property_readonly("type", &SymbolicExpression::GetType)
    .def_property_readonly("value", &SymbolicExpression::GetValue)
    .def_property_readonly("address", &SymbolicExpression::GetAddress)
    .def_property_readonly("expression", &SymbolicExpression::GetExpression)
    ;

  // operation expression class inherited from base expression class

  auto const& rOperationExpressionClass = py::class_<OperationExpression, OperationExpression::SPType>(rMod, "OperationExpression", py::base<Expression>())
    .def("__str__", &OperationExpression::ToString)
    .def_property_readonly("operation", &OperationExpression::GetOperation)
    ;

  // UnaryOperationExpression expression class inherited from operation expression class

  auto const& rUnaryOperationExpressionClass = py::class_<UnaryOperationExpression, UnaryOperationExpression::SPType>(rMod, "UnaryOperationExpression", py::base<OperationExpression>())
    .def("__str__", &UnaryOperationExpression::ToString)
    .def_property_readonly("expression", &UnaryOperationExpression::GetExpression)
    ;

  // BinaryOperationExpression expression class inherited from operation expression class

  auto const& rBinaryOperationClass = py::class_<BinaryOperationExpression, BinaryOperationExpression::SPType>(rMod, "BinaryOperationExpression", py::base<OperationExpression>())
    .def("__str__", &BinaryOperationExpression::ToString)
    .def_property_readonly("left_expression", &BinaryOperationExpression::GetLeftExpression)
    .def_property_readonly("right_expression", &BinaryOperationExpression::GetRightExpression)
    ;

  // helpers used to create instance of a specific expression type

  rMod
    .def("expr_make_tern_cond", &Expr::MakeTernaryCond)
    .def("expr_make_bv", (Expression::SPType(*)(BitVector const&))&Expr::MakeBitVector)
    .def("expr_make_bool", &Expr::MakeBoolean)
    .def("expr_make_id", &Expr::MakeId)
    .def("expr_make_trk", &Expr::MakeTrack)
    .def("expr_make_mem", &Expr::MakeMem)
    .def("expr_make_un_op", &Expr::MakeUnOp)
    .def("expr_make_bin_op", &Expr::MakeBinOp)
    .def("expr_make_assign", &Expr::MakeAssign)
    .def("expr_make_sym", &Expr::MakeSym)
    ;

  // exposing enumerations

  py::enum_<ConditionExpression::Type>(rConditionExpressionClass, "Type")
    .value("COND_UNKNOWN", ConditionExpression::CondUnk)
    .value("COND_EQ",      ConditionExpression::CondEq)
    .value("COND_NE",      ConditionExpression::CondNe)
    .value("COND_UGT",     ConditionExpression::CondUgt)
    .value("COND_UGE",     ConditionExpression::CondUge)
    .value("COND_ULT",     ConditionExpression::CondUlt)
    .value("COND_ULE",     ConditionExpression::CondUle)
    .value("COND_SGT",     ConditionExpression::CondSgt)
    .value("COND_SGE",     ConditionExpression::CondSge)
    .value("COND_SLT",     ConditionExpression::CondSlt)
    .value("COND_SLE",     ConditionExpression::CondSle)
    ;

  py::enum_<OperationExpression::Type>(rOperationExpressionClass, "Type")
    .value("OP_UNK",         OperationExpression::OpUnk)
    .value("OP_NOT",         OperationExpression::OpNot)
    .value("OP_NEG",         OperationExpression::OpNeg)
    .value("OP_SWAP",        OperationExpression::OpSwap)
    .value("OP_BSF",         OperationExpression::OpBsf)
    .value("OP_BSR",         OperationExpression::OpBsr)
    .value("OP_AND",         OperationExpression::OpAnd)
    .value("OP_OR",          OperationExpression::OpOr)
    .value("OP_XOR",         OperationExpression::OpXor)
    .value("OP_LLS",         OperationExpression::OpLls)
    .value("OP_LRS",         OperationExpression::OpLrs)
    .value("OP_ARS",         OperationExpression::OpArs)
    .value("OP_ROL",         OperationExpression::OpRol)
    .value("OP_ADD",         OperationExpression::OpAdd)
    .value("OP_SUB",         OperationExpression::OpSub)
    .value("OP_MUL",         OperationExpression::OpMul)
    .value("OP_SDIV",        OperationExpression::OpSDiv)
    .value("OP_UDIV",        OperationExpression::OpUDiv)
    .value("OP_SMOD",        OperationExpression::OpSMod)
    .value("OP_UMOD",        OperationExpression::OpUMod)
    .value("OP_SEXT",        OperationExpression::OpSext)
    .value("OP_ZEXT",        OperationExpression::OpZext)
    .value("OP_INSERTBITS",  OperationExpression::OpInsertBits)
    .value("OP_EXTRACTBITS", OperationExpression::OpExtractBits)
    .value("OP_CLEARBITS",   OperationExpression::OpClearBits)
    .value("OP_BCAST",       OperationExpression::OpBcast)
    ;
  rMod.def("expr_op_get_name", expr_op_get_name);

  py::enum_<SymbolicExpression::Type>(rSymbolicExpressionClass, "Type")
    .value("SYM_UNKNOWN",           SymbolicExpression::Unknown)
    .value("SYM_RETURN_VAL",        SymbolicExpression::ReturnedValue)
    .value("SYM_FROM_PARAMETER",    SymbolicExpression::FromParameter)
    .value("SYM_EXTERNAL_VALUE",    SymbolicExpression::ExternalValue)
    .value("SYM_EXTERNAL_FUNCTION", SymbolicExpression::ExternalFunction)
    .value("SYM_UNDEFINED",         SymbolicExpression::Undefined)
    ;

  py::class_<ExpressionVisitor>(rMod, "ExpressionVisitor");

  py::class_<NormalizeIdentifier>(rMod, "NormalizeIdentifier", py::base<ExpressionVisitor>())
    .def(py::init<CpuInformation const&, u8>())
    ;

  py::class_<IdentifierToVariable>(rMod, "IdentifierToVariable", py::base<ExpressionVisitor>())
    .def(py::init<>())
    ;

  py::class_<SimplifyVisitor>(rMod, "SimplifyVisitor", py::base<ExpressionVisitor>())
    .def(py::init<>())
    ;

  py::class_<FilterVisitor>(rMod, "FilterVisitor", py::base<ExpressionVisitor>())
    .def(py::init<FilterVisitor::Matcher, size_t>())
    .def("get_matched_expressions", &FilterVisitor::GetMatchedExpressions)
    ;

  py::class_<SymbolicVisitor>(rMod, "SymbolicVisitor", py::base<ExpressionVisitor>())
    .def(py::init<Document const&, u8, bool>())
    .def("fork", &SymbolicVisitor::Fork)
    .def("remove_expression_annotations", &SymbolicVisitor::RemoveExpressionAnnotations)
    .def("get_value", &SymbolicVisitor::GetValue)
    .def("find_expressions_by_key", &SymbolicVisitor::FindExpressionsByKey)
    .def("find_expressions_by_value", &SymbolicVisitor::FindExpressionsByValue)
    .def("find_expressions_by_use", &SymbolicVisitor::FindExpressionsByUse)
    .def("bind_expression", &SymbolicVisitor::BindExpression)
    .def("update_address", &SymbolicVisitor::UpdateAddress)
    .def("find_all_paths", &SymbolicVisitor::FindAllPaths)
    .def("__str__", &SymbolicVisitor::ToString)
    //.def_property_readonly("expressions", &SymbolicVisitor::GetExpressions)
    ;
}
