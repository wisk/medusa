#ifndef _MEDUSA_EXPRESSION_HPP_
#define _MEDUSA_EXPRESSION_HPP_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/cpu.hpp"

#include <list>

MEDUSA_NAMESPACE_BEGIN

class ExpressionVisitor;

class Medusa_EXPORT Expression
{
public:
  virtual ~Expression(void) {}

  typedef std::list<Expression *> List;
  virtual std::string ToString(void) const = 0;
  virtual Expression *Clone(void) const = 0;
  virtual u32 GetSizeInBit(void) const = 0;
  virtual Expression* Visit(ExpressionVisitor *pVisitor) const = 0;
};

class Medusa_EXPORT ContextExpression : public Expression
{
public:
  virtual ~ContextExpression(void) {}

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue) const = 0;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value) = 0;
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const = 0;
};

class Medusa_EXPORT ExpressionVisitor
{
public:
  virtual Expression* VisitBind(Expression::List const& rExprList) = 0;
  virtual Expression* VisitCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr) = 0;
  virtual Expression* VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr) = 0;
  virtual Expression* VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr) = 0;
  virtual Expression* VisitWhileCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pBodyExpr) = 0;
  virtual Expression* VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr) = 0;
  virtual Expression* VisitConstant(u32 Type, u64 Value) = 0;
  virtual Expression* VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo) = 0;
  virtual Expression* VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref) = 0;
};

class Medusa_EXPORT BindExpression : public Expression
{
public:
  BindExpression(Expression::List const& rExprs)
    : m_Expressions(rExprs) {}

  virtual ~BindExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitBind(m_Expressions); }

private:
  Expression::List m_Expressions;
};

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

  ConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr)
    : m_Type(CondType), m_pRefExpr(pRefExpr), m_pTestExpr(pTestExpr) {}

  virtual ~ConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitCondition(m_Type, m_pRefExpr, m_pTestExpr); }

protected:
  Type m_Type;
  Expression *m_pRefExpr;
  Expression *m_pTestExpr;
};

class Medusa_EXPORT IfConditionExpression : public ConditionExpression
{
public:
  IfConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr)
    : ConditionExpression(CondType, pRefExpr, pTestExpr), m_pThenExpr(pThenExpr) {}

  virtual ~IfConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitIfCondition(m_Type, m_pRefExpr, m_pTestExpr, m_pThenExpr); }

protected:
  Expression *m_pThenExpr;
};

class Medusa_EXPORT IfElseConditionExpression : public IfConditionExpression
{
public:
  IfElseConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pThenExpr, Expression *pElseExpr)
    : IfConditionExpression(CondType, pRefExpr, pTestExpr, pThenExpr), m_pElseExpr(pElseExpr) {}

  virtual ~IfElseConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitIfElseCondition(m_Type, m_pRefExpr, m_pTestExpr, m_pThenExpr, m_pElseExpr); }

protected:
  Expression *m_pElseExpr;
};

class Medusa_EXPORT WhileConditionExpression : public ConditionExpression
{
public:
  WhileConditionExpression(Type CondType, Expression *pRefExpr, Expression *pTestExpr, Expression *pBodyExpr)
    : ConditionExpression(CondType, pRefExpr, pTestExpr), m_pBodyExpr(pBodyExpr) {}

  virtual ~WhileConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitWhileCondition(m_Type, m_pRefExpr, m_pTestExpr, m_pBodyExpr); }

protected:
  Expression *m_pBodyExpr;
};

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
    OpDiv,
    OpSext /* Sign Extend */
  };

  //! pLeftExpr and pRightExpr must be allocated by standard new
  OperationExpression(Type OpType, Expression *pLeftExpr, Expression *pRightExpr)
    : m_OpType(OpType), m_pLeftExpr(pLeftExpr), m_pRightExpr(pRightExpr) {}

  virtual ~OperationExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitOperation(m_OpType, m_pLeftExpr, m_pRightExpr); }

private:
  u8          m_OpType;
  Expression *m_pLeftExpr;
  Expression *m_pRightExpr;
};

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

  ConstantExpression(u32 ConstType, u64 Value)
    : m_ConstType(ConstType), m_Value(
        ConstType == ConstUnknownBit ||
        ConstType == Const64Bit ?
        Value : (Value & ((1ULL << m_ConstType) - 1))) {}

  virtual ~ConstantExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return m_ConstType; }
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitConstant(m_ConstType, m_Value); }

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

private:
  u32 m_ConstType;
  u64 m_Value;
};

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

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

private:
  u32 m_Id;
  CpuInformation const* m_pCpuInfo;
};

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

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64& rValue) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, u64 Value);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  Expression* GetAddressExpression(void) const { return m_pExprOffset; }
  bool IsDereferencable(void) const { return m_Dereference; }

private:
  u32         m_AccessSizeInBit;
  Expression *m_pExprBase;
  Expression *m_pExprOffset;
  bool        m_Dereference;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EXPRESSION_HPP_
