#ifndef _MEDUSA_EXPRESSION_HPP_
#define _MEDUSA_EXPRESSION_HPP_

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

  private:
    std::set<Track::Id> m_Ids;
  };
}

// expression /////////////////////////////////////////////////////////////////

class Medusa_EXPORT Expression
{
public:
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

  Expression(Kind ExprKind) : m_Kind(ExprKind) {}
  virtual ~Expression(void) {}

  Kind GetKind(void) const { return m_Kind; }

  typedef std::list<Expression *> List;

  virtual std::string ToString(void) const = 0;
  virtual Expression *Clone(void) const = 0;
  virtual u32 GetSizeInBit(void) const = 0;
  virtual Expression* Visit(ExpressionVisitor* pVisitor) = 0;
  virtual bool SignExtend(u32 NewSizeInBit) = 0;

private:
  Kind m_Kind;
};

// context expression /////////////////////////////////////////////////////////

// TODO: get rid of this
class Medusa_EXPORT ContextExpression : public Expression
{
public:
  ContextExpression(Expression::Kind ExprKind) : Expression(ExprKind) {}
  virtual ~ContextExpression(void) {}

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend = false) const = 0;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend = false) = 0;
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const = 0;
};

// system expression //////////////////////////////////////////////////////////

class Medusa_EXPORT SystemExpression : public Expression
{
public:
  SystemExpression(std::string const& rExprName);
  virtual ~SystemExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

protected:
  std::string m_Name;
};

// bind expression ////////////////////////////////////////////////////////////

class Medusa_EXPORT BindExpression : public Expression
{
public:
  BindExpression(Expression::List const& rExprs);
  virtual ~BindExpression(void);

  Expression::List& GetBoundExpressions(void) { return m_Expressions; }

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

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

  ConditionExpression(Expression::Kind ExprKind, Type CondType, Expression *pRefExpr, Expression *pTestExpr);

  virtual ~ConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const { return nullptr; }
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  Type GetType(void) const { return m_Type; }
  Expression* GetReferenceExpression(void) const { return m_pRefExpr; }
  Expression* GetTestExpression(void) const { return m_pTestExpr; }

protected:
  Type m_Type;
  Expression* m_pRefExpr;
  Expression* m_pTestExpr;
};

class Medusa_EXPORT TernaryConditionExpression : public ConditionExpression
{
public:
  TernaryConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pTrueExpr, Expression *pFalseExpr);

  virtual ~TernaryConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual Expression* Visit(ExpressionVisitor* pVisitor);

  Expression* GetTrueExpression(void) { return m_pTrueExpr; }
  Expression* GetFalseExpression(void) { return m_pFalseExpr; }

protected:
  Type m_Type;
  Expression* m_pTrueExpr, *m_pFalseExpr;
};

class Medusa_EXPORT IfElseConditionExpression : public ConditionExpression
{
public:
  IfElseConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr, Expression *pElseExpr);
  virtual ~IfElseConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  Expression* GetThenExpression(void) { return m_pThenExpr; }
  Expression* GetElseExpression(void) { return m_pElseExpr; }

protected:
  Expression* m_pThenExpr;
  Expression* m_pElseExpr;
};

class Medusa_EXPORT WhileConditionExpression : public ConditionExpression
{
public:
  WhileConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pBodyExpr);
  virtual ~WhileConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor);

  Expression* GetBodyExpression(void) { return m_pBodyExpr; }

protected:
  Expression* m_pBodyExpr;
};

// operation expression ///////////////////////////////////////////////////////

class Medusa_EXPORT AssignmentExpression : public Expression
{
public:
  //! pLeftExpr and pRightExpr must be allocated by standard new
  AssignmentExpression(Expression *pLeftExpr, Expression *pRightExpr);
  virtual ~AssignmentExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  virtual Expression* GetLeftExpression(void)  { return m_pLeftExpr; }
  virtual Expression* GetRightExpression(void) { return m_pRightExpr; }

private:
  Expression* m_pLeftExpr;
  Expression* m_pRightExpr;
};

class Medusa_EXPORT OperationExpression : public Expression
{
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
  OperationExpression(Type OpType, Expression *pLeftExpr, Expression *pRightExpr);
  virtual ~OperationExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  virtual u8 GetOperation(void) const { return m_OpType; }
  virtual Expression* GetLeftExpression(void)  { return m_pLeftExpr;  }
  virtual Expression* GetRightExpression(void) { return m_pRightExpr; }

private:
  u8          m_OpType;
  Expression* m_pLeftExpr;
  Expression* m_pRightExpr;
};

// constant expression ////////////////////////////////////////////////////////

class Medusa_EXPORT ConstantExpression : public ContextExpression
{
public:
  enum Type
  {
    ConstUnknownBit = 0,
    Const1Bit       = 1,
    Const8Bit       = 8,
    Const16Bit      = 16,
    Const32Bit      = 32,
    Const64Bit      = 64,
    //Const128Bit,
    //Const256Bit,
    //Const512Bit,
    ConstSigned     = 0x80000000
  };

