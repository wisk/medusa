#ifndef MEDUSA_EXPRESSION_SIMPLIFIER_HPP
#define MEDUSA_EXPRESSION_SIMPLIFIER_HPP

#include "medusa/expression.hpp"

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT ExpressionSimplifier
{
public:
  ExpressionSimplifier(void) : m_IsDone(false) {}

  bool Execute(void);

protected:
  virtual bool _RunOnce(void) = 0;
  virtual bool _Finalize(void) = 0;

  bool m_IsDone;
};

class MEDUSA_EXPORT TrackedIdPropagation : public ExpressionSimplifier
{
public:
  TrackedIdPropagation(Expression::LSPType& rExprs, u32 Id);

protected:
  Expression::LSPType& m_rExprs;
  AssignmentExpression::SPType m_spResExpr;

  virtual bool _RunOnce(void);
  virtual bool _Finalize(void);

private:
  Expression::SPType __FindTrackedIdExpression(u32 Id, Address const& rAddr);
};

class MEDUSA_EXPORT TrackedIdMerger : public ExpressionSimplifier
{
public:
  TrackedIdMerger(Expression::SPType spToBeMerged, Expression::VSPType const& rExprsPool);

protected:
  Expression::SPType m_spMergedExpr;
  Expression::VSPType m_rExprsPool;

  virtual bool _RunOnce(void);
  virtual bool _Finalize(void);
};

//https://english.stackexchange.com/questions/35860/normalization-vs-canonicalization
class MEDUSA_EXPORT NormalizeExpression : public ExpressionSimplifier
{
public:
  NormalizeExpression(Expression::SPType spExpr);

  virtual bool _RunOnce(void);
  virtual bool _Finalize(void);

protected:
  Expression::SPType m_spExpr;
};

class MEDUSA_EXPORT ConstantPropagation : public ExpressionSimplifier
{
public:
  ConstantPropagation(Expression::SPType spExpr);

  virtual bool _RunOnce(void);
  virtual bool _Finalize(void);

protected:
  Expression::SPType m_spExpr;
};

class MEDUSA_EXPORT ExpressionRewriter : public ExpressionSimplifier
{
public:
  ExpressionRewriter(Expression::SPType& rspExpr);

protected:
  virtual bool _RunOnce(void);
  virtual bool _Finalize(void);

  Expression::SPType& m_rspExpr;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXPRESSION_SIMPLIFIER_HPP