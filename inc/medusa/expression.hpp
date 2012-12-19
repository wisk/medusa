#ifndef _MEDUSA_EXPRESSION_HPP_
#define _MEDUSA_EXPRESSION_HPP_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/cpu.hpp"

#include <list>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Expression
{
public:
  virtual ~Expression(void) {}

  typedef std::list<Expression *> List;
  virtual std::string ToString(void) const = 0;
  virtual Expression *Clone(void) const = 0;
  virtual u32 GetSizeInBit(void) const = 0;

private:
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

protected:
  Expression *m_pElseExpr;
};

class Medusa_EXPORT OperationExpression : public Expression
{
public:
  enum Type
  {
    OpUnk,
    OpAff,
    OpAnd,
    OpOr,
    OpXor,
    OpLls, /* Logical Left Shift */
    OpLrs, /* Logical Right Shift */
    OpArs, /* Arithmetic Right Shift */
    OpAdd,
    OpSub,
    OpMul,
    OpDiv
  };

  //! pLeftExpr and pRightExpr must be allocated by standard new
  OperationExpression(Type OpType, Expression *pLeftExpr, Expression *pRightExpr)
    : m_OpType(OpType), m_pLeftExpr(pLeftExpr), m_pRightExpr(pRightExpr) {}

  virtual ~OperationExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }

private:
  u8          m_OpType;
  Expression *m_pLeftExpr;
  Expression *m_pRightExpr;
};

class Medusa_EXPORT InvalidExpression : public Expression
{
public:
  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const { return 0; }
};

class Medusa_EXPORT ConstantExpression : public Expression
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

private:
  u32 m_ConstType;
  u64 m_Value;
};

class Medusa_EXPORT IdentifierExpression : public Expression
{
public:
  IdentifierExpression(u32 Id, CpuInformation const* pCpuInfo)
    : m_Id(Id), m_pCpuInfo(pCpuInfo) {}

  virtual ~IdentifierExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const;

private:
  u32 m_Id;
  CpuInformation const* m_pCpuInfo;
};

class Medusa_EXPORT MemoryExpression : public Expression
{
public:
  MemoryExpression(Expression *pExprBase, Expression *pExprOffset, bool Dereference = true)
    : m_pExprBase(pExprBase), m_pExprOffset(pExprOffset), m_Dereference(Dereference)
  { assert(pExprOffset != nullptr); }

  virtual ~MemoryExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;
  virtual u32 GetSizeInBit(void) const;

  Expression* GetAddressExpression(void) const { return m_pExprOffset; }
  bool IsDereferencable(void) const { return m_Dereference; }

private:
  Expression *m_pExprBase;
  Expression *m_pExprOffset;
  bool        m_Dereference;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EXPRESSION_HPP_
