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

// TODO: add tainted.{h,c}pp
namespace Taint
{
  class Medusa_EXPORT Context
  {
  public:
    void TaintId(u32 Id, Address const& rCurAddr);
    bool GetTaintAddress(u32 RegId, Address& rTaintedAddress);

  private:
    std::unordered_map<u32, Address> m_TaintedId;
  };

  typedef std::tuple<u32, Address> Id;

  class Medusa_EXPORT BackTrackContext
  {
  public:
    void TrackId(Taint::Id const& rId) { m_Ids.insert(rId); }
    void UntrackId(Taint::Id const& rId) { m_Ids.erase(rId); }
    bool IsTracked(Taint::Id const& rId) const { return m_Ids.find(rId) != std::end(m_Ids); }

  private:
    std::set<Taint::Id> m_Ids;
  };
}

// expression /////////////////////////////////////////////////////////////////

class Medusa_EXPORT Expression
{
public:
  virtual ~Expression(void) {}

  typedef std::list<Expression *> List;

  virtual std::string ToString(void) const = 0; // LATER: could be done using visitor
  virtual Expression *Clone(void) const = 0; // LATER: could be done using visitor
  virtual u32 GetSizeInBit(void) const = 0;
  virtual Expression* Visit(ExpressionVisitor *pVisitor) const = 0;
  virtual bool SignExtend(u32 NewSizeInBit) = 0;
};

// context expression /////////////////////////////////////////////////////////

class Medusa_EXPORT ContextExpression : public Expression
{
public:
  virtual ~ContextExpression(void) {}

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend = false) const = 0;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend = false) = 0;
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const = 0;
};

// expression visitor /////////////////////////////////////////////////////////

class Medusa_EXPORT ExpressionVisitor
{
public:
  virtual Expression* VisitSystem(std::string const& rName);
  virtual Expression* VisitBind(Expression::List const& rExprList);
  virtual Expression* VisitCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr);
  virtual Expression* VisitTernaryCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pTrueExpr, Expression const* pFalseExpr);
  virtual Expression* VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr);
  virtual Expression* VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr);
  virtual Expression* VisitWhileCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pBodyExpr);
  virtual Expression* VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr);
  virtual Expression* VisitConstant(u32 Type, u64 Value);
  virtual Expression* VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo);
  virtual Expression* VisitTaintedIdentifier(u32 Id, CpuInformation const* pCpuInfo, Address const& rCurAddr);
  virtual Expression* VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref);
  virtual Expression* VisitSymbolic(u32 Type, std::string const& rValue);
};

class Medusa_EXPORT ModifyIdVisitor : public ExpressionVisitor
{
public:
  ModifyIdVisitor(u32 Id) : m_Id(Id), m_Result(false), m_IsAssigned(false) {}
  virtual Expression* VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr);
  virtual Expression* VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo);
  virtual Expression* VisitTaintedIdentifier(u32 Id, CpuInformation const* pCpuInfo, Address const& rCurAddr);

  bool GetResult(void) const { return m_Result; }

protected:
  u32 m_Id;
  bool m_Result;
  bool m_IsAssigned;
};

class Medusa_EXPORT CloneVisitor : public ExpressionVisitor
{
public:
  virtual Expression* VisitSystem(std::string const& rName);
  virtual Expression* VisitBind(Expression::List const& rExprList);
  virtual Expression* VisitCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr);
  virtual Expression* VisitTernaryCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pTrueExpr, Expression const* pFalseExpr);
  virtual Expression* VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr);
  virtual Expression* VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr);
  virtual Expression* VisitWhileCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pBodyExpr);
  virtual Expression* VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr);
  virtual Expression* VisitConstant(u32 Type, u64 Value);
  virtual Expression* VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo);
  virtual Expression* VisitTaintedIdentifier(u32 Id, CpuInformation const* pCpuInfo, Address const& rCurAddr);
  virtual Expression* VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref);
  virtual Expression* VisitSymbolic(u32 Type, std::string const& rValue);
};

