#ifndef MEDUSA_EXPRESSION_VISITOR_HPP
#define MEDUSA_EXPRESSION_VISITOR_HPP

#include "medusa/expression.hpp"
#include "medusa/expression_simplifier.hpp"
#include "medusa/architecture.hpp"

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT CloneVisitor : public ExpressionVisitor
{
public:
  Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr) override;
  Expression::SPType VisitBind(BindExpression::SPType spBindExpr) override;
  Expression::SPType VisitCondition(ConditionExpression::SPType spCondExpr) override;
  Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr) override;
  Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr) override;
  Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr) override;
  Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr) override;
  Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spOpExpr) override;
  Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spOpExpr) override;
  Expression::SPType VisitBitVector(BitVectorExpression::SPType spIntExpr) override;
  Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr) override;
  Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr) override;
  Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr) override;
  Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr) override;
  Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr) override;
  Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr) override;
};

class MEDUSA_EXPORT FilterVisitor : public ExpressionVisitor
{
public:
  using Matcher = std::function <Expression::SPType(Expression::SPType spExpr)>;
  FilterVisitor(Matcher ExprMatcher, size_t NrOfResult = 0) : m_ExprMatcher(ExprMatcher), m_NbrOfResult(NrOfResult) {}

  Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr) override;
  Expression::SPType VisitBind(BindExpression::SPType spBindExpr) override;
  Expression::SPType VisitCondition(ConditionExpression::SPType spCondExpr) override;
  Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr) override;
  Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr) override;
  Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr) override;
  Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr) override;
  Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spOpExpr) override;
  Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spOpExpr) override;
  Expression::SPType VisitBitVector(BitVectorExpression::SPType spIntExpr) override;
  Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr) override;
  Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr) override;
  Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr) override;
  Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr) override;
  Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr) override;
  Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr) override;

  Expression::VSPType  GetMatchedExpressions() { return m_MatchedExprs; }

protected:
  void _Evaluate(Expression::SPType spExpr);
  void _AddExpression(Expression::SPType spExpr);
  bool _IsDone() const;

  Matcher m_ExprMatcher;
  size_t m_NbrOfResult;
  Expression::VSPType m_MatchedExprs;
};

class MEDUSA_EXPORT EvaluateVisitor : public ExpressionVisitor
{
public:
  // LATER: It'd be better to use a context filled by the architecture itself in order to correctly
  // map the PC pointer. On some architecture the PC is set on multiple register (e.g.: x86 cs:offset,
  // lr35902/gb bank:offset...).
  EvaluateVisitor(Document const& rDoc, Address const& rCurAddr, u8 Mode, bool EvalMemRef = true);

  Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr) override;
  Expression::SPType VisitBind(BindExpression::SPType spBindExpr) override;
  Expression::SPType VisitCondition(ConditionExpression::SPType spCondExpr) override;
  Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr) override;
  Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr) override;
  Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr) override;
  Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr) override;
  Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spOpExpr) override;
  Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spOpExpr) override;
  Expression::SPType VisitBitVector(BitVectorExpression::SPType spIntExpr) override;
  Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr) override;
  Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr) override;
  Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr) override;
  Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr) override;
  Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr) override;
  Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr) override;

  bool IsSymbolic() const { return m_IsSymbolic; }
  bool IsRelative() const { return m_IsRelative; }
  bool IsMemoryReference() const { return m_IsMemoryReference; }

  Expression::SPType GetResultExpression() const { return m_spResExpr; }
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

  Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr) override;
  Expression::SPType VisitBind(BindExpression::SPType spBindExpr) override;
  Expression::SPType VisitCondition(ConditionExpression::SPType spCondExpr) override;
  Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr) override;
  Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr) override;
  Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr) override;
  Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr) override;
  Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spOpExpr) override;
  Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spOpExpr) override;
  Expression::SPType VisitBitVector(BitVectorExpression::SPType spIntExpr) override;
  Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr) override;
  Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr) override;
  Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr) override;
  Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr) override;
  Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr) override;
  Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr) override;

  SymbolicVisitor Fork() const;

  bool IsSymbolic() const { return m_IsSymbolic; }
  bool IsRelative() const { return m_IsRelative; }
  bool IsMemoryReference() const { return m_IsMemoryReference; }

  static Expression::SPType  RemoveExpressionAnnotations(Expression::SPType spExpr);
  Expression::SPType         GetValue(Expression::SPType spExpr) const;
  Expression::VSPType        FindExpressionsByKey(Expression::SPType spPatExpr) const;
  Expression::VSPType        FindExpressionsByValue(Expression::SPType spPatExpr) const;
  Expression::VSPType        FindExpressionsByUse(Expression::SPType spPatExpr) const;

  std::string ToString() const;
  bool BindExpression(Expression::SPType spKeyExpr, Expression::SPType spValueExpr, bool Propagate = false);
  bool UpdateAddress(Architecture& rArch, Address const& rAddr);

  using Updater = std::function<bool(Expression::SPType& rspExpr)>;
  bool UpdateExpression(Expression::SPType spKeyExpr, Updater updt);

  using DestinationPathCallbackType = std::function<void(Expression::SPType spDstExpr, Expression::VSPType spCondExprs)>;
  bool FindAllPaths(int& rNumOfPathFound, Architecture& rArch, DestinationPathCallbackType DstPathCb);

protected:
  void _InsertExpression(Expression::SPType spKeyExpr, Expression::SPType spValExpr);
  bool _RemoveExpression(Expression::SPType spKeyExpr);
  bool _EvaluateCondition(u8 CondOp, BitVectorExpression::SPType spConstRefExpr, BitVectorExpression::SPType spConstTestExpr, bool& rRes) const;

  using SymbolicContextType = std::vector<std::pair<Expression::SPType, Expression::SPType>>;
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
  Expression::SPType GetTrackedExpression(); // NOTE: caller take the ownership

  Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr) override;
  Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr) override;
  Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr) override;

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

  bool GetResult() const { return m_Result; }

  Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr) override;
  Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr) override;

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

  Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr) override;
  Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr) override;

protected:
  CpuInformation const& m_rCpuInfo;
  u8 m_Mode;
};

class MEDUSA_EXPORT IdentifierToVariable : public CloneVisitor
{
public:
  Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr) override;
  std::set<u32> const& GetUsedId() { return m_UsedId; }

private:
  std::set<u32> m_UsedId;
};

class MEDUSA_EXPORT SimplifyVisitor : public CloneVisitor
{
public:
  Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr) override;
  Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr) override;
  Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr) override;
  Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr) override;
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

  Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr) override;
  bool                       IsCommutative(BinaryOperationExpression::SPType spBinOpExpr);
  bool                       IsAssociative(Expression::SPType spBinOpExpr);
  Expression::SPType         GetOperand(BinaryOperationExpression::SPType spBinOpExpr, ConstantFoldingVisitor::Position Operand);
  Expression::SPType         SimplifyAssociativeOrCommutative(BinaryOperationExpression::SPType spBinOpExpr);
  Expression::SPType         SimplifyBinOp(OperationExpression::Type Operation, Expression::SPType spLeftExpr, Expression::SPType spRightExpr);

protected:
  Document const& m_rDoc;
  u8              m_Mode;
  Address         m_CurAddr;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXPRESSION_VISITOR_HPP
