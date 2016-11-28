#ifndef MEDUSA_EXPRESSION_FILTER_HPP
#define MEDUSA_EXPRESSION_FILTER_HPP

#include "medusa/export.hpp"
#include "medusa/expression.hpp"

namespace medusa
{
  typedef std::unordered_map<std::string, Expression::SPType> NamedExpressionType;

  class MEDUSA_EXPORT ExpressionPattern
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

  class MEDUSA_EXPORT ExpressionFilter
  {
  public:
    ExpressionFilter(ExpressionPattern::SPType spPattern);

    bool Execute(Expression::SPType spExpr);
    Expression::SPType GetExpression(std::string const& rExprName);

    typedef std::function<void(std::string const& rExprName, Expression::SPType& rspExpr)> Callback;
    void ForEach(Callback cb);

  private:
    ExpressionPattern::SPType m_spPattern;
    NamedExpressionType m_NamedExprs;
  };

  namespace Pattern
  {
    class MEDUSA_EXPORT Any : public ExpressionPattern
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

    class MEDUSA_EXPORT Callback : public ExpressionPattern
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

    class MEDUSA_EXPORT Memory : public ExpressionPattern
    {
    public:
      Memory(ExpressionPattern::SPType spOffsetPattern)
        : m_spOffsetPattern(spOffsetPattern) {}
      Memory(ExpressionPattern::SPType spBasePattern, ExpressionPattern::SPType spOffsetPattern)
        : m_spBasePattern(spBasePattern), m_spOffsetPattern(spOffsetPattern) {}

      Memory(std::string const& rPatternName, ExpressionPattern::SPType spOffsetPattern)
        : ExpressionPattern(rPatternName), m_spOffsetPattern(spOffsetPattern) {}
      Memory(std::string const& rPatternName, ExpressionPattern::SPType spBasePattern, ExpressionPattern::SPType spOffsetPattern)
        : ExpressionPattern(rPatternName), m_spOffsetPattern(spOffsetPattern) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        auto spMemExpr = expr_cast<MemoryExpression>(spExpr);
        if (spMemExpr == nullptr)
          return false;

        if (m_spBasePattern != nullptr && spMemExpr->GetBaseExpression() != nullptr)
          if (!m_spBasePattern->Filter(rNamedExprs, spMemExpr->GetBaseExpression()))
            return false;

        if (!m_spOffsetPattern->Filter(rNamedExprs, spMemExpr->GetOffsetExpression()))
          return false;

        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

      operator SPType(void)
      {
        return std::make_shared<Memory>(m_PatternName, m_spBasePattern, m_spOffsetPattern);
      }

    private:
      ExpressionPattern::SPType m_spBasePattern, m_spOffsetPattern;
    };

    template<unsigned Op>
    class AnyUnary : public ExpressionPattern
    {
    public:
      AnyUnary(ExpressionPattern::SPType spPattern) : m_spPattern(spPattern) {}
      AnyUnary(std::string const& rPatternName, ExpressionPattern::SPType spPattern) : ExpressionPattern(rPatternName), m_spPattern(spPattern) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        auto spUnOpExpr = expr_cast<UnaryOperationExpression>(spExpr);
        if (spUnOpExpr == nullptr)
          return false;

        if (!m_spPattern->Filter(rNamedExprs, spUnOpExpr->GetExpression()))
          return false;

        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

      operator SPType(void)
      {
        return std::make_shared<AnyUnary>(m_PatternName, m_spPattern);
      }

    private:
      ExpressionPattern::SPType m_spPattern;
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

    using NOT  = Unary<OperationExpression::OpNot>;
    using NEG  = Unary<OperationExpression::OpNeg>;
    using SWAP = Unary<OperationExpression::OpSwap>; // byte swap
    using BSF  = Unary<OperationExpression::OpBsf>;  // bit scan forward
    using BSR  = Unary<OperationExpression::OpBsr>;  // bit scan reverse

    template<unsigned Op>
    class AnyBinary : public ExpressionPattern
    {
    public:
      AnyBinary(ExpressionPattern::SPType spLeftPattern, ExpressionPattern::SPType spRightPattern)
        : m_spLeftPattern(spLeftPattern), m_spRightPattern(spRightPattern) {}
      AnyBinary(std::string const& rPatternName, ExpressionPattern::SPType spLeftPattern, ExpressionPattern::SPType spRightPattern)
        : ExpressionPattern(rPatternName), m_spLeftPattern(spLeftPattern), m_spRightPattern(spRightPattern) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        auto spBinOpExpr = expr_cast<BinaryOperationExpression>(spExpr);
        if (spBinOpExpr == nullptr)
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
        return std::make_shared<AnyBinary>(m_PatternName, m_spLeftPattern, m_spRightPattern);
      }

    private:
      ExpressionPattern::SPType m_spLeftPattern, m_spRightPattern;
    };

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

    using AND          = Binary<OperationExpression::OpAnd>;
    using OR           = Binary<OperationExpression::OpOr>;
    using XOR          = Binary<OperationExpression::OpXor>;
    using LLS          = Binary<OperationExpression::OpLls>; /* Logical Left Shift */
    using LRS          = Binary<OperationExpression::OpLrs>; /* Logical Right Shift */
    using ARS          = Binary<OperationExpression::OpArs>; /* Arithmetic Right Shift */
    using ROL          = Binary<OperationExpression::OpRol>; /* Logical Left Rotate */
    using ROR          = Binary<OperationExpression::OpRor>; /* Logical Right Rotate */
    using ADD          = Binary<OperationExpression::OpAdd>;
    using SUB          = Binary<OperationExpression::OpSub>;
    using MUL          = Binary<OperationExpression::OpMul>;
    using SDIV         = Binary<OperationExpression::OpSDiv>;
    using UDIV         = Binary<OperationExpression::OpUDiv>;
    using SMOD         = Binary<OperationExpression::OpSMod>;
    using UMOD         = Binary<OperationExpression::OpUMod>;
    using SEXT         = Binary<OperationExpression::OpSext>; /* Sign Extend */
    using ZEXT         = Binary<OperationExpression::OpZext>; /* Zero Extend */
    using INSERT_BITS  = Binary<OperationExpression::OpInsertBits>;
    using EXTRACT_BITS = Binary<OperationExpression::OpExtractBits>;
    using BCAST        = Binary<OperationExpression::OpBcast>; /* Bit Cast */

