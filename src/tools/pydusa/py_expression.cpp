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
    .def_static("parse", &Expression::Parse)
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

  auto const& rConditionExpressionClass = py::class_<ConditionExpression, ConditionExpression::SPType, Expression>(rMod, "ConditionExpression")
    .def("__str__", &ConditionExpression::ToString)
    .def_property_readonly("type", &ConditionExpression::GetType)
    .def_property_readonly("condition", &ConditionExpression::GetCondition)
    .def_property_readonly("opposite_condition", &ConditionExpression::GetOppositeCondition)
    .def_property_readonly("referenced_expression", &ConditionExpression::GetReferenceExpression)
    .def_property_readonly("tested_expression", &ConditionExpression::GetTestExpression)
    ;

  auto const& rTernaryConditionExpressionClass = py::class_<TernaryConditionExpression, TernaryConditionExpression::SPType, ConditionExpression>(rMod, "TernaryConditionExpression")
    .def_property_readonly("true_expression", &TernaryConditionExpression::GetTrueExpression)
    .def_property_readonly("false_expression", &TernaryConditionExpression::GetFalseExpression)
    ;

  auto const& rIfElseConditionExpressionClass = py::class_<IfElseConditionExpression, IfElseConditionExpression::SPType, ConditionExpression>(rMod, "IfElseConditionExpression")
    .def_property_readonly("then_expression", &IfElseConditionExpression::GetThenExpression)
    .def_property_readonly("else_expression", &IfElseConditionExpression::GetElseExpression)
    ;

  auto const& rWhileConditionExpressionClass = py::class_<WhileConditionExpression, WhileConditionExpression::SPType, ConditionExpression>(rMod, "WhileConditionExpression")
    .def_property_readonly("body_expression", &WhileConditionExpression::GetBodyExpression)
    ;

  // constant expression class inherited from base expression class

  auto const& rBitVectorExpressionClass = py::class_<BitVectorExpression, BitVectorExpression::SPType, Expression>(rMod, "BitVectorExpression")
    .def("__str__", &BitVectorExpression::ToString)
    .def_property_readonly("value", &BitVectorExpression::GetInt)
    ;

  // identifier expression class inherited from base expression class

  auto const& rIdentifierExpressionClass = py::class_<IdentifierExpression, IdentifierExpression::SPType, Expression>(rMod, "IdentifierExpression")
    .def("__str__", &IdentifierExpression::ToString)
    .def_property_readonly("identifier", &GetExprIdIdentifier)
    ;

  auto const& rTrackExpressionClass = py::class_<TrackExpression, TrackExpression::SPType, Expression>(rMod, "TrackExpression")
    .def("__str__", &TrackExpression::ToString)
    .def_property_readonly("tracked_expression", &TrackExpression::GetTrackedExpression)
    .def_property_readonly("tracked_address", &TrackExpression::GetTrackAddress)
    .def_property_readonly("tracked_position", &TrackExpression::GetTrackPosition)
    ;

  // assignment expression class inherited from base expression class

  auto const& rAssignmentExpressionClass = py::class_<AssignmentExpression, AssignmentExpression::SPType, Expression>(rMod, "AssignmentExpression")
    .def("__str__", &AssignmentExpression::ToString)
    .def_property_readonly("destination_expression", &AssignmentExpression::GetDestinationExpression)
    .def_property_readonly("source_expression", &AssignmentExpression::GetSourceExpression)
    ;

  // memory expression class inherited from base expression class

  auto const& rMemoryExpressionClass = py::class_<MemoryExpression, MemoryExpression::SPType, Expression>(rMod, "MemoryExpression")
    .def("__str__", &MemoryExpression::ToString)
    .def_property_readonly("access_bit_size", &MemoryExpression::GetAccessSizeInBit)
    .def_property_readonly("base_expression", &MemoryExpression::GetBaseExpression)
    .def_property_readonly("offset_expression", &MemoryExpression::GetOffsetExpression)
    .def_property_readonly("is_deferencable", &MemoryExpression::IsDereferencable)
    ;

  // symbolic expression class inherited from base expression class

  auto const& rSymbolicExpressionClass = py::class_<SymbolicExpression, SymbolicExpression::SPType, Expression>(rMod, "SymbolicExpression")
    .def("__str__", &SymbolicExpression::ToString)
    .def_property_readonly("type", &SymbolicExpression::GetType)
    .def_property_readonly("value", &SymbolicExpression::GetValue)
    .def_property_readonly("address", &SymbolicExpression::GetAddress)
    .def_property_readonly("expression", &SymbolicExpression::GetExpression)
    ;

  // variable expression class inherited from base expression class

  auto const& rVariableExpressionClass = py::class_<VariableExpression, VariableExpression::SPType, Expression>(rMod, "VariableExpression")
    .def("__str__", &VariableExpression::ToString)
    .def_property_readonly("name", &VariableExpression::GetName)
    .def_property_readonly("type", &VariableExpression::GetType)
    ;

  // operation expression class inherited from base expression class

  auto const& rOperationExpressionClass = py::class_<OperationExpression, OperationExpression::SPType, Expression>(rMod, "OperationExpression")
    .def("__str__", &OperationExpression::ToString)
    .def_property_readonly("operation", &OperationExpression::GetOperation)
    ;

  // UnaryOperationExpression expression class inherited from operation expression class

  auto const& rUnaryOperationExpressionClass = py::class_<UnaryOperationExpression, UnaryOperationExpression::SPType, OperationExpression>(rMod, "UnaryOperationExpression")
    .def("__str__", &UnaryOperationExpression::ToString)
    .def_property_readonly("expression", &UnaryOperationExpression::GetExpression)
    ;

  // BinaryOperationExpression expression class inherited from operation expression class

  auto const& rBinaryOperationClass = py::class_<BinaryOperationExpression, BinaryOperationExpression::SPType, OperationExpression>(rMod, "BinaryOperationExpression")
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
    .def("expr_make_var", &Expr::MakeVar)
    ;

  // exposing enumerations

  py::enum_<ConditionExpression::Type>(rConditionExpressionClass, "COND")
    .value("UNKNOWN", ConditionExpression::CondUnk)
    .value("EQ",      ConditionExpression::CondEq)
    .value("NE",      ConditionExpression::CondNe)
    .value("UGT",     ConditionExpression::CondUgt)
    .value("UGE",     ConditionExpression::CondUge)
    .value("ULT",     ConditionExpression::CondUlt)
    .value("ULE",     ConditionExpression::CondUle)
    .value("SGT",     ConditionExpression::CondSgt)
    .value("SGE",     ConditionExpression::CondSge)
    .value("SLT",     ConditionExpression::CondSlt)
    .value("SLE",     ConditionExpression::CondSle)
    ;

  py::enum_<OperationExpression::Type>(rOperationExpressionClass, "OP")
    .value("UNK",         OperationExpression::OpUnk)
    .value("NOT",         OperationExpression::OpNot)
    .value("NEG",         OperationExpression::OpNeg)
    .value("SWAP",        OperationExpression::OpSwap)
    .value("BSF",         OperationExpression::OpBsf)
    .value("BSR",         OperationExpression::OpBsr)
    .value("AND",         OperationExpression::OpAnd)
    .value("OR",          OperationExpression::OpOr)
    .value("XOR",         OperationExpression::OpXor)
    .value("LLS",         OperationExpression::OpLls)
    .value("LRS",         OperationExpression::OpLrs)
    .value("ARS",         OperationExpression::OpArs)
    .value("ROL",         OperationExpression::OpRol)
    .value("ADD",         OperationExpression::OpAdd)
    .value("SUB",         OperationExpression::OpSub)
    .value("MUL",         OperationExpression::OpMul)
    .value("SDIV",        OperationExpression::OpSDiv)
    .value("UDIV",        OperationExpression::OpUDiv)
    .value("SMOD",        OperationExpression::OpSMod)
    .value("UMOD",        OperationExpression::OpUMod)
    .value("SEXT",        OperationExpression::OpSext)
    .value("ZEXT",        OperationExpression::OpZext)
    .value("INSERTBITS",  OperationExpression::OpInsertBits)
    .value("EXTRACTBITS", OperationExpression::OpExtractBits)
    .value("CLEARBITS",   OperationExpression::OpClearBits)
    .value("BCAST",       OperationExpression::OpBcast)
    ;
  rMod.def("expr_op_get_name", expr_op_get_name);

  py::enum_<SymbolicExpression::Type>(rSymbolicExpressionClass, "SYM")
    .value("UNKNOWN",           SymbolicExpression::Unknown)
    .value("RETURNED_VALUE",    SymbolicExpression::ReturnedValue)
    .value("FROM_PARAMETER",    SymbolicExpression::FromParameter)
    .value("EXTERNAL_VALUE",    SymbolicExpression::ExternalValue)
    .value("EXTERNAL_FUNCTION", SymbolicExpression::ExternalFunction)
    .value("UNDEFINED",         SymbolicExpression::Undefined)
    ;

  py::enum_<VariableExpression::Type>(rVariableExpressionClass, "VAR")
    .value("UNKNOWN",    VariableExpression::Unknown)
    .value("ALLOC",      VariableExpression::Alloc)
    .value("FREE",       VariableExpression::Free)
    .value("USE",        VariableExpression::Use)
    ;

  py::class_<ExpressionVisitor>(rMod, "ExpressionVisitor");

  py::class_<NormalizeIdentifier, ExpressionVisitor>(rMod, "NormalizeIdentifier")
    .def(py::init<CpuInformation const&, u8>())
    ;

  py::class_<IdentifierToVariable, ExpressionVisitor>(rMod, "IdentifierToVariable")
    .def(py::init<>())
    .def_property_readonly("used_identifier", &IdentifierToVariable::GetUsedId)
    ;

  py::class_<SimplifyVisitor, ExpressionVisitor>(rMod, "SimplifyVisitor")
    .def(py::init<>())
    ;

  py::class_<FilterVisitor, ExpressionVisitor>(rMod, "FilterVisitor")
    .def(py::init<FilterVisitor::Matcher, size_t>())
    .def("get_matched_expressions", &FilterVisitor::GetMatchedExpressions)
    ;

  py::class_<SymbolicVisitor, ExpressionVisitor>(rMod, "SymbolicVisitor")
    .def(py::init<Document const&, u8, bool>())
    .def("fork", &SymbolicVisitor::Fork)
    .def_static("remove_expression_annotations", &SymbolicVisitor::RemoveExpressionAnnotations)
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
