#ifndef MEDUSA_EXPRESSION_HPP_HPP
#define MEDUSA_EXPRESSION_HPP_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/information.hpp"
#include "medusa/context.hpp"

#include <list>

MEDUSA_NAMESPACE_BEGIN

class ExpressionVisitor;

// TODO: add Track.{h,c}pp
namespace Track
{
  class Medusa_EXPORT Context
  {
  public:
    void TrackId(u32 Id, Address const& rCurAddr);
    bool GetTrackAddress(u32 RegId, Address& rTrackedAddress);
    bool IsEmpty(void) const { return m_TrackedId.empty(); }

  private:
    std::unordered_map<u32, Address> m_TrackedId;
  };

  typedef std::tuple<u32, Address> Id;

  class Medusa_EXPORT BackTrackContext
  {
  public:
    void TrackId(Track::Id const& rId) { m_Ids.insert(rId); }
    void UntrackId(Track::Id const& rId) { m_Ids.erase(rId); }
    bool IsTracked(Track::Id const& rId) const { return m_Ids.find(rId) != std::end(m_Ids); }
    bool IsEmpty(void) const { return m_Ids.empty(); }

  private:
    std::set<Track::Id> m_Ids;
  };
}

// expression /////////////////////////////////////////////////////////////////

class Medusa_EXPORT Expression : public std::enable_shared_from_this<Expression>
{
public:
  typedef std::shared_ptr<Expression> SPType;
  typedef std::list<Expression::SPType> List;
  typedef std::vector<Expression::SPType> VSPType;

  enum Kind
  {
    Unknown,
    Bind,
    Cond,
    TernaryCond,
    IfElseCond,
    WhileCond,
    Assign,
    Op,
    Const,
    Id,
    TrackedId,
    Mem,
    Sym,
  };

  virtual ~Expression(void) {}

  virtual std::string ToString(void) const = 0;
  virtual Expression::SPType Clone(void) const = 0;
  virtual u32 GetSizeInBit(void) const = 0;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor) = 0;
  virtual bool SignExtend(u32 NewSizeInBit) = 0;
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) = 0;

  virtual Kind GetClassKind(void) const { return Unknown; }
  static Kind GetStaticClassKind(void) { return Unknown; }
  virtual bool IsKindOf(Kind ExprKind) const { return ExprKind == Unknown; }

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend = false) const { return false; }
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend = false) { return false; }
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const { return false; }

  template <typename T>
  bool IsClassOf(void) const { return IsKindOf(T::GetStaticClassKind()); }
};

template <typename T>
typename T::SPType expr_cast(Expression::SPType spExpr)
{
  if (spExpr == nullptr)
    return nullptr;
  if (!spExpr->IsClassOf<T>())
    return nullptr;
  return std::static_pointer_cast<T>(spExpr);
}

#define DECL_EXPR(EXPR_NAME, EXPR_KIND, BASE_EXPR_NAME)\
  public:\
  typedef std::shared_ptr<EXPR_NAME> SPType;\
  virtual Kind GetClassKind(void) const { return EXPR_KIND; }\
  static  Kind GetStaticClassKind(void) { return EXPR_KIND; }\
  virtual bool IsKindOf(Kind ExprKind) const { return ExprKind == EXPR_KIND || BASE_EXPR_NAME::IsKindOf(ExprKind); }

// system expression //////////////////////////////////////////////////////////

class Medusa_EXPORT SystemExpression : public Expression
{
public:
  SystemExpression(std::string const& rExprName);
  virtual ~SystemExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }

protected:
  std::string m_Name;
};

// bind expression ////////////////////////////////////////////////////////////

class Medusa_EXPORT BindExpression : public Expression
{
  DECL_EXPR(BindExpression, Expression::Bind, Expression)

public:
  BindExpression(Expression::List const& rExprs);
  virtual ~BindExpression(void);

  Expression::List& GetBoundExpressions(void) { return m_Expressions; }

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);

private:
  Expression::List m_Expressions;
};

// condition expression ///////////////////////////////////////////////////////

class Medusa_EXPORT ConditionExpression : public Expression
{
public:
  enum Type
  {
    CondUnk,
    CondEq,
    CondNe,
    CondUgt,
    CondUge,
    CondUlt,
    CondUle,
    CondSgt,
    CondSge,
    CondSlt,
    CondSle
  };

  ConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr);

  virtual ~ConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const { return nullptr; }
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }

  Type GetType(void) const { return m_Type; }
  Expression::SPType GetReferenceExpression(void) const { return m_spRefExpr; }
  Expression::SPType GetTestExpression(void) const { return m_spTestExpr; }