    class AnyCondition : public ExpressionPattern
    {
    public:
      AnyCondition(ExpressionPattern::SPType spRefPattern, ExpressionPattern::SPType spTestPattern)
        : m_spRefPattern(spRefPattern), m_spTestPattern(spTestPattern) {}
      AnyCondition(std::string const& rPatternName, ExpressionPattern::SPType spRefPattern, ExpressionPattern::SPType spTestPattern)
        : ExpressionPattern(rPatternName), m_spRefPattern(spRefPattern), m_spTestPattern(spTestPattern) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        auto spCondExpr = expr_cast<ConditionExpression>(spExpr);
        if (!m_spRefPattern->Filter(rNamedExprs, spCondExpr->GetReferenceExpression()))
          return false;
        if (!m_spTestPattern->Filter(rNamedExprs, spCondExpr->GetTestExpression()))
          return false;

        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

      operator SPType(void)
      {
        return std::make_shared<AnyCondition>(m_PatternName, m_spRefPattern, m_spTestPattern);
      }

    private:
      ExpressionPattern::SPType m_spRefPattern, m_spTestPattern;
    };

    template<unsigned Cond>
    class Condition : public ExpressionPattern
    {
    public:
      Condition(ExpressionPattern::SPType spRefPattern, ExpressionPattern::SPType spTestPattern)
        : m_spRefPattern(spRefPattern), m_spTestPattern(spTestPattern) {}
      Condition(std::string const& rPatternName, ExpressionPattern::SPType spRefPattern, ExpressionPattern::SPType spTestPattern)
        : ExpressionPattern(rPatternName), m_spRefPattern(spRefPattern), m_spTestPattern(spTestPattern) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        auto spCondExpr = expr_cast<ConditionExpression>(spExpr);
        if (spCondExpr->GetType() != Cond)
          return false;
        if (!m_spRefPattern->Filter(rNamedExprs, spCondExpr->GetReferenceExpression()))
          return false;
        if (!m_spTestPattern->Filter(rNamedExprs, spCondExpr->GetTestExpression()))
          return false;

        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

      operator SPType(void)
      {
        return std::make_shared<Condition>(m_PatternName, m_spRefPattern, m_spTestPattern);
      }

    private:
      ExpressionPattern::SPType m_spRefPattern, m_spTestPattern;
    };

    using EQ  = Condition<ConditionExpression::CondEq>;  //     equal
    using NE  = Condition<ConditionExpression::CondNe>;  // not equal
    using UGT = Condition<ConditionExpression::CondUgt>; // unsigned greater than
    using UGE = Condition<ConditionExpression::CondUge>; // unsigned greater than or equal
    using ULT = Condition<ConditionExpression::CondUlt>; // unsigned lesser  than
    using ULE = Condition<ConditionExpression::CondUle>; // unsigned lesser  than or equal
    using SGT = Condition<ConditionExpression::CondSgt>; //   signed greater than
    using SGE = Condition<ConditionExpression::CondSge>; //   signed greater than or equal
    using SLT = Condition<ConditionExpression::CondSlt>; //   signed lesser  than
    using SLE = Condition<ConditionExpression::CondSle>; //   signed lesser  than or equal

    class Ternary : public ExpressionPattern
    {
    public:
      Ternary(ExpressionPattern::SPType spCondPattern, ExpressionPattern::SPType spTruePattern, ExpressionPattern::SPType spFalsePattern)
        : m_spCondPattern(spCondPattern), m_spTruePattern(spTruePattern), m_spFalsePattern(spFalsePattern) {}
      Ternary(std::string const& rPatternName, ExpressionPattern::SPType spCondPattern, ExpressionPattern::SPType spTruePattern, ExpressionPattern::SPType spFalsePattern)
        : ExpressionPattern(rPatternName), m_spCondPattern(spCondPattern), m_spTruePattern(spTruePattern), m_spFalsePattern(spFalsePattern) {}

      virtual bool Filter(NamedExpressionType& rNamedExprs, Expression::SPType spExpr) const
      {
        auto spTernExpr = expr_cast<TernaryConditionExpression>(spExpr);
        if (spTernExpr == nullptr)
          return false;
        if (!m_spCondPattern->Filter(rNamedExprs, spTernExpr))
          return false;
        if (!m_spTruePattern->Filter(rNamedExprs, spTernExpr->GetTrueExpression()))
          return false;
        if (!m_spFalsePattern->Filter(rNamedExprs, spTernExpr->GetFalseExpression()))
          return false;

        _CaptureExpressionIfNeeded(rNamedExprs, spExpr);
        return true;
      }

      operator SPType(void)
      {
        return std::make_shared<Ternary>(m_PatternName, m_spCondPattern, m_spTruePattern, m_spFalsePattern);
      }

    private:
      ExpressionPattern::SPType m_spCondPattern;
      ExpressionPattern::SPType m_spTruePattern, m_spFalsePattern;
    };
  } // namespace Pattern
}

#endif // !MEDUSA_EXPRESSION_FILTER_HPP
