#ifndef MEDUSA_EXPRESSION_VISITOR_HPP
#define MEDUSA_EXPRESSION_VISITOR_HPP

#include "medusa/expression.hpp"
#include "medusa/expression_simplifier.hpp"
#include "medusa/architecture.hpp"

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT CloneVisitor : public ExpressionVisitor
{
public:
  virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
  virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
  virtual Expression::SPType VisitCondition(ConditionExpression::SPType spCondExpr);
  virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBitVector(BitVectorExpression::SPType spIntExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr);
  virtual Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr);
  virtual Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr);
  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
  virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);
};

class MEDUSA_EXPORT FilterVisitor : public ExpressionVisitor
{
public:
  typedef std::function <Expression::SPType(Expression::SPType spExpr)> Matcher;
  FilterVisitor(Matcher ExprMatcher, size_t NrOfResult = 0) : m_ExprMatcher(ExprMatcher), m_NbrOfResult(NrOfResult) {}

  virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
  virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
  virtual Expression::SPType VisitCondition(ConditionExpression::SPType spCondExpr);
  virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBitVector(BitVectorExpression::SPType spIntExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr);
  virtual Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr);
  virtual Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr);
  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
  virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);

  Expression::VSPType  GetMatchedExpressions(void) { return m_MatchedExprs; }

protected:
  void _Evaluate(Expression::SPType spExpr);
  void _AddExpression(Expression::SPType spExpr);
  bool _IsDone(void) const;

  Matcher m_ExprMatcher;
  size_t m_NbrOfResult;
  Expression::VSPType m_MatchedExprs;
};

class MEDUSA_EXPORT EvaluateVisitor : public ExpressionVisitor
{
public:
  // LATER: It'd be better to use a context filled by the architecture itself in order to correctly
  // map the PC pointer. On some architecture the PC is set on multiple register (e.g.: x86 cs:offset,
  // z80/gb bank:offset...).
  EvaluateVisitor(Document const& rDoc, Address const& rCurAddr, u8 Mode, bool EvalMemRef = true);

  virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
  virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
  virtual Expression::SPType VisitCondition(ConditionExpression::SPType spCondExpr);
  virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBitVector(BitVectorExpression::SPType spIntExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr);
  virtual Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr);
  virtual Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr);
  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
  virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);

  bool IsSymbolic(void) const { return m_IsSymbolic; }
  bool IsRelative(void) const { return m_IsRelative; }
  bool IsMemoryReference(void) const { return m_IsMemoryReference; }

  Expression::SPType GetResultExpression(void) const { return m_spResExpr; }
  void SetId(u32 Id, Expression::SPType spExpr);

protected:
  bool _EvaluateCondition(u8 CondOp, BitVectorExpression::SPType spConstRefExpr, BitVectorExpression::SPType spConstTestExpr, bool& rRes) const;

  Document const&    m_rDoc;
  u8                 m_Mode;
  Address const&     m_rCurAddr;
  std::unordered_map<u32, Expression::SPType> m_Ids;
  bool               m_IsSymbolic;
  bool               m_IsRelative;
  bool               m_IsMemoryReference;
  bool               m_EvalMemRef;
  Expression::SPType m_spResExpr;
};

class MEDUSA_EXPORT SymbolicVisitor : public ExpressionVisitor
{
public:
  SymbolicVisitor(Document const& rDoc, u8 Mode, bool EvalMemRef = true);

  virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
  virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
  virtual Expression::SPType VisitCondition(ConditionExpression::SPType spCondExpr);
  virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBitVector(BitVectorExpression::SPType spIntExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr);
  virtual Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr);
  virtual Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr);
  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
  virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);

  SymbolicVisitor Fork(void) const;

  bool IsSymbolic(void) const { return m_IsSymbolic; }
  bool IsRelative(void) const { return m_IsRelative; }
  bool IsMemoryReference(void) const { return m_IsMemoryReference; }

  static Expression::SPType  RemoveExpressionAnnotations(Expression::SPType spExpr);
  Expression::SPType         GetValue(Expression::SPType spExpr) const;
  Expression::VSPType        FindExpressionsByKey(Expression::SPType spPatExpr) const;
  Expression::VSPType        FindExpressionsByValue(Expression::SPType spPatExpr) const;
  Expression::VSPType        FindExpressionsByUse(Expression::SPType spPatExpr) const;

  std::string ToString(void) const;
  bool BindExpression(Expression::SPType spKeyExpr, Expression::SPType spValueExpr, bool Propagate = false);
  bool UpdateAddress(Architecture& rArch, Address const& rAddr);

  typedef std::function<bool(Expression::SPType& rspExpr)> Updater;
  bool UpdateExpression(Expression::SPType spKeyExpr, Updater updt);

  typedef std::function<void(Expression::SPType spDstExpr, Expression::VSPType spCondExprs)> DestinationPathCallbackType;
  bool FindAllPaths(int& rNumOfPathFound, Architecture& rArch, DestinationPathCallbackType DstPathCb);

