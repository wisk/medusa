#ifndef MEDUSA_EXPRESSION_HPP_HPP
#define MEDUSA_EXPRESSION_HPP_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/information.hpp"
#include "medusa/context.hpp"

#include <list>

MEDUSA_NAMESPACE_BEGIN

class ExpressionVisitor;

// TODO: add Track.{h,c}pp
namespace Track
{
  class Medusa_EXPORT Context
  {
  public:
    void TrackId(u32 Id, Address const& rCurAddr);
    bool GetTrackAddress(u32 RegId, Address& rTrackedAddress);
    bool IsEmpty(void) const { return m_TrackedId.empty(); }

  private:
    std::unordered_map<u32, Address> m_TrackedId;
  };

  typedef std::tuple<u32, Address> Id;

  class Medusa_EXPORT BackTrackContext
  {
  public:
    void TrackId(Track::Id const& rId) { m_Ids.insert(rId); }
    void UntrackId(Track::Id const& rId) { m_Ids.erase(rId); }
    bool IsTracked(Track::Id const& rId) const { return m_Ids.find(rId) != std::end(m_Ids); }
    bool IsEmpty(void) const { return m_Ids.empty(); }

  private:
    std::set<Track::Id> m_Ids;
  };
}

// expression /////////////////////////////////////////////////////////////////

class Medusa_EXPORT Expression : public std::enable_shared_from_this<Expression>
{
public:
  typedef std::shared_ptr<Expression> SPType;
  typedef std::list<Expression::SPType> LSPType;
  typedef std::vector<Expression::SPType> VSPType;

  typedef std::deque<IntType> DataContainerType;

  enum Kind
  {
    Unknown,
    Sys,
    Bind,
    Cond,
    TernaryCond,
    IfElseCond,
    WhileCond,
    Assign,
    Op,
    UnOp,
    BinOp,
    Const,
    IntConst,
    FloatConst,
    Id,
    VecId,
    Track,
    Var,
    Mem,
    Sym,
  };

  enum CompareType
  {
    CmpUnknown,
    CmpDifferent,
    CmpSameExpression,
    CmpIdentical,
  };

  virtual ~Expression(void) {}

  virtual std::string ToString(void) const = 0;
  virtual Expression::SPType Clone(void) const = 0;
  virtual u32 GetBitSize(void) const = 0;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor) = 0;
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) = 0;
  virtual CompareType Compare(Expression::SPType spExpr) const = 0;

  virtual Kind GetClassKind(void) const { return Unknown; }
  static  Kind GetStaticClassKind(void) { return Unknown; }
  virtual bool IsKindOf(Kind ExprKind) const { return ExprKind == Unknown; }

  virtual void   Prepare(DataContainerType& rData) const { rData.resize(1); }
  virtual bool   Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const { return false; }
  virtual bool   Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) { return false; }
  virtual bool   GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const { return false; }
  virtual SPType ToAddress(void) const { return nullptr; }

  template <typename T>
  bool IsClassOf(void) const { return IsKindOf(T::GetStaticClassKind()); }
};

template <typename T>
typename T::SPType expr_cast(Expression::SPType spExpr)
{
  if (spExpr == nullptr)
    return nullptr;
  if (!spExpr->IsClassOf<T>())
    return nullptr;
  return std::static_pointer_cast<T>(spExpr);
}

#define DECL_EXPR(EXPR_NAME, EXPR_KIND, BASE_EXPR_NAME)\
  public:\
  typedef std::shared_ptr<EXPR_NAME> SPType;\
  virtual Kind GetClassKind(void) const { return EXPR_KIND; }\
  static  Kind GetStaticClassKind(void) { return EXPR_KIND; }\
  virtual bool IsKindOf(Kind ExprKind) const { return ExprKind == EXPR_KIND || BASE_EXPR_NAME::IsKindOf(ExprKind); }

// system expression //////////////////////////////////////////////////////////

class Medusa_EXPORT SystemExpression : public Expression
{
  DECL_EXPR(SystemExpression, Expression::Sys, Expression)

public:
  SystemExpression(std::string const& rExprName, Address const& rAddr);
  virtual ~SystemExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  std::string const& GetName(void)    const { return m_Name;    }
  Address     const& GetAddress(void) const { return m_Address; }

protected:
  std::string m_Name;
  Address     m_Address;
};

// bind expression ////////////////////////////////////////////////////////////

class Medusa_EXPORT BindExpression : public Expression
{
  DECL_EXPR(BindExpression, Expression::Bind, Expression)

public:
  BindExpression(Expression::LSPType const& rExprs);
  virtual ~BindExpression(void);

  Expression::LSPType& GetBoundExpressions(void) { return m_Expressions; }

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

private:
  Expression::LSPType m_Expressions;
};

// condition expression ///////////////////////////////////////////////////////