protected:
  Type m_Type;
  Expression::SPType m_spRefExpr;
  Expression::SPType m_spTestExpr;
};

class Medusa_EXPORT TernaryConditionExpression : public ConditionExpression
{
  DECL_EXPR(TernaryConditionExpression, Expression::TernaryCond, Expression)

public:
  TernaryConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spTrueExpr, Expression::SPType spFalseExpr);

  virtual ~TernaryConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);

  Expression::SPType GetTrueExpression(void) { return m_spTrueExpr; }
  Expression::SPType GetFalseExpression(void) { return m_spFalseExpr; }

protected:
  Type m_Type;
  Expression::SPType m_spTrueExpr;
  Expression::SPType m_spFalseExpr;
};

class Medusa_EXPORT IfElseConditionExpression : public ConditionExpression
{
  DECL_EXPR(IfElseConditionExpression, Expression::IfElseCond, Expression)

public:
  IfElseConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spThenExpr, Expression::SPType spElseExpr);
  virtual ~IfElseConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);

  Expression::SPType GetThenExpression(void) { return m_spThenExpr; }
  Expression::SPType GetElseExpression(void) { return m_spElseExpr; }

protected:
  Expression::SPType m_spThenExpr;
  Expression::SPType m_spElseExpr;
};

class Medusa_EXPORT WhileConditionExpression : public ConditionExpression
{
  DECL_EXPR(WhileConditionExpression, Expression::WhileCond, Expression)

public:
  WhileConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spBodyExpr);
  virtual ~WhileConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);

  Expression::SPType GetBodyExpression(void) { return m_spBodyExpr; }

protected:
  Expression::SPType m_spBodyExpr;
};

// operation expression ///////////////////////////////////////////////////////

class Medusa_EXPORT AssignmentExpression : public Expression
{
  DECL_EXPR(AssignmentExpression, Expression::Assign, Expression)

public:
  //! pLeftExpr and pRightExpr must be allocated by standard new
  AssignmentExpression(Expression::SPType spDstExpr, Expression::SPType spSrcExpr);
  virtual ~AssignmentExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);

  virtual Expression::SPType GetDestinationExpression(void) { return m_spDstExpr; }
  virtual Expression::SPType GetSourceExpression(void) { return m_spSrcExpr; }

private:
  Expression::SPType m_spDstExpr;
  Expression::SPType m_spSrcExpr;
};

class Medusa_EXPORT OperationExpression : public Expression
{
  DECL_EXPR(OperationExpression, Expression::Op, Expression)

public:
  enum Type
  {
    OpUnk,
    OpXchg,
    OpAnd,
    OpOr,
    OpXor,
    OpLls, /* Logical Left Shift */
    OpLrs, /* Logical Right Shift */
    OpArs, /* Arithmetic Right Shift */
    OpAdd,
    OpSub,
    OpMul,
    OpSDiv,
    OpUDiv,
    OpSext /* Sign Extend */
  };

  //! pLeftExpr and pRightExpr must be allocated by standard new
  OperationExpression(Type OpType, Expression::SPType spLeftExpr, Expression::SPType spRightExpr);
  virtual ~OperationExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);

  u8 GetOperation(void) const { return m_OpType; }
  u8 GetOppositeOperation(void) const;

  Expression::SPType GetLeftExpression(void)  { return m_spLeftExpr; }
  Expression::SPType GetRightExpression(void) { return m_spRightExpr; }

  void SwapExpressions(void) { std::swap(m_spLeftExpr, m_spRightExpr); }
  void SwapLeftExpression(Expression::SPType& rspExpr) { m_spLeftExpr.swap(rspExpr); }
  void SwapRightExpression(Expression::SPType& rspExpr) { m_spRightExpr.swap(rspExpr); }
  void SwapLeftExpressions(OperationExpression::SPType spOpExpr);


private:
  u8 m_OpType;
  Expression::SPType m_spLeftExpr;
  Expression::SPType m_spRightExpr;
};

// constant expression ////////////////////////////////////////////////////////

class Medusa_EXPORT ConstantExpression : public Expression
{
  DECL_EXPR(ConstantExpression, Expression::Const, Expression)

public:
  enum Type
  {
    ConstUnknownBit = 0,
    Const1Bit = 1,
    Const8Bit = 8,
    Const16Bit = 16,
    Const32Bit = 32,
    Const64Bit = 64,
    //Const128Bit,
    //Const256Bit,
    //Const512Bit,
    ConstSigned = 0x80000000
  };