protected:
  void _InsertExpression(Expression::SPType spKeyExpr, Expression::SPType spValExpr);
  bool _EvaluateCondition(u8 CondOp, BitVectorExpression::SPType spConstRefExpr, BitVectorExpression::SPType spConstTestExpr, bool& rRes) const;

  typedef std::map<Expression::SPType, Expression::SPType> SymbolicContextType;
  Document const&                   m_rDoc;
  u8                                m_Mode;
  SymbolicContextType               m_SymCtxt;
  Expression::VSPType               m_SymCond;            // This field contains assumption needed for symbolic context to be valid.
  std::set<std::string>             m_VarPool;
  IfElseConditionExpression::SPType m_spCond;             // Used to transformed IfElseExpr to TernExpr when possible.
  bool                              m_IsSymbolic;
  bool                              m_IsRelative;
  bool                              m_IsMemoryReference;
  bool                              m_EvalMemRef;
  bool                              m_Update;
  Address                           m_CurAddr;
  u8                                m_CurPos;
};

//! Visit an expression and convert IdentifierExpression to TrackExpression.
class MEDUSA_EXPORT TrackVisitor : public CloneVisitor
{
public:
  TrackVisitor(Address const& rCurAddr, Track::Context& rCtxt);
  Expression::SPType GetTrackedExpression(void); // NOTE: caller take the ownership

  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr);

private:
  Address m_CurAddr;
  Track::Context& m_rCtxt;
  Expression::SPType m_spTrackedExpr;
  bool m_IsAssigned;
};

//! BackTrackVisitor looks for TrackedIdentifier expression which correponds to both rAddr and Id
//! If the expression contains a track id, GetResult will return true
//! Visit always return nullptr
//! BackTrackContext is updated with the source
class MEDUSA_EXPORT BackTrackVisitor : public ExpressionVisitor
{
public:
  BackTrackVisitor(Track::BackTrackContext& rBtCtxt)
    : m_rBtCtxt(rBtCtxt)
    , m_IsAssigned(false), m_TrackSource(false), m_Result(false) {}

  bool GetResult(void) const { return m_Result; }

  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr);

private:
  Track::BackTrackContext& m_rBtCtxt;
  bool m_IsAssigned;
  bool m_TrackSource;
  bool m_Result;
};

class MEDUSA_EXPORT NormalizeIdentifier : public CloneVisitor
{
public:
  NormalizeIdentifier(CpuInformation const& rCpuinfo, u8 Mode);

  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);

protected:
  CpuInformation const& m_rCpuInfo;
  u8 m_Mode;
};

class MEDUSA_EXPORT IdentifierToVariable : public CloneVisitor
{
public:
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  std::set<u32> const& GetUsedId(void) { return m_UsedId; }

private:
  std::set<u32> m_UsedId;
};

class MEDUSA_EXPORT SimplifyVisitor : public CloneVisitor
{
public:
  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
};

class MEDUSA_EXPORT ConstantFoldingVisitor : public CloneVisitor
{
public:
  ConstantFoldingVisitor(Document const& rDoc, Address const& rCurAddr, u8 Mode, bool EvalMemRef = true)
          : m_rDoc(rDoc), m_CurAddr(rCurAddr), m_Mode(Mode)
  {}

  enum  Position
  {
      Left = 0,
      Right,
  };

  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr);
  bool                       IsCommutative(BinaryOperationExpression::SPType spBinOpExpr);
  bool                       IsAssociative(Expression::SPType spBinOpExpr);
  Expression::SPType         GetOperand(BinaryOperationExpression::SPType spBinOpExpr, ConstantFoldingVisitor::Position Operand);
  Expression::SPType         SimplifyAssociativeOrCommutative(BinaryOperationExpression::SPType spBinOpExpr);
  Expression::SPType         SimplifyBinOp(OperationExpression::Type Operation, Expression::SPType spLeftExpr, Expression::SPType spRightExpr);

protected:
  Document const&                   m_rDoc;
  u8                                m_Mode;
  Address                           m_CurAddr;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXPRESSION_VISITOR_HPP
