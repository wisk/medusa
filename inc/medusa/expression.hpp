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
  virtual bool SignExtend(u32 NewSizeInBit) = 0;
};

class Medusa_EXPORT ContextExpression : public Expression
{
public:
  virtual ~ContextExpression(void) {}

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64& rValue) const = 0;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64 Value) = 0;
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, Address& rAddress) const = 0;
};

class Medusa_EXPORT ExpressionVisitor
{
public:
  virtual Expression* VisitBind           (Expression::List const& rExprList)                                                                                           { return nullptr; }
  virtual Expression* VisitCondition      (u32 Type, Expression const* pRefExpr, Expression const* pTestExpr)                                                           { return nullptr; }
  virtual Expression* VisitIfCondition    (u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr)                              { return nullptr; }
  virtual Expression* VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr) { return nullptr; }
  virtual Expression* VisitWhileCondition (u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pBodyExpr)                              { return nullptr; }
  virtual Expression* VisitOperation      (u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr)                                                         { return nullptr; }
  virtual Expression* VisitConstant       (u32 Type, u64 Value)                                                                                                         { return nullptr; }
  virtual Expression* VisitIdentifier     (u32 Id, CpuInformation const* pCpuInfo)                                                                                      { return nullptr; }
  virtual Expression* VisitMemory         (u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref)                                 { return nullptr; }
  virtual Expression* VisitVariable       (u32 SizeInBit, std::string const& rName)                                                                                     { return nullptr; }
};

class Medusa_EXPORT ExpressionVisitor_FindOperation : public ExpressionVisitor
{
public:
  virtual Expression* VisitBind     (Expression::List const& rExprList);
  virtual Expression* VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr);
};

class Medusa_EXPORT ExpressionVisitor_FindDestination : public ExpressionVisitor
{
public:
  virtual Expression* VisitBind     (Expression::List const& rExprList);
  virtual Expression* VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr);
};

class Medusa_EXPORT ExpressionVisitor_ContainIdentifier : public ExpressionVisitor
{
public:
  ExpressionVisitor_ContainIdentifier(u32 Id) : m_Id(Id), m_Result(false) {}
  bool GetResult(void) const { return m_Result; }

  virtual Expression* VisitBind      (Expression::List const& rExprList);
  virtual Expression* VisitOperation (u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr);
  virtual Expression* VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo);
  virtual Expression* VisitMemory    (u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref);

private:
  u32 m_Id;
  bool m_Result;
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
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

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
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

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
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

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
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

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
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  virtual u8 GetOperation(void) const { return m_OpType; }
  virtual Expression const* GetLeftExpression(void)  const { return m_pLeftExpr;  }
  virtual Expression const* GetRightExpression(void) const { return m_pRightExpr; }

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
  virtual bool SignExtend(u32 NewSizeInBit);

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64& rValue) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64 Value);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, Address& rAddress) const;

  u64          GetConstant(void) const { return m_Value; }

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
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64& rValue) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64 Value);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, Address& rAddress) const;

  u32 GetId(void) const { return m_Id; }

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
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64& rValue) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64 Value);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, Address& rAddress) const;

  Expression* GetAddressExpression(void) const { return m_pExprOffset; }
  bool IsDereferencable(void) const { return m_Dereference; }

private:
  u32         m_AccessSizeInBit;
  Expression *m_pExprBase;
  Expression *m_pExprOffset;
  bool        m_Dereference;
};

class Medusa_EXPORT VariableExpression : public ContextExpression
{
public:
  VariableExpression(u32 Type, std::string const& rName)
    : m_Type(Type), m_Name(rName) {}

  virtual ~VariableExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression* Clone(void) const;
  virtual u32 GetSizeInBit(void) const;
  virtual Expression* Visit(ExpressionVisitor* pVisitor) const { return pVisitor->VisitVariable(m_Type, m_Name); }
  virtual bool SignExtend(u32 NewSizeInBit) { return false; }

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64& rValue) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, u64 Value);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt, Address& rAddress) const;

private:
  std::string m_Name;
  u32 m_Type;
};


MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_EXPRESSION_HPP_