  ConstantExpression(u32 ConstType, u64 Value);
  virtual ~ConstantExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return m_ConstType; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit);

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

class Medusa_EXPORT IdentifierExpression : public ContextExpression
{
public:
  IdentifierExpression(u32 Id, CpuInformation const* pCpuInfo);

  virtual ~IdentifierExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression* Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

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
public:
  TrackedIdentifierExpression(u32 Id, CpuInformation const* pCpuInfo, Address const& rCurAddr);

  virtual ~TrackedIdentifierExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression* Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  u32 GetId(void) const { return m_Id; }
  CpuInformation const* GetCpuInformation(void) const { return m_pCpuInfo; }
  Address GetCurrentAddress(void) const { return m_CurAddr; }

private:
  u32 m_Id;
  CpuInformation const* m_pCpuInfo;
  Address m_CurAddr;
};

// memory expression //////////////////////////////////////////////////////////

class Medusa_EXPORT MemoryExpression : public ContextExpression
{
public:
  MemoryExpression(u32 AccessSize, Expression *pExprBase, Expression *pExprOffset, bool Dereference = true);

  virtual ~MemoryExpression(void);

  u32 GetAccessSizeInBit(void) const { return m_AccessSizeInBit; }
  Expression* GetBaseExpression(void) { return m_pExprBase; }
  Expression* GetOffsetExpression(void) { return m_pExprOffset; }

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression* Visit(ExpressionVisitor* pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend = false) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend = false);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  Expression* GetAddressExpression(void) const { return m_pExprOffset; }
  bool IsDereferencable(void) const { return m_Dereference; }

private:
  u32         m_AccessSizeInBit;
  Expression *m_pExprBase;
  Expression *m_pExprOffset;
  bool        m_Dereference;
};

// symbolic expression ////////////////////////////////////////////////////////

class Medusa_EXPORT SymbolicExpression : public Expression
{
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
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression* Visit(ExpressionVisitor *pVisitor);
  virtual bool SignExtend(u32 NewSizeInBit);

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
  virtual Expression* VisitSystem(SystemExpression* pSysExpr);
  virtual Expression* VisitBind(BindExpression* pBindExpr);
  virtual Expression* VisitTernaryCondition(TernaryConditionExpression* pTernExpr);
  virtual Expression* VisitIfElseCondition(IfElseConditionExpression* pIfElseExpr);
  virtual Expression* VisitWhileCondition(WhileConditionExpression* pWhileExpr);
  virtual Expression* VisitAssignment(AssignmentExpression* pAssignExpr);
  virtual Expression* VisitOperation(OperationExpression* pOpExpr);
  virtual Expression* VisitConstant(ConstantExpression* pConstExpr);
  virtual Expression* VisitIdentifier(IdentifierExpression* pIdExpr);
  virtual Expression* VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr);
  virtual Expression* VisitMemory(MemoryExpression* pMemExpr);
  virtual Expression* VisitSymbolic(SymbolicExpression* pSymExpr);
};

class Medusa_EXPORT CloneVisitor : public ExpressionVisitor
{
public:
  virtual Expression* VisitSystem(SystemExpression* pSysExpr);
  virtual Expression* VisitBind(BindExpression* pBindExpr);
  virtual Expression* VisitTernaryCondition(TernaryConditionExpression* pTernExpr);
  virtual Expression* VisitIfElseCondition(IfElseConditionExpression* pIfElseExpr);
  virtual Expression* VisitWhileCondition(WhileConditionExpression* pWhileExpr);
  virtual Expression* VisitAssignment(AssignmentExpression* pAssignExpr);
  virtual Expression* VisitOperation(OperationExpression* pOpExpr);
  virtual Expression* VisitConstant(ConstantExpression* pConstExpr);
  virtual Expression* VisitIdentifier(IdentifierExpression* pIdExpr);
  virtual Expression* VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr);
  virtual Expression* VisitMemory(MemoryExpression* pMemExpr);
  virtual Expression* VisitSymbolic(SymbolicExpression* pSymExpr);
};

//! Visit an expression and convert IdentifierExpression to TrackedIdentifierExpression.
class Medusa_EXPORT TrackVisitor : public CloneVisitor
{
public:
  TrackVisitor(Address const& rCurAddr, Track::Context& rCtxt);
  Expression* GetTrackedExpression(void); // NOTE: caller take the ownership

  virtual Expression* VisitAssignment(AssignmentExpression* pAssignExpr);
  virtual Expression* VisitOperation(OperationExpression* pOpExpr);
  virtual Expression* VisitIdentifier(IdentifierExpression* pIdExpr);
  virtual Expression* VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr);

private:
  Address m_CurAddr;
  Track::Context& m_rCtxt;
  Expression* m_pTrackedExpr;
  bool m_IsAssigned;
};

