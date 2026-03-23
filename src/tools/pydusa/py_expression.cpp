#include "pydusa.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <medusa/expression.hpp>
#include <medusa/expression_visitor.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

// get string representation of an identifier expression
char const* GetExprIdIdentifier(IdentifierExpression *e)
{
  return e->GetCpuInformation()->ConvertIdentifierToName(e->GetId());
}

// Type of operation expression: get enumeration name from value
OperationExpression::Type expr_op_get_name(OperationExpression::Type t) { return t; }

void PydusaExpression(nb::module_& rMod)
{

  // base expression class

  nb::class_<Expression> pyExpr(rMod, "Expression");
  pyExpr
    .def("__str__", &Expression::ToString)
    .def_static("parse", &Expression::Parse)
    .def("update_child", &Expression::UpdateChild)
    .def("compare", &Expression::Compare)
    .def("visit", &Expression::Visit)
    .def_prop_ro("bit_size", &Expression::GetBitSize)
    ;

  nb::enum_<Expression::CompareType>(pyExpr, "Compare")
    .value("UNKNOWN", Expression::CmpUnknown)
    .value("DIFFERENT", Expression::CmpDifferent)
    .value("SAME_EXPRESSION", Expression::CmpSameExpression)
    .value("IDENTICAL", Expression::CmpIdentical)
    ;

  // condition expression class inherited from base expression class

  auto const& rConditionExpressionClass = nb::class_<ConditionExpression, Expression>(rMod, "ConditionExpression")
    .def("__str__", &ConditionExpression::ToString)
    .def_prop_ro("type", &ConditionExpression::GetType)
    .def_prop_ro("condition", &ConditionExpression::GetCondition)
    .def_prop_ro("opposite_condition", &ConditionExpression::GetOppositeCondition)
    .def_prop_ro("referenced_expression", &ConditionExpression::GetReferenceExpression)
    .def_prop_ro("tested_expression", &ConditionExpression::GetTestExpression)
    ;

  auto const& rTernaryConditionExpressionClass = nb::class_<TernaryConditionExpression, ConditionExpression>(rMod, "TernaryConditionExpression")
    .def_prop_ro("true_expression", &TernaryConditionExpression::GetTrueExpression)
    .def_prop_ro("false_expression", &TernaryConditionExpression::GetFalseExpression)
    ;

  auto const& rIfElseConditionExpressionClass = nb::class_<IfElseConditionExpression, ConditionExpression>(rMod, "IfElseConditionExpression")
    .def_prop_ro("then_expression", &IfElseConditionExpression::GetThenExpression)
    .def_prop_ro("else_expression", &IfElseConditionExpression::GetElseExpression)
    ;

  auto const& rWhileConditionExpressionClass = nb::class_<WhileConditionExpression, ConditionExpression>(rMod, "WhileConditionExpression")
    .def_prop_ro("body_expression", &WhileConditionExpression::GetBodyExpression)
    ;

  // constant expression class inherited from base expression class

  auto const& rBitVectorExpressionClass = nb::class_<BitVectorExpression, Expression>(rMod, "BitVectorExpression")
    .def("__str__", &BitVectorExpression::ToString)
    .def_prop_ro("value", &BitVectorExpression::GetInt)
    ;

  // identifier expression class inherited from base expression class

  auto const& rIdentifierExpressionClass = nb::class_<IdentifierExpression, Expression>(rMod, "IdentifierExpression")
    .def("__str__", &IdentifierExpression::ToString)
    .def_prop_ro("identifier", &GetExprIdIdentifier)
    ;

  auto const& rTrackExpressionClass = nb::class_<TrackExpression, Expression>(rMod, "TrackExpression")
    .def("__str__", &TrackExpression::ToString)
    .def_prop_ro("tracked_expression", &TrackExpression::GetTrackedExpression)
    .def_prop_ro("tracked_address", &TrackExpression::GetTrackAddress)
    .def_prop_ro("tracked_position", &TrackExpression::GetTrackPosition)
    ;

  // assignment expression class inherited from base expression class

  auto const& rAssignmentExpressionClass = nb::class_<AssignmentExpression, Expression>(rMod, "AssignmentExpression")
    .def("__str__", &AssignmentExpression::ToString)
    .def_prop_ro("destination_expression", &AssignmentExpression::GetDestinationExpression)
    .def_prop_ro("source_expression", &AssignmentExpression::GetSourceExpression)
    ;

  // memory expression class inherited from base expression class

  auto const& rMemoryExpressionClass = nb::class_<MemoryExpression, Expression>(rMod, "MemoryExpression")
    .def("__str__", &MemoryExpression::ToString)
    .def_prop_ro("access_bit_size", &MemoryExpression::GetAccessSizeInBit)
    .def_prop_ro("base_expression", &MemoryExpression::GetBaseExpression)
    .def_prop_ro("offset_expression", &MemoryExpression::GetOffsetExpression)
    .def_prop_ro("is_deferencable", &MemoryExpression::IsDereferencable)
    ;

  // symbolic expression class inherited from base expression class

  auto const& rSymbolicExpressionClass = nb::class_<SymbolicExpression, Expression>(rMod, "SymbolicExpression")
    .def("__str__", &SymbolicExpression::ToString)
    .def_prop_ro("type", &SymbolicExpression::GetType)
    .def_prop_ro("value", &SymbolicExpression::GetValue)
    .def_prop_ro("address", &SymbolicExpression::GetAddress)
    .def_prop_ro("expression", &SymbolicExpression::GetExpression)
    ;

  // variable expression class inherited from base expression class

  auto const& rVariableExpressionClass = nb::class_<VariableExpression, Expression>(rMod, "VariableExpression")
    .def("__str__", &VariableExpression::ToString)
    .def_prop_ro("name", &VariableExpression::GetName)
    .def_prop_ro("type", &VariableExpression::GetType)
    ;

  // operation expression class inherited from base expression class

  auto const& rOperationExpressionClass = nb::class_<OperationExpression, Expression>(rMod, "OperationExpression")
    .def("__str__", &OperationExpression::ToString)
    .def_prop_ro("operation", &OperationExpression::GetOperation)
    ;

  // UnaryOperationExpression expression class inherited from operation expression class

  auto const& rUnaryOperationExpressionClass = nb::class_<UnaryOperationExpression, OperationExpression>(rMod, "UnaryOperationExpression")
    .def("__str__", &UnaryOperationExpression::ToString)
    .def_prop_ro("expression", &UnaryOperationExpression::GetExpression)
    ;

  // BinaryOperationExpression expression class inherited from operation expression class

  auto const& rBinaryOperationClass = nb::class_<BinaryOperationExpression, OperationExpression>(rMod, "BinaryOperationExpression")
    .def("__str__", &BinaryOperationExpression::ToString)
    .def_prop_ro("left_expression", &BinaryOperationExpression::GetLeftExpression)
    .def_prop_ro("right_expression", &BinaryOperationExpression::GetRightExpression)
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

  nb::enum_<ConditionExpression::Type>(rConditionExpressionClass, "Type")
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

  nb::enum_<OperationExpression::Type>(rOperationExpressionClass, "Type")
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

  nb::enum_<SymbolicExpression::Type>(rSymbolicExpressionClass, "Type")
    .value("SYM_UNKNOWN",           SymbolicExpression::Unknown)
    .value("SYM_RETURNED_VALUE",    SymbolicExpression::ReturnedValue)
    .value("SYM_FROM_PARAMETER",    SymbolicExpression::FromParameter)
    .value("SYM_EXTERNAL_VALUE",    SymbolicExpression::ExternalValue)
    .value("SYM_EXTERNAL_FUNCTION", SymbolicExpression::ExternalFunction)
    .value("SYM_UNDEFINED",         SymbolicExpression::Undefined)
    ;

  nb::enum_<VariableExpression::Type>(rVariableExpressionClass, "Type")
    .value("VAR_UNKNOWN",    VariableExpression::Unknown)
    .value("VAR_ALLOC",      VariableExpression::Alloc)
    .value("VAR_FREE",       VariableExpression::Free)
    .value("VAR_USE",        VariableExpression::Use)
    ;

  nb::class_<ExpressionVisitor>(rMod, "ExpressionVisitor");

  nb::class_<NormalizeIdentifier, ExpressionVisitor>(rMod, "NormalizeIdentifier")
    .def(nb::init<CpuInformation const&, u8>())
    ;

  nb::class_<IdentifierToVariable, ExpressionVisitor>(rMod, "IdentifierToVariable")
    .def(nb::init<>())
    .def_prop_ro("used_identifier", &IdentifierToVariable::GetUsedId)
    ;

  nb::class_<SimplifyVisitor, ExpressionVisitor>(rMod, "SimplifyVisitor")
    .def(nb::init<>())
    ;

  nb::class_<FilterVisitor, ExpressionVisitor>(rMod, "FilterVisitor")
    .def(nb::init<FilterVisitor::Matcher, size_t>())
    .def("get_matched_expressions", &FilterVisitor::GetMatchedExpressions)
    ;

  nb::class_<SymbolicVisitor, ExpressionVisitor>(rMod, "SymbolicVisitor")
    .def(nb::init<Document const&, u8, bool>())
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
    //.def_prop_ro("expressions", &SymbolicVisitor::GetExpressions)
    ;
}