//! Visit an expression and convert IdentifierExpression to TaintedIdentifierExpression.
class Medusa_EXPORT TaintVisitor : public CloneVisitor
{
public:
  TaintVisitor(Address const& rCurAddr, Taint::Context& rCtxt);
  Expression* GetTaintedExpression(void); // NOTE: caller take the ownership

  virtual Expression* VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr);
  virtual Expression* VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo);
  virtual Expression* VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref);

private:
  Address m_CurAddr;
  Taint::Context& m_rCtxt;
  Expression* m_pTaintedExpr;
  bool m_IsAssigned;
};

class Medusa_EXPORT BackTrackVisitor : public ExpressionVisitor
{
public:
  BackTrackVisitor(Taint::BackTrackContext& rBtCtxt, Address const& rAddr, u32 Id)
    : m_rBtCtxt(rBtCtxt), m_Id(Id), m_Addr(rAddr)
    , m_IsAssigned(false), m_TrackSource(false), m_Result(false) {}

  bool GetResult(void) const { return m_Result; }

  virtual Expression* VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr);
  virtual Expression* VisitTaintedIdentifier(u32 Id, CpuInformation const* pCpuInfo, Address const& rCurAddr);

private:
  Taint::BackTrackContext& m_rBtCtxt;
  Address m_Addr;
  u32 m_Id;
  bool m_IsAssigned;
  bool m_TrackSource;
  bool m_Result;
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
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitSystem(m_Name); }
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

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitBind(m_Expressions); }
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

  ConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr);

  virtual ~ConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitCondition(m_Type, m_pRefExpr, m_pTestExpr); }
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  Type GetType(void) const { return m_Type; }
  Expression* GetReferenceExpression(void) const { return m_pRefExpr; }
  Expression* GetTestExpression(void) const { return m_pTestExpr; }

protected:
  Type m_Type;
  Expression *m_pRefExpr;
  Expression *m_pTestExpr;
};

class Medusa_EXPORT TernaryConditionExpression : public ConditionExpression
{
public:
  TernaryConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pTrueExpr, Expression *pFalseExpr);

  virtual ~TernaryConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitTernaryCondition(m_Type, m_pRefExpr, m_pTestExpr, m_pTrueExpr, m_pFalseExpr); }

  Expression* GetTrueExpression(void) const { return m_pTrueExpr; }
  Expression* GetFalseExpression(void) const { return m_pFalseExpr; }

protected:
  Type m_Type;
  Expression* m_pTrueExpr, *m_pFalseExpr;
};

class Medusa_EXPORT IfConditionExpression : public ConditionExpression
{
public:
  IfConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr);
  IfConditionExpression(ConditionExpression* pCondExpr, Expression *pThenExpr);
  virtual ~IfConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitIfCondition(m_Type, m_pRefExpr, m_pTestExpr, m_pThenExpr); }
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  Expression* GetThenExpression(void) const { return m_pThenExpr; }

protected:
  Expression *m_pThenExpr;
};

class Medusa_EXPORT IfElseConditionExpression : public IfConditionExpression
{
public:
  IfElseConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr, Expression *pElseExpr);
  IfElseConditionExpression(ConditionExpression* pCondExpr, Expression *pThenExpr, Expression *pElseExpr);
  virtual ~IfElseConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitIfElseCondition(m_Type, m_pRefExpr, m_pTestExpr, m_pThenExpr, m_pElseExpr); }
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  Expression* GetElseExpression(void) const { return m_pElseExpr; }

protected:
  Expression *m_pElseExpr;
};

class Medusa_EXPORT WhileConditionExpression : public ConditionExpression
{
public:
  WhileConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pBodyExpr);
  WhileConditionExpression(ConditionExpression* pCondExpr, Expression *pBodyExpr); // FIXME: memleak
  virtual ~WhileConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitWhileCondition(m_Type, m_pRefExpr, m_pTestExpr, m_pBodyExpr); }

  Expression* GetBodyExpression(void) const { return m_pBodyExpr; }

protected:
  Expression *m_pBodyExpr;
};

// operation expression ///////////////////////////////////////////////////////

