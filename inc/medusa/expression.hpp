#ifndef _MEDUSA_EXPRESSION_HPP_
#define _MEDUSA_EXPRESSION_HPP_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

#include <list>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Expression
{
public:
  typedef std::list<Expression *> List;
  virtual std::string ToString(void) const = 0;
  virtual Expression *Clone(void) const = 0;

private:
};

class Medusa_EXPORT BindExpression : public Expression
{
public:
  BindExpression(Expression::List const& rExprs)
    : m_Expressions(rExprs) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;

private:
  Expression::List m_Expressions;
};

class Medusa_EXPORT ConditionExpression : public Expression
{
public:
  enum Type
  {
    CondEqual
  };

  ConditionExpression(Expression *pTestExpr)
    : m_pTestExpr(pTestExpr) {}

protected:
  Expression *m_pTestExpr;
};

class Medusa_EXPORT IfConditionExpression : public ConditionExpression
{
public:
  IfConditionExpression(Expression *pTestExpr, Expression *pThenExpr)
    : ConditionExpression(pTestExpr), m_pThenExpr(pThenExpr) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;

protected:
  Expression *m_pThenExpr;
};

class Medusa_EXPORT IfElseConditionExpression : public IfConditionExpression
{
public:
  IfElseConditionExpression(Expression *pTestExpr, Expression *pThenExpr, Expression *pElseExpr)
    : IfConditionExpression(pTestExpr, pThenExpr), m_pElseExpr(pElseExpr) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;

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
};

class Medusa_EXPORT ConstantExpression : public Expression
{
public:
  ConstantExpression(u32 ConstType, u64 Value)
    : m_ConstType(ConstType), m_Value(Value) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;

private:
  u32 m_ConstType;
  u64 m_Value;
};

class Medusa_EXPORT IdentifierExpression : public Expression
{
public:
  IdentifierExpression(u32 Id)
    : m_Id(Id) {}

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;

private:
  u32 m_Id;
};

class Medusa_EXPORT MemoryExpression : public Expression
{
public:
  MemoryExpression(Expression *pExprBase, Expression *pExprOffset)
    : m_pExprBase(pExprBase), m_pExprOffset(pExprOffset) {}

  ~MemoryExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression *Clone(void) const;

  Expression* GetAddressExpression(void) const { return m_pExprOffset; }

private:
  Expression *m_pExprBase;
  Expression *m_pExprOffset;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EXPRESSION_HPP_