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
    typedef std::shared_ptr<ExpressionPattern> SPType;

    ExpressionPattern(std::string const& rPatternName = "");

    virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
    {
      return false;
    }

    operator SPType(void)
    {
      return nullptr;
    }

  protected:
    void _CaptureExpressionIfNeeded(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const;

    std::string m_PatternName;
  };

  class Medusa_EXPORT ExpressionFilter
  {
  public:
    ExpressionFilter(ExpressionPattern::SPType spPattern);

    bool Execute(Expression::SPType spExpr);
    Expression::SPType GetExpression(std::string const& rExprName);

  private:
    ExpressionPattern::SPType m_spPattern;
    NamedExpressionType m_NamedExprs;
  };

  namespace Pattern
  {
#define EXPORT_TEMPLATE_PATTERN(type, name)\
  template Medusa_EXPORT class type;\
  using name = type

    class Medusa_EXPORT Any : public ExpressionPattern
    {
    public:
      Any(std::string const& rPatternName) : ExpressionPattern(rPatternName) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

      operator SPType(void)
      {
        return std::make_shared<Any>(m_PatternName);
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

      operator SPType(void)
      {
        return std::make_shared<Callback>(m_PatternName, m_Callback);
      }

    private:
      Type m_Callback;
    };

    template<unsigned Op>
    class Unary : public ExpressionPattern
    {
    public:
      Unary(ExpressionPattern::SPType spPattern) : m_spPattern(spPattern) {}
      Unary(std::string const& rPatternName, ExpressionPattern::SPType spPattern) : ExpressionPattern(rPatternName), m_spPattern(spPattern) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        auto spUnOpExpr = expr_cast<UnaryOperationExpression>(spExpr);
        if (spUnOpExpr == nullptr)
          return false;

        if (spUnOpExpr->GetOperation() != Op)
          return false;

        if (!m_spPattern->Filter(rNamedExprs, spUnOpExpr->GetExpression()))
          return false;

        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

      operator SPType(void)
      {
        return std::make_shared<Unary>(m_PatternName, m_spPattern);
      }

    private:
      ExpressionPattern::SPType m_spPattern;
    };

    EXPORT_TEMPLATE_PATTERN(Unary<OperationExpression::OpNot>,  NOT);
    EXPORT_TEMPLATE_PATTERN(Unary<OperationExpression::OpNeg>,  NEG);
    EXPORT_TEMPLATE_PATTERN(Unary<OperationExpression::OpSwap>, SWAP); // byte swap
    EXPORT_TEMPLATE_PATTERN(Unary<OperationExpression::OpBsf>,  BSF);  // bit scan forward
    EXPORT_TEMPLATE_PATTERN(Unary<OperationExpression::OpBsr>,  BSR);  // bit scan reverse

    template<unsigned Op>
    class Binary : public ExpressionPattern
    {
    public:
      Binary(ExpressionPattern::SPType spLeftPattern, ExpressionPattern::SPType spRightPattern)
        : m_spLeftPattern(spLeftPattern), m_spRightPattern(spRightPattern) {}
      Binary(std::string const& rPatternName, ExpressionPattern::SPType spLeftPattern, ExpressionPattern::SPType spRightPattern)
        : ExpressionPattern(rPatternName), m_spLeftPattern(spLeftPattern), m_spRightPattern(spRightPattern) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        auto spBinOpExpr = expr_cast<BinaryOperationExpression>(spExpr);
        if (spBinOpExpr == nullptr)
          return false;

        if (spBinOpExpr->GetOperation() != Op)
          return false;

        if (!m_spLeftPattern->Filter(rNamedExprs, spBinOpExpr->GetLeftExpression()))
          return false;
        if (!m_spRightPattern->Filter(rNamedExprs, spBinOpExpr->GetRightExpression()))
          return false;

        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

      operator SPType(void)
      {
        return std::make_shared<Binary>(m_PatternName, m_spLeftPattern, m_spRightPattern);
      }

    private:
      ExpressionPattern::SPType m_spLeftPattern, m_spRightPattern;
    };

    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpAnd>,         AND);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpOr>,          OR);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpXor>,         XOR);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpLls>,         LLS); /* Logical Left Shift */
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpLrs>,         LRS); /* Logical Right Shift */
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpArs>,         ARS); /* Arithmetic Right Shift */
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpRol>,         ROL); /* Logical Left Rotate */
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpRor>,         ROR); /* Logical Right Rotate */
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpAdd>,         ADD);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpSub>,         SUB);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpMul>,         MUL);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpSDiv>,        SDIV);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpUDiv>,        UDIV);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpSMod>,        SMOD);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpUMod>,        UMOD);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpSext>,        SEXT); /* Sign Extend */
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpZext>,        ZEXT); /* Zero Extend */
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpInsertBits>,  INSERT_BITS);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpExtractBits>, EXTRACT_BITS);
    EXPORT_TEMPLATE_PATTERN(Binary<OperationExpression::OpBcast>,       BCAST); /* Bit Cast */

#undef EXPORT_TEMPLATE_PATTERN
  }
}

#endif // !MEDUSA_EXPRESSION_FILTER_HPP
