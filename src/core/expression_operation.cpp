#include <medusa/expression.hpp>

namespace medusa
{

  Expression::SPType operator++(Expression::SPType spExpr)
  {
    auto ExprBitSz = spExpr->GetBitSize();
    return Expr::MakeBinOp(OperationExpression::OpAdd, spExpr, Expr::MakeBitVector(ExprBitSz, 1));
  }

  Expression::SPType operator++(Expression::SPType spExpr, int)
  {
    auto ExprBitSz = spExpr->GetBitSize();
    return Expr::MakeBinOp(OperationExpression::OpAdd, spExpr, Expr::MakeBitVector(ExprBitSz, 1));
  }

  Expression::SPType operator--(Expression::SPType spExpr)
  {
    auto ExprBitSz = spExpr->GetBitSize();
    return Expr::MakeBinOp(OperationExpression::OpSub, spExpr, Expr::MakeBitVector(ExprBitSz, 1));
  }

  Expression::SPType operator--(Expression::SPType spExpr, int)
  {
    auto ExprBitSz = spExpr->GetBitSize();
    return Expr::MakeBinOp(OperationExpression::OpSub, spExpr, Expr::MakeBitVector(ExprBitSz, 1));
  }

  Expression::SPType operator~(Expression::SPType const spExpr)
  {
    return Expr::MakeUnOp(OperationExpression::OpNot, spExpr);
  }

  Expression::SPType operator-(Expression::SPType const spExpr)
  {
    return Expr::MakeUnOp(OperationExpression::OpNeg, spExpr);
  }

  Expression::SPType operator&(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpAnd, spLeftExpr, spRightExpr);
  }

  Expression::SPType operator|(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpOr, spLeftExpr, spRightExpr);
  }

  Expression::SPType operator^(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpXor, spLeftExpr, spRightExpr);
  }

  Expression::SPType operator+(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpAdd, spLeftExpr, spRightExpr);
  }

  Expression::SPType operator-(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpSub, spLeftExpr, spRightExpr);
  }

  Expression::SPType operator*(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpMul, spLeftExpr, spRightExpr);
  }

  Expression::SPType operator/(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpUDiv, spLeftExpr, spRightExpr);
  }

  Expression::SPType SDIV(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpSDiv, spLeftExpr, spRightExpr);
  }

  Expression::SPType operator%(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpUMod, spLeftExpr, spRightExpr);
  }

  Expression::SPType SREM(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpSMod, spLeftExpr, spRightExpr);
  }

  Expression::SPType operator<<(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpLls, spLeftExpr, spRightExpr);
  }

  Expression::SPType operator>>(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpLrs, spLeftExpr, spRightExpr);
  }

  Expression::SPType ARS(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr)
  {
    return Expr::MakeBinOp(OperationExpression::OpArs, spLeftExpr, spRightExpr);
  }

  Expression::SPType operator&(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return spLeftExpr & Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal);
  }

  Expression::SPType operator|(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return spLeftExpr | Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal);
  }

  Expression::SPType operator^(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return spLeftExpr ^ Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal);
  }

  Expression::SPType operator+(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return spLeftExpr + Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal);
  }

  Expression::SPType operator-(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return spLeftExpr - Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal);
  }

  Expression::SPType operator*(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return spLeftExpr * Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal);
  }

  Expression::SPType operator/(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return spLeftExpr / Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal);
  }

  Expression::SPType SDIV(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return SDIV(spLeftExpr, Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal));
  }

  Expression::SPType operator%(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return spLeftExpr % Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal);
  }

  Expression::SPType SREM(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return SREM(spLeftExpr, Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal));
  }
  
  Expression::SPType operator<<(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return spLeftExpr << Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal);
  }

  Expression::SPType operator>>(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return spLeftExpr >> Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal);
  }

  Expression::SPType ARS(Expression::SPType const spLeftExpr, u32 RightVal)
  {
    return ARS(spLeftExpr, Expr::MakeBitVector(spLeftExpr->GetBitSize(), RightVal));
  }
}