class Medusa_EXPORT BackTrackVisitor : public ExpressionVisitor
{
public:
  BackTrackVisitor(Track::BackTrackContext& rBtCtxt, Address const& rAddr, u32 Id)
    : m_rBtCtxt(rBtCtxt), m_Id(Id), m_Addr(rAddr)
    , m_IsAssigned(false), m_TrackSource(false), m_Result(false) {}

  bool GetResult(void) const { return m_Result; }

  virtual Expression* VisitAssignment(AssignmentExpression* pAssignExpr);
  virtual Expression* VisitIdentifier(IdentifierExpression* pIdExpr);
  virtual Expression* VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr);

private:
  Track::BackTrackContext& m_rBtCtxt;
  Address m_Addr;
  u32 m_Id;
  bool m_IsAssigned;
  bool m_TrackSource;
  bool m_Result;
};

// expression matcher /////////////////////////////////////////////////////////

namespace ExprMatcher
{
  class Medusa_EXPORT Node
  {
  public:
    virtual bool operator()(Expression* pExpr) const = 0;
  };

  class Medusa_EXPORT Not : public Node
  {
  public:
    Not(Node const& rNode) : m_rNode(rNode) {}

    virtual bool operator()(Expression* pExpr) const;

  private:
    Node const& m_rNode;
  };

  class Medusa_EXPORT And : public Node
  {
  public:
    And(Node const& rLeft, Node const& rRight) : m_rLeft(rLeft), m_rRight(rRight) {}

    virtual bool operator()(Expression* pExpr) const;

  private:
    Node const& m_rLeft;
    Node const& m_rRight;
  };

  class Medusa_EXPORT Or : public Node
  {
  public:
    Or(Node const& rLeft, Node const& rRight) : m_rLeft(rLeft), m_rRight(rRight) {}

    virtual bool operator()(Expression* pExpr) const;

  private:
    Node const& m_rLeft;
    Node const& m_rRight;
  };

  class Medusa_EXPORT TypeIs : public Node
  {
  public:
    TypeIs(Expression::Kind ExprKind) : m_ExprKind(ExprKind) {}

    virtual bool operator()(Expression* pExpr) const;

  private:
    Expression::Kind m_ExprKind;
  };

  class Medusa_EXPORT IdIs : public Node
  {
  public:
    IdIs(u32 Id) : m_Id(Id) {}

    virtual bool operator()(Expression* pExpr) const;

  private:
    u32 m_Id;
  };

  class Medusa_EXPORT OpIs : public Node
  {
  public:
    OpIs(OperationExpression::Kind OpType) : m_OpType(OpType) {}

    virtual bool operator()(Expression* pExpr) const;

  private:
    OperationExpression::Kind m_OpType;
  };

  class Medusa_EXPORT OpLeftIs : public Node
  {
  public:
    OpLeftIs(Node const& rNode) : m_rNode(rNode) {}

    virtual bool operator()(Expression* pExpr) const;

  private:
    Node const& m_rNode;
  };
}

ExprMatcher::Node&& operator!(ExprMatcher::Node const& rNode);
ExprMatcher::Node&& operator&(ExprMatcher::Node const& rLeft, ExprMatcher::Node const& rRight);
ExprMatcher::Node&& operator|(ExprMatcher::Node const& rLeft, ExprMatcher::Node const& rRight);

class Medusa_EXPORT ExpressionMatcher
{
public:
  ExpressionMatcher(ExprMatcher::Node const& rMatcher) : m_rMatcher(rMatcher) {}

  bool Test(Expression* pExpr) const;
  bool Test(Expression::List const& rExprs) const;
  Expression::List Filter(std::list<Expression*> const& rExprs) const;

private:
  ExprMatcher::Node const& m_rMatcher;
};

// helper /////////////////////////////////////////////////////////////////////

namespace Expr
{
  Medusa_EXPORT Expression* MakeConst(u32 ConstType, u64 Value);
  Medusa_EXPORT Expression* MakeBoolean(bool Value);
  Medusa_EXPORT Expression* MakeId(u32 Id, CpuInformation const* pCpuInfo);
  Medusa_EXPORT Expression* MakeMem(u32 AccessSize, Expression *pExprBase, Expression *pExprOffset, bool Dereference = true);

  Medusa_EXPORT Expression* MakeTernaryCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pTrueExpr, Expression *pFalseExpr);
  Medusa_EXPORT Expression* MakeIfElseCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr, Expression *pElseExpr);
  Medusa_EXPORT Expression* MakeWhileCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pBodyExpr);

  Medusa_EXPORT Expression* MakeAssign(Expression *pLeftExpr, Expression *pRightExpr);
  Medusa_EXPORT Expression* MakeOp(OperationExpression::Type OpType, Expression *pLeftExpr, Expression *pRightExpr);

  Medusa_EXPORT Expression* MakeBind(Expression::List const& rExprs);

  Medusa_EXPORT Expression* MakeSym(SymbolicExpression::Type SymType, std::string const& rValue);
}


MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EXPRESSION_HPP_