class Medusa_EXPORT ConditionExpression : public Expression
{
public:
  typedef std::shared_ptr<ConditionExpression> SPType;

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

  ConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr);

  virtual ~ConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const { return nullptr; }
  virtual u32 GetBitSize(void) const { return 0; }
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Type GetType(void) const { return m_Type; }
  Expression::SPType GetReferenceExpression(void) const { return m_spRefExpr; }
  Expression::SPType GetTestExpression(void) const { return m_spTestExpr; }

protected:
  Type m_Type;
  Expression::SPType m_spRefExpr;
  Expression::SPType m_spTestExpr;
};

class Medusa_EXPORT TernaryConditionExpression : public ConditionExpression
{
  DECL_EXPR(TernaryConditionExpression, Expression::TernaryCond, Expression)

public:
  TernaryConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spTrueExpr, Expression::SPType spFalseExpr);

  virtual ~TernaryConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetTrueExpression(void) { return m_spTrueExpr; }
  Expression::SPType GetFalseExpression(void) { return m_spFalseExpr; }

protected:
  Expression::SPType m_spTrueExpr;
  Expression::SPType m_spFalseExpr;
};

class Medusa_EXPORT IfElseConditionExpression : public ConditionExpression
{
  DECL_EXPR(IfElseConditionExpression, Expression::IfElseCond, Expression)

public:
  IfElseConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spThenExpr, Expression::SPType spElseExpr);
  virtual ~IfElseConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetThenExpression(void) { return m_spThenExpr; }
  Expression::SPType GetElseExpression(void) { return m_spElseExpr; }

protected:
  Expression::SPType m_spThenExpr;
  Expression::SPType m_spElseExpr;
};

class Medusa_EXPORT WhileConditionExpression : public ConditionExpression
{
  DECL_EXPR(WhileConditionExpression, Expression::WhileCond, Expression)

public:
  WhileConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spBodyExpr);
  virtual ~WhileConditionExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetBodyExpression(void) { return m_spBodyExpr; }

protected:
  Expression::SPType m_spBodyExpr;
};

// operation expression ///////////////////////////////////////////////////////

class Medusa_EXPORT AssignmentExpression : public Expression
{
  DECL_EXPR(AssignmentExpression, Expression::Assign, Expression)

public:
  AssignmentExpression(Expression::SPType spDstExpr, Expression::SPType spSrcExpr);
  virtual ~AssignmentExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual Expression::SPType GetDestinationExpression(void) { return m_spDstExpr; }
  virtual Expression::SPType GetSourceExpression(void) { return m_spSrcExpr; }

private:
  Expression::SPType m_spDstExpr;
  Expression::SPType m_spSrcExpr;
};

class Medusa_EXPORT OperationExpression : public Expression
{
  DECL_EXPR(OperationExpression, Expression::Op, Expression)

public:
  enum Type
  {
    OpUnk,

    // Unary operations
    OpNot,
    OpNeg,
    OpSwap, // byte swap
    OpBsf,  // bit scan forward
    OpBsr,  // bit scan reverse

    // Binary operations
    OpXchg, // exchange
    OpAnd,
    OpOr,
    OpXor,
    OpLls, /* Logical Left Shift */
    OpLrs, /* Logical Right Shift */
    OpArs, /* Arithmetic Right Shift */
    OpRol, /* Logical Left Rotate */
    OpRor, /* Logical Right Rotate */
    OpAdd,
    OpSub,
    OpMul,
    OpSDiv,
    OpUDiv,
    OpSMod,
    OpUMod,
    OpSext, /* Sign Extend */
    OpZext, /* Zero Extend */
    OpInsertBits,
    OpExtractBits,
    OpBcast, /* Bit Cast */
  };

  //! pLeftExpr and pRightExpr must be allocated by standard new
  OperationExpression(Type OpType);
  virtual ~OperationExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const { return nullptr; }
  virtual u32 GetBitSize(void) const;
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  u8 GetOperation(void) const { return m_OpType; }
  u8 GetOppositeOperation(void) const;

protected:
  u8 m_OpType;
};

class Medusa_EXPORT UnaryOperationExpression : public OperationExpression
{
  DECL_EXPR(UnaryOperationExpression, Expression::UnOp, Expression)

public:
  UnaryOperationExpression(Type OpType, Expression::SPType spExpr);
  virtual ~UnaryOperationExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetExpression(void) { return m_spExpr;  }

private:
  Expression::SPType m_spExpr;
};

class Medusa_EXPORT BinaryOperationExpression : public OperationExpression
{
  DECL_EXPR(BinaryOperationExpression, Expression::BinOp, Expression)

public:
  BinaryOperationExpression(Type OpType, Expression::SPType spLeftExpr, Expression::SPType spRightExpr);
  virtual ~BinaryOperationExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetLeftExpression(void)  { return m_spLeftExpr; }
  Expression::SPType GetRightExpression(void) { return m_spRightExpr; }

