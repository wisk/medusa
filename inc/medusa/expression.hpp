#ifndef _MEDUSA_EXPRESSION_HPP_
#define _MEDUSA_EXPRESSION_HPP_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

MEDUSA_NAMESPACE_BEGIN

class Expression
{
public:
  virtual std::string ToString(void) const = 0;

private:
};

class OperationExpression : public Expression
{
public:
  enum Type
  {
    OpUnk,
    OpAff,
    OpAnd,
    OpOr,
    OpXor,
    OpAdd,
    OpSub,
    OpMul,
    OpDiv
  };
  virtual std::string ToString(void) const;

private:
  u8          m_OpType;
  Expression *m_pLeftExpr;
  Expression *m_pRightExpr;
};

class ConstantExpression : public Expression
{
public:
  virtual std::string ToString(void) const;

private:
  u32 m_ConstType;
  u64 m_Value;
};

class IdentifierExpression : public Expression
{
public:
  virtual std::string ToString(void) const;

private:
  u32 m_Id;
};

class MemoryExpression : public Expression
{
public:
  enum Type
  {
    OpUnk,
    OpRead,
    OpWrite
  };
  virtual std::string ToString(void) const;

private:
  u8 m_MemOpType;
  Address m_Address;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EXPRESSION_HPP_