#ifndef MEDUSA_EXPRESSION_SIMPLIFIER_HPP
#define MEDUSA_EXPRESSION_SIMPLIFIER_HPP

#include "medusa/expression.hpp"

MEDUSA_NAMESPACE_BEGIN

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

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXPRESSION_SIMPLIFIER_HPP