  ConstantExpression(u32 ConstType, u64 Value);
  virtual ~ConstantExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return m_ConstType; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend = false) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend = false);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  Type         GetType(void) const { return static_cast<Type>(m_ConstType); }
  u64          GetConstant(void) const { return m_Value; }

private:
  u32 m_ConstType;
  u64 m_Value;
};

// identifier expression //////////////////////////////////////////////////////

class Medusa_EXPORT IdentifierExpression : public Expression
{
  DECL_EXPR(IdentifierExpression, Expression::Id, Expression)

public:
  IdentifierExpression(u32 Id, CpuInformation const* pCpuInfo);

  virtual ~IdentifierExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend = false) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend = false);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  u32 GetId(void) const { return m_Id; }
  CpuInformation const* GetCpuInformation(void) const { return m_pCpuInfo; }

protected:
  u32 m_Id;
  CpuInformation const* m_pCpuInfo;
};

class Medusa_EXPORT TrackedIdentifierExpression : public Expression
{
  DECL_EXPR(TrackedIdentifierExpression, Expression::TrackedId, Expression)

public:
  TrackedIdentifierExpression(u32 Id, CpuInformation const* pCpuInfo, Address const& rCurAddr);

  virtual ~TrackedIdentifierExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }

  u32 GetId(void) const { return m_Id; }
  CpuInformation const* GetCpuInformation(void) const { return m_pCpuInfo; }
  Address GetCurrentAddress(void) const { return m_CurAddr; }

private:
  u32 m_Id;
  CpuInformation const* m_pCpuInfo;
  Address m_CurAddr;
};

// memory expression //////////////////////////////////////////////////////////

class Medusa_EXPORT MemoryExpression : public Expression
{
  DECL_EXPR(MemoryExpression, Expression::Mem, Expression)

public:
  MemoryExpression(u32 AccessSize, Expression::SPType spExprBase, Expression::SPType spExprOffset, bool Dereference = true);

  virtual ~MemoryExpression(void);

  u32 GetAccessSizeInBit(void) const { return m_AccessSizeInBit; }
  Expression::SPType GetBaseExpression(void) { return m_spBaseExpr; }
  Expression::SPType GetOffsetExpression(void) { return m_spOffExpr; }

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend = false) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend = false);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  Expression::SPType GetAddressExpression(void) const { return m_spOffExpr; }
  bool IsDereferencable(void) const { return m_Dereference; }

private:
  u32 m_AccessSizeInBit;
  Expression::SPType m_spBaseExpr;
  Expression::SPType m_spOffExpr;
  bool m_Dereference;
};

// symbolic expression ////////////////////////////////////////////////////////

class Medusa_EXPORT SymbolicExpression : public Expression
{
  DECL_EXPR(SymbolicExpression, Expression::Sym, Expression)

public:
  enum Type
  {
    Unknown,
    ReturnedValue,
    FromParameter,
    Undefined,
  };

  SymbolicExpression(Type SymType, std::string const& rValue);

  virtual ~SymbolicExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor *pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }

  Type GetType(void) const { return m_Type; }
  std::string const& GetValue(void) const { return m_Value; }

private:
  Type m_Type;
  std::string m_Value;
};

// expression visitor /////////////////////////////////////////////////////////

class Medusa_EXPORT ExpressionVisitor
{
public:
  virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
  virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
  virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitOperation(OperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitConstant(ConstantExpression::SPType spConstExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr);
  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
  virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);
};

class Medusa_EXPORT CloneVisitor : public ExpressionVisitor
{
public:
  virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
  virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
  virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitOperation(OperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitConstant(ConstantExpression::SPType spConstExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr);
  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
  virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);
};

class Medusa_EXPORT FilterVisitor : public ExpressionVisitor
{
public:
  typedef std::function <Expression::SPType(Expression::SPType spExpr)> Matcher;
  FilterVisitor(Matcher ExprMatcher, size_t NrOfResult = 0) : m_ExprMatcher(ExprMatcher), m_NbrOfResult(NrOfResult) {}

  virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
  virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
  virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitOperation(OperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitConstant(ConstantExpression::SPType spConstExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr);
  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
  virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);

  Expression::List GetMatchedExpressions(void) { return m_MatchedExprs; }

protected:
  void _Evaluate(Expression::SPType spExpr);
  void _AddExpression(Expression::SPType spExpr);
  bool _IsDone(void) const;

  Matcher m_ExprMatcher;
  size_t m_NbrOfResult;
  Expression::List m_MatchedExprs;
};