  void SwapExpressions(void) { std::swap(m_spLeftExpr, m_spRightExpr); }
  void SwapLeftExpression(Expression::SPType& rspExpr) { m_spLeftExpr.swap(rspExpr); }
  void SwapRightExpression(Expression::SPType& rspExpr) { m_spRightExpr.swap(rspExpr); }
  void SwapLeftExpressions(BinaryOperationExpression::SPType spOpExpr);

private:
  Expression::SPType m_spLeftExpr;
  Expression::SPType m_spRightExpr;
};

// constant expression ////////////////////////////////////////////////////////

class Medusa_EXPORT ConstantExpression : public Expression
{
  DECL_EXPR(ConstantExpression, Expression::Const, Expression)
};

class Medusa_EXPORT IntegerExpression : public ConstantExpression
{
  DECL_EXPR(IntegerExpression, Expression::IntConst, Expression)

public:
  IntegerExpression(u16 BitSize, ap_int Value);
  IntegerExpression(IntType const& rValue);
  virtual ~IntegerExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const { return m_Value.GetBitSize(); }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  IntType      GetInt(void) const { return m_Value; }

private:
  IntType m_Value;
};

class Medusa_EXPORT FloatingPointExpression : public ConstantExpression
{
  DECL_EXPR(FloatingPointExpression, Expression::FloatConst, Expression)

public:
  union FloatingType
  {
    double dbl;
    float sgl;
  };

  enum Precision
  {
    Double,
    Single,
  };

  FloatingPointExpression(float const Value);
  FloatingPointExpression(double const Value);
  FloatingPointExpression(FloatingType const& rValue);
  virtual ~FloatingPointExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  float GetSingle(void) const { return m_Value.sgl; };
  double GetDouble(void) const { return m_Value.dbl; };
  u8 GetPrecision(void) const { return m_Precision; };

private:
  FloatingType m_Value;
  u8 m_Precision;
};

// identifier expression //////////////////////////////////////////////////////

class Medusa_EXPORT IdentifierExpression : public Expression
{
  DECL_EXPR(IdentifierExpression, Expression::Id, Expression)

public:
  IdentifierExpression(u32 Id, CpuInformation const* pCpuInfo);

  virtual ~IdentifierExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  u32 GetId(void) const { return m_Id; }
  CpuInformation const* GetCpuInformation(void) const { return m_pCpuInfo; }

protected:
  u32 m_Id;
  CpuInformation const* m_pCpuInfo;
};

class Medusa_EXPORT VectorIdentifierExpression : public Expression
{
  DECL_EXPR(VectorIdentifierExpression, Expression::VecId, Expression);

public:
  VectorIdentifierExpression(std::vector<u32> const& rVecId, CpuInformation const* pCpuInfo);

  virtual ~VectorIdentifierExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual void Prepare(DataContainerType& rData) const;
  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  std::vector<u32> GetVector(void) const { return m_VecId; }
  CpuInformation const* GetCpuInformation(void) const { return m_pCpuInfo; }

protected:
  std::vector<u32> m_VecId;
  CpuInformation const* m_pCpuInfo;
};

class Medusa_EXPORT TrackExpression : public Expression
{
  DECL_EXPR(TrackExpression, Expression::Track, Expression)

public:
  TrackExpression(Expression::SPType spTrkExpr, Address const& rCurAddr, u8 Pos);

  virtual ~TrackExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetTrackedExpression(void) const { return m_spTrkExpr; }
  Address GetTrackAddress(void) const { return m_CurAddr; }
  u8 GetTrackPosition(void) const { return m_Pos; }

private:
  Expression::SPType m_spTrkExpr;
  Address m_CurAddr;
  u8 m_Pos;
};

// variable ///////////////////////////////////////////////////////////////////

class Medusa_EXPORT VariableExpression : public Expression
{
  DECL_EXPR(VariableExpression, Expression::Var, Expression)

public:
  enum ActionType
  {
    Unknown,
    Alloc,
    Free,
    Use,
  };

  VariableExpression(std::string const& rVarName, ActionType VarType, u32 BitSize = 0);

  virtual ~VariableExpression(void);

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  std::string const& GetName(void) const { return m_Name; }
  ActionType         GetAction(void) const { return m_Action; }

  void SetBitSize(u32 BitSize) { m_BitSize = BitSize; }

protected:
  std::string m_Name;
  ActionType m_Action;
  u32 m_BitSize;
};

// memory expression //////////////////////////////////////////////////////////

class Medusa_EXPORT MemoryExpression : public Expression
{
  DECL_EXPR(MemoryExpression, Expression::Mem, Expression)

public:
  MemoryExpression(u32 AccessSize, Expression::SPType spExprBase, Expression::SPType spExprOffset, bool Dereference = true);

