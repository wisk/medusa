#ifndef MEDUSA_EXPRESSION_FILTER_HPP
#define MEDUSA_EXPRESSION_FILTER_HPP

#include "medusa/export.hpp"
#include "medusa/expression.hpp"

namespace medusa
{
  typedef std::unordered_map<std::string, Expression::SPType> NamedExpressionType;

  class Medusa_EXPORT ExpressionPattern
  {
  public:
    ExpressionPattern(std::string const& rPatternName = "");

    virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const = 0;

  protected:
    void _CaptureExpressionIfNeeded(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const;

    std::string m_PatternName;
  };

  class Medusa_EXPORT ExpressionFilter
  {
  public:
    ExpressionFilter(ExpressionPattern const& rPattern);

    bool Execute(Expression::SPType spExpr);
    Expression::SPType GetExpression(std::string const& rExprName);

  private:
    ExpressionPattern const& m_rPattern;
    NamedExpressionType m_NamedExprs;
  };

  namespace Pattern
  {
    class Medusa_EXPORT Any : public ExpressionPattern
    {
    public:
      Any(std::string const& rPatternName) : ExpressionPattern(rPatternName) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }
    };

    class Medusa_EXPORT Callback : public ExpressionPattern
    {
    public:
      typedef std::function<bool(Expression::SPType spExpr)> Type;

      Callback(Type Callback) : m_Callback(Callback) {}
      Callback(std::string const& rPatternName, Type Callback) : ExpressionPattern(rPatternName), m_Callback(Callback) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        if (!m_Callback(spExpr))
          return false;
        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

    private:
      Type m_Callback;
    };

    template<unsigned Op>
    class Unary : public ExpressionPattern
    {
    public:
      Unary(ExpressionPattern& rPattern) : m_rPattern(rPattern) {}
      Unary(std::string const& rPatternName, ExpressionPattern& rPattern) : ExpressionPattern(rPatternName), m_rPattern(rPattern) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        auto spUnOpExpr = expr_cast<UnaryOperationExpression>(spExpr);
        if (spUnOpExpr == nullptr)
          return false;

        if (spUnOpExpr->GetOperation() != Op)
          return false;

        if (!m_rPattern.Filter(rNamedExprs, spUnOpExpr->GetExpression()))
          return false;

        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

    private:
      ExpressionPattern& m_rPattern;
    };

    template<> class Medusa_EXPORT Unary<OperationExpression::OpNot>;
    template<> class Medusa_EXPORT Unary<OperationExpression::OpNeg>;
    template<> class Medusa_EXPORT Unary<OperationExpression::OpSwap>; // byte swap
    template<> class Medusa_EXPORT Unary<OperationExpression::OpBsf>;  // bit scan forward
    template<> class Medusa_EXPORT Unary<OperationExpression::OpBsr>;  // bit scan reverse

    template<unsigned Op>
    class Binary : public ExpressionPattern
    {
    public:
      Binary(ExpressionPattern& rLeftPattern, ExpressionPattern& rRightPattern)
        : m_rLeftPattern(rLeftPattern), m_rRightPattern(rRightPattern) {}
      Binary(std::string const& rPatternName, ExpressionPattern& rLeftPattern, ExpressionPattern& rRightPattern)
        : ExpressionPattern(rPatternName), m_rLeftPattern(rLeftPattern), m_rRightPattern(rRightPattern) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        auto spBinOpExpr = expr_cast<BinaryOperationExpression>(spExpr);
        if (spBinOpExpr == nullptr)
          return false;

        if (spBinOpExpr->GetOperation() != Op)
          return false;

        if (!m_rLeftPattern.Filter(rNamedExprs, spBinOpExpr->GetLeftExpression()))
          return false;
        if (!m_rRightPattern.Filter(rNamedExprs, spBinOpExpr->GetRightExpression()))
          return false;

        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

    private:
      ExpressionPattern& m_rLeftPattern, &m_rRightPattern;
    };

    template<> class Medusa_EXPORT Binary<OperationExpression::OpAnd>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpOr>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpXor>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpLls>; /* Logical Left Shift */
    template<> class Medusa_EXPORT Binary<OperationExpression::OpLrs>; /* Logical Right Shift */
    template<> class Medusa_EXPORT Binary<OperationExpression::OpArs>; /* Arithmetic Right Shift */
    template<> class Medusa_EXPORT Binary<OperationExpression::OpRol>; /* Logical Left Rotate */
    template<> class Medusa_EXPORT Binary<OperationExpression::OpRor>; /* Logical Right Rotate */
    template<> class Medusa_EXPORT Binary<OperationExpression::OpAdd>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpSub>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpMul>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpSDiv>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpUDiv>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpSMod>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpUMod>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpSext>; /* Sign Extend */
    template<> class Medusa_EXPORT Binary<OperationExpression::OpZext>; /* Zero Extend */
    template<> class Medusa_EXPORT Binary<OperationExpression::OpInsertBits>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpExtractBits>;
    template<> class Medusa_EXPORT Binary<OperationExpression::OpBcast>; /* Bit Cast */
  }
}

#endif // !MEDUSA_EXPRESSION_FILTER_HPP