class Medusa_EXPORT EvaluateVisitor : public ExpressionVisitor
{
public:
  // LATER: It'd be better to use a context filled by the architecture itself in order to correctly
  // map the PC pointer. On some architecture the PC is set on multiple register (e.g.: x86 cs:offset,
  // z80/gb bank:offset, ...).
  EvaluateVisitor(Document const& rDoc, u32 PcBaseId, u32 PcOffId, Address const& rCurAddr);

  virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
  virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
  virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitOperation(OperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitConstant(ConstantExpression::SPType spConstExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr);
  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
  virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);

  bool IsSymbolic(void) const { return m_IsSymbolic; }
  Expression::SPType GetResultExpression(void) const { return m_spResExpr; }

protected:
  Document const& m_rDoc;
  u32 m_PcBaseId;
  u32 m_PcOffId;
  Address const& m_rCurAddr;
  bool m_IsSymbolic;
  Expression::SPType m_spResExpr;
};

//! Visit an expression and convert IdentifierExpression to TrackedIdentifierExpression.
class Medusa_EXPORT TrackVisitor : public CloneVisitor
{
public:
  TrackVisitor(Address const& rCurAddr, Track::Context& rCtxt);
  Expression::SPType GetTrackedExpression(void); // NOTE: caller take the ownership

  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr);

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
class Medusa_EXPORT BackTrackVisitor : public ExpressionVisitor
{
public:
  BackTrackVisitor(Track::BackTrackContext& rBtCtxt)
    : m_rBtCtxt(rBtCtxt)
    , m_IsAssigned(false), m_TrackSource(false), m_Result(false) {}

  bool GetResult(void) const { return m_Result; }

  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr);

private:
  Track::BackTrackContext& m_rBtCtxt;
  bool m_IsAssigned;
  bool m_TrackSource;
  bool m_Result;
};

// expression simplifier //////////////////////////////////////////////////////

class Medusa_EXPORT ExpressionSimplifier
{
public:
  ExpressionSimplifier(void) : m_IsDone(false) {}

  bool Execute(void);

protected:
  virtual bool _RunOnce(void) = 0;
  virtual bool _Finalize(void) = 0;

  bool m_IsDone;
};

class Medusa_EXPORT TrackedIdPropagation : public ExpressionSimplifier
{
public:
  TrackedIdPropagation(Expression::List& rExprs, u32 Id);

protected:
  Expression::List& m_rExprs;
  AssignmentExpression::SPType m_spResExpr;

  virtual bool _RunOnce(void);
  virtual bool _Finalize(void);

private:
  Expression::SPType __FindTrackedIdExpression(u32 Id, Address const& rAddr);
};

//https://english.stackexchange.com/questions/35860/normalization-vs-canonicalization
class Medusa_EXPORT NormalizeExpression : public ExpressionSimplifier
{
public:
  NormalizeExpression(Expression::SPType spExpr);

  virtual bool _RunOnce(void);
  virtual bool _Finalize(void);

protected:
  Expression::SPType m_spExpr;
};

class Medusa_EXPORT ConstantPropagation : public ExpressionSimplifier
{
public:
  ConstantPropagation(Expression::SPType spExpr);

  virtual bool _RunOnce(void);
  virtual bool _Finalize(void);

protected:
  Expression::SPType m_spExpr;
};

// helper /////////////////////////////////////////////////////////////////////

namespace Expr
{
  Medusa_EXPORT Expression::SPType MakeConst(u32 ConstType, u64 Value);
  Medusa_EXPORT Expression::SPType MakeBoolean(bool Value);
  Medusa_EXPORT Expression::SPType MakeId(u32 Id, CpuInformation const* pCpuInfo);
  Medusa_EXPORT Expression::SPType MakeMem(u32 AccessSize, Expression::SPType spExprBase, Expression::SPType spExprOffset, bool Dereference = true);

  Medusa_EXPORT Expression::SPType MakeTernaryCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spTrueExpr, Expression::SPType spFalseExpr);
  Medusa_EXPORT Expression::SPType MakeIfElseCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spThenExpr, Expression::SPType spElseExpr);
  Medusa_EXPORT Expression::SPType MakeWhileCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spBodyExpr);

  Medusa_EXPORT Expression::SPType MakeAssign(Expression::SPType spDstExpr, Expression::SPType spSrcExpr);
  Medusa_EXPORT Expression::SPType MakeOp(OperationExpression::Type OpType, Expression::SPType spLeftExpr, Expression::SPType spRightExpr);

  Medusa_EXPORT Expression::SPType MakeBind(Expression::List const& rExprs);

  Medusa_EXPORT Expression::SPType MakeSym(SymbolicExpression::Type SymType, std::string const& rValue);
}


MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXPRESSION_HPP_HPP