  virtual ~MemoryExpression(void);

  u32 GetAccessSizeInBit(void) const { return m_AccessSizeInBit; }
  Expression::SPType GetBaseExpression(void) { return m_spBaseExpr; }
  Expression::SPType GetOffsetExpression(void) { return m_spOffExpr; }

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual bool               Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
  virtual bool               Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
  virtual bool               GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;
  virtual Expression::SPType ToAddress(void) const;

  Expression::SPType GetAddressExpression(void) const { return m_spOffExpr; }
  bool IsDereferencable(void) const { return m_Dereference; }

private:
  u32 m_AccessSizeInBit;
  Expression::SPType m_spBaseExpr;
  Expression::SPType m_spOffExpr;
  bool m_Dereference;
};

// symbolic expression ////////////////////////////////////////////////////////

class Medusa_EXPORT SymbolicExpression : public Expression
{
  DECL_EXPR(SymbolicExpression, Expression::Sym, Expression)

public:
  enum Type
  {
    Unknown,
    ReturnedValue,
    FromParameter,
    Undefined,
  };

  SymbolicExpression(Type SymType, std::string const& rValue, Address const& rAddr, Expression::SPType spExpr);

  virtual ~SymbolicExpression(void) {}

  virtual std::string ToString(void) const;
  virtual Expression::SPType Clone(void) const;
  virtual u32 GetBitSize(void) const;
  virtual Expression::SPType Visit(ExpressionVisitor *pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Type GetType(void) const { return m_Type; }
  std::string const& GetValue(void) const { return m_Value; }
  Address const& GetAddress(void) const { return m_Address; }
  Expression::SPType GetExpression(void) const { return m_spExpr; }

private:
  Type m_Type;
  std::string m_Value;
  Address m_Address;
  Expression::SPType m_spExpr;
};

// visitor ////////////////////////////////////////////////////////////////////

class Medusa_EXPORT ExpressionVisitor
{
public:
  virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
  virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
  virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitInt(IntegerExpression::SPType spIntExpr);
  virtual Expression::SPType VisitFloat(FloatingPointExpression::SPType spFloatExpr);
  virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
  virtual Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr);
  virtual Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr);
  virtual Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr);
  virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
  virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);
};

// helper /////////////////////////////////////////////////////////////////////

namespace Expr
{
  Medusa_EXPORT Expression::SPType MakeInt(IntType const& rValue);
  Medusa_EXPORT Expression::SPType MakeInt(u16 BitSize, ap_int Value);
  Medusa_EXPORT Expression::SPType MakeFloat(float const Value);
  Medusa_EXPORT Expression::SPType MakeFloat(double const Value);
  Medusa_EXPORT Expression::SPType MakeBoolean(bool Value);
  Medusa_EXPORT Expression::SPType MakeId(u32 Id, CpuInformation const* pCpuInfo);
  Medusa_EXPORT Expression::SPType MakeVecId(std::vector<u32> const& rVecId, CpuInformation const* pCpuInfo);
  Medusa_EXPORT Expression::SPType MakeTrack(Expression::SPType spTrkExpr, Address const& rCurAddr, u8 Pos);
  Medusa_EXPORT Expression::SPType MakeMem(u32 AccessSize, Expression::SPType spExprBase, Expression::SPType spExprOffset, bool Dereference = true);
  Medusa_EXPORT Expression::SPType MakeVar(std::string const& rName, VariableExpression::ActionType Act, u16 BitSize = 0);

  Medusa_EXPORT Expression::SPType MakeTernaryCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spTrueExpr, Expression::SPType spFalseExpr);
  Medusa_EXPORT Expression::SPType MakeIfElseCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spThenExpr, Expression::SPType spElseExpr);
  Medusa_EXPORT Expression::SPType MakeWhileCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spBodyExpr);

  Medusa_EXPORT Expression::SPType MakeAssign(Expression::SPType spDstExpr, Expression::SPType spSrcExpr);
  Medusa_EXPORT Expression::SPType MakeUnOp(OperationExpression::Type OpType, Expression::SPType spExpr);
  Medusa_EXPORT Expression::SPType MakeBinOp(OperationExpression::Type OpType, Expression::SPType spLeftExpr, Expression::SPType spRightExpr);

  Medusa_EXPORT Expression::SPType MakeBind(Expression::LSPType const& rExprs);

  Medusa_EXPORT Expression::SPType MakeSym(SymbolicExpression::Type SymType, std::string const& rValue, Address const& rAddr, Expression::SPType spExpr = nullptr);
  Medusa_EXPORT Expression::SPType MakeSys(std::string const& rName, Address const& rAddr);

  Medusa_EXPORT bool TestKind(Expression::Kind Kind, Expression::SPType spExpr);
}


MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXPRESSION_HPP_HPP