class Medusa_EXPORT OperationExpression : public Expression
{
public:
  enum Type
  {
    OpUnk,
    OpAff,
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
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitOperation(m_OpType, m_pLeftExpr, m_pRightExpr); }
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  virtual u8 GetOperation(void) const { return m_OpType; }
  virtual Expression const* GetLeftExpression(void)  const { return m_pLeftExpr;  }
  virtual Expression const* GetRightExpression(void) const { return m_pRightExpr; }

private:
  u8          m_OpType;
  Expression *m_pLeftExpr;
  Expression *m_pRightExpr;
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
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitConstant(m_ConstType, m_Value); }
  virtual bool SignExtend(u32 NewSizeInBit);

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend = false) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend = false);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  u64          GetConstant(void) const { return m_Value; }

private:
  u32 m_ConstType;
  u64 m_Value;
};

// identifier expression //////////////////////////////////////////////////////

class Medusa_EXPORT IdentifierExpression : public ContextExpression
{
public:
  IdentifierExpression(u32 Id, CpuInformation const* pCpuInfo)
    : m_Id(Id), m_pCpuInfo(pCpuInfo) {}

  virtual ~IdentifierExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitIdentifier(m_Id, m_pCpuInfo); }
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue, bool SignExtend = false) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value, bool SignExtend = false);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  u32 GetId(void) const { return m_Id; }

protected:
  u32 m_Id;
  CpuInformation const* m_pCpuInfo;
};

class Medusa_EXPORT TaintedIdentifierExpression : public IdentifierExpression
{
public:
  TaintedIdentifierExpression(u32 Id, CpuInformation const* pCpuInfo, Address const& rCurAddr)
    : IdentifierExpression(Id, pCpuInfo), m_CurAddr(rCurAddr) {}

  virtual ~TaintedIdentifierExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitTaintedIdentifier(m_Id, m_pCpuInfo, m_CurAddr); }

private:
  Address m_CurAddr;
};

// memory expression //////////////////////////////////////////////////////////

class Medusa_EXPORT MemoryExpression : public ContextExpression
{
public:
  MemoryExpression(u32 AccessSize, Expression *pExprBase, Expression *pExprOffset, bool Dereference = true)
    : m_AccessSizeInBit(AccessSize), m_pExprBase(pExprBase), m_pExprOffset(pExprOffset), m_Dereference(Dereference)
  { assert(pExprOffset != nullptr); }

  virtual ~MemoryExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitMemory(m_AccessSizeInBit, m_pExprBase, m_pExprOffset, m_Dereference); }
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

  SymbolicExpression(Type SymType, std::string const& rValue)
    : m_Type(SymType), m_Value(rValue) {}

  virtual ~SymbolicExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression* Visit(ExpressionVisitor *pVisitor) const;
  virtual bool SignExtend(u32 NewSizeInBit);

  Type GetType(void) const { return m_Type; }
  std::string const& GetValue(void) const { return m_Value; }

private:
  Type m_Type;
  std::string m_Value;
};

// helper /////////////////////////////////////////////////////////////////////

namespace Expr
{
  Medusa_EXPORT Expression* MakeConst(u32 ConstType, u64 Value);
  Medusa_EXPORT Expression* MakeBoolean(bool Value);
  Medusa_EXPORT Expression* MakeId(u32 Id, CpuInformation const* pCpuInfo);
  Medusa_EXPORT Expression* MakeMem(u32 AccessSize, Expression *pExprBase, Expression *pExprOffset, bool Dereference = true);

  Medusa_EXPORT Expression* MakeCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr);
  Medusa_EXPORT Expression* MakeTernaryCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pTrueExpr, Expression *pFalseExpr);
  Medusa_EXPORT Expression* MakeIfCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr);
  Medusa_EXPORT Expression* MakeIfElseCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr, Expression *pElseExpr);
  Medusa_EXPORT Expression* MakeWhileCond(ConditionExpression::Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pBodyExpr);

  Medusa_EXPORT Expression* MakeOp(OperationExpression::Type OpType, Expression *pLeftExpr, Expression *pRightExpr);

  Medusa_EXPORT Expression* MakeBind(Expression::List const& rExprs);

  Medusa_EXPORT Expression* MakeSym(SymbolicExpression::Type SymType, std::string const& rValue);
}


MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EXPRESSION_HPP_
