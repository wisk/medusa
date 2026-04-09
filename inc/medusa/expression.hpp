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
  class MEDUSA_EXPORT Context
  {
  public:
    void TrackId(u32 Id, Address const& rCurAddr);
    [[nodiscard]] bool GetTrackAddress(u32 RegId, Address& rTrackedAddress);
    [[nodiscard]] bool IsEmpty() const { return m_TrackedId.empty(); }

  private:
    std::unordered_map<u32, Address> m_TrackedId;
  };

  using Id = std::tuple<u32, Address>;

  class MEDUSA_EXPORT BackTrackContext
  {
  public:
    void TrackId(Track::Id const& rId) { m_Ids.insert(rId); }
    void UntrackId(Track::Id const& rId) { m_Ids.erase(rId); }
    [[nodiscard]] bool IsTracked(Track::Id const& rId) const { return m_Ids.find(rId) != std::end(m_Ids); }
    [[nodiscard]] bool IsEmpty() const { return m_Ids.empty(); }

  private:
    std::set<Track::Id> m_Ids;
  };
}

// expression /////////////////////////////////////////////////////////////////

class MEDUSA_EXPORT Expression : public std::enable_shared_from_this<Expression>
{
public:
  using SPType = std::shared_ptr<Expression>;
  using LSPType = std::list<Expression::SPType>;
  using VSPType = std::vector<Expression::SPType>;

  using DataContainerType = std::deque<BitVector>;

  enum class Kind
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
    Id,
    VecId,
    Track,
    Var,
    Mem,
    Sym,
  };

  enum class CompareType
  {
    CmpUnknown,
    CmpDifferent,
    CmpSameExpression,
    CmpIdentical,
  };

  // Backward-compatible aliases for generated code that uses Expression::Id, Expression::Mem, etc.
  static constexpr Kind Unknown      = Kind::Unknown;
  static constexpr Kind Sys          = Kind::Sys;
  static constexpr Kind Bind         = Kind::Bind;
  static constexpr Kind Cond         = Kind::Cond;
  static constexpr Kind TernaryCond  = Kind::TernaryCond;
  static constexpr Kind IfElseCond   = Kind::IfElseCond;
  static constexpr Kind WhileCond    = Kind::WhileCond;
  static constexpr Kind Assign       = Kind::Assign;
  static constexpr Kind Op           = Kind::Op;
  static constexpr Kind UnOp         = Kind::UnOp;
  static constexpr Kind BinOp        = Kind::BinOp;
  static constexpr Kind Const        = Kind::Const;
  static constexpr Kind Id           = Kind::Id;
  static constexpr Kind VecId        = Kind::VecId;
  static constexpr Kind Track        = Kind::Track;
  static constexpr Kind Var          = Kind::Var;
  static constexpr Kind Mem          = Kind::Mem;
  static constexpr Kind Sym          = Kind::Sym;

  static constexpr CompareType CmpUnknown        = CompareType::CmpUnknown;
  static constexpr CompareType CmpDifferent      = CompareType::CmpDifferent;
  static constexpr CompareType CmpSameExpression = CompareType::CmpSameExpression;
  static constexpr CompareType CmpIdentical      = CompareType::CmpIdentical;

  virtual ~Expression() = default;

  static Expression::VSPType Parse(std::string const& rExpressions, CpuInformation const& rCpuInfo, u8 Mode);

  [[nodiscard]] virtual std::string ToString() const = 0;
  [[nodiscard]] virtual Expression::SPType Clone() const = 0;
  [[nodiscard]] virtual u32 GetBitSize() const = 0;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor) = 0;
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) = 0;
  [[nodiscard]] virtual CompareType Compare(Expression::SPType spExpr) const = 0;

  [[nodiscard]] virtual Kind GetClassKind() const { return Kind::Unknown; }
  [[nodiscard]] static  Kind GetStaticClassKind() { return Kind::Unknown; }
  [[nodiscard]] virtual bool IsKindOf(Kind ExprKind) const { return ExprKind == Kind::Unknown; }

  virtual void   Prepare(DataContainerType& rData) const { rData.resize(1); }
  [[nodiscard]] virtual bool   Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const { return false; }
  virtual bool   Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) { return false; }
  [[nodiscard]] virtual bool   GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const { return false; }
  [[nodiscard]] virtual SPType ToAddress() const { return nullptr; }

  template <typename T>
  [[nodiscard]] bool IsClassOf() const { return IsKindOf(T::GetStaticClassKind()); }
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
  using SPType = std::shared_ptr<EXPR_NAME>;\
  [[nodiscard]] Kind GetClassKind() const override { return EXPR_KIND; }\
  [[nodiscard]] static  Kind GetStaticClassKind() { return EXPR_KIND; }\
  [[nodiscard]] bool IsKindOf(Kind ExprKind) const override { return ExprKind == EXPR_KIND || BASE_EXPR_NAME::IsKindOf(ExprKind); }


// unary operations
/// src: http://en.cppreference.com/w/cpp/language/operator_incdec
MEDUSA_EXPORT Expression::SPType operator++(Expression::SPType spExpr);
MEDUSA_EXPORT Expression::SPType operator++(Expression::SPType spExpr, int);
MEDUSA_EXPORT Expression::SPType operator--(Expression::SPType spExpr);
MEDUSA_EXPORT Expression::SPType operator--(Expression::SPType spExpr, int);

MEDUSA_EXPORT Expression::SPType operator~(Expression::SPType const spExpr);
MEDUSA_EXPORT Expression::SPType operator-(Expression::SPType const spExpr);

// binary operations
MEDUSA_EXPORT Expression::SPType operator&(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);
MEDUSA_EXPORT Expression::SPType operator|(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);
MEDUSA_EXPORT Expression::SPType operator^(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);

MEDUSA_EXPORT Expression::SPType operator+(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);
MEDUSA_EXPORT Expression::SPType operator-(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);
MEDUSA_EXPORT Expression::SPType operator*(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);
MEDUSA_EXPORT Expression::SPType operator/(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);
MEDUSA_EXPORT Expression::SPType SDIV     (Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);
MEDUSA_EXPORT Expression::SPType operator%(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);
MEDUSA_EXPORT Expression::SPType SREM     (Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);

MEDUSA_EXPORT Expression::SPType operator<<(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);
MEDUSA_EXPORT Expression::SPType operator>>(Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);
MEDUSA_EXPORT Expression::SPType ARS       (Expression::SPType const spLeftExpr, Expression::SPType const spRightExpr);

MEDUSA_EXPORT Expression::SPType operator&(Expression::SPType const spLeftExpr, u32 RightVal);
MEDUSA_EXPORT Expression::SPType operator|(Expression::SPType const spLeftExpr, u32 RightVal);
MEDUSA_EXPORT Expression::SPType operator^(Expression::SPType const spLeftExpr, u32 RightVal);

MEDUSA_EXPORT Expression::SPType operator+(Expression::SPType const spLeftExpr, u32 RightVal);
MEDUSA_EXPORT Expression::SPType operator-(Expression::SPType const spLeftExpr, u32 RightVal);
MEDUSA_EXPORT Expression::SPType operator*(Expression::SPType const spLeftExpr, u32 RightVal);
MEDUSA_EXPORT Expression::SPType operator/(Expression::SPType const spLeftExpr, u32 RightVal);
MEDUSA_EXPORT Expression::SPType SDIV     (Expression::SPType const spLeftExpr, u32 RightVal);
MEDUSA_EXPORT Expression::SPType operator%(Expression::SPType const spLeftExpr, u32 RightVal);
MEDUSA_EXPORT Expression::SPType SREM     (Expression::SPType const spLeftExpr, u32 RightVal);

MEDUSA_EXPORT Expression::SPType operator<<(Expression::SPType const spLeftExpr, u32 RightVal);
MEDUSA_EXPORT Expression::SPType operator>>(Expression::SPType const spLeftExpr, u32 RightVal);
MEDUSA_EXPORT Expression::SPType ARS       (Expression::SPType const spLeftExpr, u32 RightVal);

// system expression //////////////////////////////////////////////////////////

class MEDUSA_EXPORT SystemExpression : public Expression
{
  DECL_EXPR(SystemExpression, Expression::Kind::Sys, Expression)

public:
  SystemExpression(std::string const& rExprName, Address const& rAddr);
  virtual ~SystemExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  std::string const& GetName()    const { return m_Name;    }
  Address     const& GetAddress() const { return m_Address; }

protected:
  std::string m_Name;
  Address     m_Address;
};

// bind expression ////////////////////////////////////////////////////////////

class MEDUSA_EXPORT BindExpression : public Expression
{
  DECL_EXPR(BindExpression, Expression::Kind::Bind, Expression)

public:
  explicit BindExpression(Expression::LSPType const& rExprs);
  virtual ~BindExpression();

  Expression::LSPType& GetBoundExpressions() { return m_Expressions; }

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

private:
  Expression::LSPType m_Expressions;
};

// condition expression ///////////////////////////////////////////////////////

class MEDUSA_EXPORT ConditionExpression : public Expression
{
public:
  using SPType = std::shared_ptr<ConditionExpression>;

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

  virtual ~ConditionExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Type GetType() const { return m_Type; }
  Type GetCondition() const { return m_Type; }
  Type GetOppositeCondition() const;
  Expression::SPType GetReferenceExpression() const { return m_spRefExpr; }
  Expression::SPType GetTestExpression() const { return m_spTestExpr; }

protected:
  Type m_Type;
  Expression::SPType m_spRefExpr;
  Expression::SPType m_spTestExpr;
};

class MEDUSA_EXPORT TernaryConditionExpression : public ConditionExpression
{
  DECL_EXPR(TernaryConditionExpression, Expression::Kind::TernaryCond, Expression)

public:
  TernaryConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spTrueExpr, Expression::SPType spFalseExpr);

  virtual ~TernaryConditionExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetTrueExpression() { return m_spTrueExpr; }
  Expression::SPType GetFalseExpression() { return m_spFalseExpr; }

protected:
  Expression::SPType m_spTrueExpr;
  Expression::SPType m_spFalseExpr;
};

class MEDUSA_EXPORT IfElseConditionExpression : public ConditionExpression
{
  DECL_EXPR(IfElseConditionExpression, Expression::Kind::IfElseCond, Expression)

public:
  IfElseConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spThenExpr, Expression::SPType spElseExpr);
  virtual ~IfElseConditionExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetThenExpression() { return m_spThenExpr; }
  Expression::SPType GetElseExpression() { return m_spElseExpr; }

protected:
  Expression::SPType m_spThenExpr;
  Expression::SPType m_spElseExpr;
};

class MEDUSA_EXPORT WhileConditionExpression : public ConditionExpression
{
  DECL_EXPR(WhileConditionExpression, Expression::Kind::WhileCond, Expression)

public:
  WhileConditionExpression(Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spBodyExpr);
  virtual ~WhileConditionExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const { return 0; }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetBodyExpression() { return m_spBodyExpr; }

protected:
  Expression::SPType m_spBodyExpr;
};

// operation expression ///////////////////////////////////////////////////////

class MEDUSA_EXPORT AssignmentExpression : public Expression
{
  DECL_EXPR(AssignmentExpression, Expression::Kind::Assign, Expression)

public:
  AssignmentExpression(Expression::SPType spDstExpr, Expression::SPType spSrcExpr);
  virtual ~AssignmentExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual Expression::SPType GetDestinationExpression() { return m_spDstExpr; }
  virtual Expression::SPType GetSourceExpression() { return m_spSrcExpr; }

private:
  Expression::SPType m_spDstExpr;
  Expression::SPType m_spSrcExpr;
};

class MEDUSA_EXPORT OperationExpression : public Expression
{
  DECL_EXPR(OperationExpression, Expression::Kind::Op, Expression)

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

    // Unary floating point operations
    OpFNeg,

    // Binary operations
    OpAnd,
    OpOr,
    OpXor,
    OpLls,         /* Logical Left Shift     */
    OpLrs,         /* Logical Right Shift    */
    OpArs,         /* Arithmetic Right Shift */
    OpRol,         /* Logical Left Rotate    */
    OpRor,         /* Logical Right Rotate   */
    OpAdd,
    OpSub,
    OpMul,
    OpSDiv,
    OpUDiv,
    OpSMod,
    OpUMod,
    OpSext,        /* Sign Extend */
    OpZext,        /* Zero Extend */
    OpInsertBits,  /* Insert bits  e.g. insert_bits (0x0000bbcc, 0x00ffff00) = 0x00bbcc00 */
    OpExtractBits, /* Extract bits e.g. extract_bits(0xaabbccdd, 0x00ffff00) = 0x0000bbcc */
    OpClearBits,   /* Clear bits   e.g. clear_bits  (0xaabbccdd, 0xff0000ff) = 0x00bbdd00 */
    OpBcast,       /* Bit Cast */

    // Binary floating point operations
    OpFAdd,
    OpFSub,
    OpFMul,
    OpFDiv,
    OpFMod,
  };

  //! pLeftExpr and pRightExpr must be allocated by standard new
  explicit OperationExpression(Type OpType);
  virtual ~OperationExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const { return nullptr; }
  virtual u32 GetBitSize() const;
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Type GetOperation() const;
  Type GetOppositeOperation() const;

protected:
  Type m_OpType;
};

class MEDUSA_EXPORT UnaryOperationExpression : public OperationExpression
{
  DECL_EXPR(UnaryOperationExpression, Expression::Kind::UnOp, Expression)

public:
  UnaryOperationExpression(Type OpType, Expression::SPType spExpr);
  virtual ~UnaryOperationExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetExpression() { return m_spExpr;  }

private:
  Expression::SPType m_spExpr;
};

class MEDUSA_EXPORT BinaryOperationExpression : public OperationExpression
{
  DECL_EXPR(BinaryOperationExpression, Expression::Kind::BinOp, Expression)

public:
  BinaryOperationExpression(Type OpType, Expression::SPType spLeftExpr, Expression::SPType spRightExpr);
  virtual ~BinaryOperationExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetLeftExpression()  { return m_spLeftExpr; }
  Expression::SPType GetRightExpression() { return m_spRightExpr; }

  void SwapExpressions() { std::swap(m_spLeftExpr, m_spRightExpr); }
  void SwapLeftExpression(Expression::SPType& rspExpr) { m_spLeftExpr.swap(rspExpr); }
  void SwapRightExpression(Expression::SPType& rspExpr) { m_spRightExpr.swap(rspExpr); }
  void SwapLeftExpressions(BinaryOperationExpression::SPType spOpExpr);

private:
  Expression::SPType m_spLeftExpr;
  Expression::SPType m_spRightExpr;
};

// constant expression ////////////////////////////////////////////////////////

class MEDUSA_EXPORT BitVectorExpression : public Expression
{
  DECL_EXPR(BitVectorExpression, Expression::Kind::Const, Expression)

public:
  BitVectorExpression(u16 BitSize, ap_int Value);
  explicit BitVectorExpression(BitVector const& rValue);
  virtual ~BitVectorExpression() = default;

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const { return m_Value.GetBitSize(); }
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  BitVector      GetInt() const { return m_Value; }

private:
  BitVector m_Value;
};

// class MEDUSA_EXPORT IntegerExpression : public BitVectorExpression
// {
//   DECL_EXPR(IntegerExpression, Expression::IntConst, Expression)

// public:
//   IntegerExpression(u16 BitSize, ap_int Value);
//   IntegerExpression(BitVector const& rValue);
//   virtual ~IntegerExpression() {}

//   virtual std::string ToString() const;
//   virtual Expression::SPType Clone() const;
//   virtual u32 GetBitSize() const { return m_Value.GetBitSize(); }
//   virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
//   virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
//   virtual CompareType Compare(Expression::SPType spExpr) const;

//   virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
//   virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
//   virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

//   BitVector      GetInt() const { return m_Value; }

// private:
//   BitVector m_Value;
// };

// class MEDUSA_EXPORT FloatingPointExpression : public BitVectorExpression
// {
//   DECL_EXPR(FloatingPointExpression, Expression::FloatConst, Expression)

// public:
//   union FloatingType
//   {
//     double dbl;
//     float sgl;
//   };

//   enum Precision
//   {
//     Double,
//     Single,
//   };

//   FloatingPointExpression(float const Value);
//   FloatingPointExpression(double const Value);
//   FloatingPointExpression(FloatingType const& rValue);
//   virtual ~FloatingPointExpression() {}

//   virtual std::string ToString() const;
//   virtual Expression::SPType Clone() const;
//   virtual u32 GetBitSize() const;
//   virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
//   virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
//   virtual CompareType Compare(Expression::SPType spExpr) const;

//   virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
//   virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
//   virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

//   float GetSingle() const { return m_Value.sgl; };
//   double GetDouble() const { return m_Value.dbl; };
//   u8 GetPrecision() const { return m_Precision; };

// private:
//   FloatingType m_Value;
//   u8 m_Precision;
// };


// identifier expression //////////////////////////////////////////////////////

class MEDUSA_EXPORT IdentifierExpression : public Expression
{
  DECL_EXPR(IdentifierExpression, Expression::Kind::Id, Expression)

public:
  IdentifierExpression(u32 Id, CpuInformation const* pCpuInfo);

  virtual ~IdentifierExpression() = default;

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  u32 GetId() const { return m_Id; }
  CpuInformation const* GetCpuInformation() const { return m_pCpuInfo; }

protected:
  u32 m_Id;
  CpuInformation const* m_pCpuInfo;
};

class MEDUSA_EXPORT VectorIdentifierExpression : public Expression
{
  DECL_EXPR(VectorIdentifierExpression, Expression::Kind::VecId, Expression);

public:
  VectorIdentifierExpression(std::vector<u32> const& rVecId, CpuInformation const* pCpuInfo);

  virtual ~VectorIdentifierExpression() = default;

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual void Prepare(DataContainerType& rData) const;
  virtual bool Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
  virtual bool Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
  virtual bool GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;

  std::vector<u32> GetVector() const { return m_VecId; }
  CpuInformation const* GetCpuInformation() const { return m_pCpuInfo; }

protected:
  std::vector<u32> m_VecId;
  CpuInformation const* m_pCpuInfo;
};

class MEDUSA_EXPORT TrackExpression : public Expression
{
  DECL_EXPR(TrackExpression, Expression::Kind::Track, Expression)

public:
  TrackExpression(Expression::SPType spTrkExpr, Address const& rCurAddr, u8 Pos);

  virtual ~TrackExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Expression::SPType GetTrackedExpression() const { return m_spTrkExpr; }
  Address GetTrackAddress() const { return m_CurAddr; }
  u8 GetTrackPosition() const { return m_Pos; }

private:
  Expression::SPType m_spTrkExpr;
  Address m_CurAddr;
  u8 m_Pos;
};

// variable ///////////////////////////////////////////////////////////////////

class MEDUSA_EXPORT VariableExpression : public Expression
{
  DECL_EXPR(VariableExpression, Expression::Kind::Var, Expression)

public:
  enum Type
  {
    Unknown,
    Alloc,
    Free,
    Use,
  };

  VariableExpression(std::string const& rVarName, Type VarType, u32 BitSize = 0);

  virtual ~VariableExpression();

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr) { return false; }
  virtual CompareType Compare(Expression::SPType spExpr) const;

  std::string const& GetName() const { return m_Name; }
  Type               GetType() const { return m_VarType; }

  void SetBitSize(u32 BitSize) { m_BitSize = BitSize; }

protected:
  std::string m_Name;
  Type        m_VarType;
  u32         m_BitSize;
};

// memory expression //////////////////////////////////////////////////////////

class MEDUSA_EXPORT MemoryExpression : public Expression
{
  DECL_EXPR(MemoryExpression, Expression::Kind::Mem, Expression)

public:
  MemoryExpression(u32 AccessSize, Expression::SPType spExprBase, Expression::SPType spExprOffset, bool Dereference = true);

  virtual ~MemoryExpression();

  u32 GetAccessSizeInBit() const { return m_AccessSizeInBit; }
  Expression::SPType GetBaseExpression() { return m_spBaseExpr; }
  Expression::SPType GetOffsetExpression() { return m_spOffExpr; }

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const;
  virtual Expression::SPType Visit(ExpressionVisitor* pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  virtual bool               Read(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData) const;
  virtual bool               Write(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, DataContainerType& rData);
  virtual bool               GetAddress(CpuContext *pCpuCtxt, MemoryContext* pMemCtxt, Address& rAddress) const;
  virtual Expression::SPType ToAddress() const;

  Expression::SPType GetAddressExpression() const { return m_spOffExpr; }
  bool IsDereferencable() const { return m_Dereference; }
private:
  u32 m_AccessSizeInBit;
  Expression::SPType m_spBaseExpr;
  Expression::SPType m_spOffExpr;
  bool m_Dereference;
};

// symbolic expression ////////////////////////////////////////////////////////

class MEDUSA_EXPORT SymbolicExpression : public Expression
{
  DECL_EXPR(SymbolicExpression, Expression::Kind::Sym, Expression)

public:
  enum Type
  {
    Unknown,
    ReturnedValue,
    FromParameter,
    ExternalValue,
    ExternalFunction,
    Undefined,
  };

  SymbolicExpression(Type SymType, std::string const& rValue, Address const& rAddr, Expression::SPType spExpr);

  virtual ~SymbolicExpression() = default;

  virtual std::string ToString() const;
  virtual Expression::SPType Clone() const;
  virtual u32 GetBitSize() const;
  virtual Expression::SPType Visit(ExpressionVisitor *pVisitor);
  virtual bool UpdateChild(Expression::SPType spOldExpr, Expression::SPType spNewExpr);
  virtual CompareType Compare(Expression::SPType spExpr) const;

  Type GetType() const { return m_Type; }
  std::string const& GetValue() const { return m_Value; }
  Address const& GetAddress() const { return m_Address; }
  Expression::SPType GetExpression() const { return m_spExpr; }

private:
  Type m_Type;
  std::string m_Value;
  Address m_Address;
  Expression::SPType m_spExpr;
};

// visitor ////////////////////////////////////////////////////////////////////

class MEDUSA_EXPORT ExpressionVisitor
{
public:
  virtual ~ExpressionVisitor() = default;

  virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
  virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
  virtual Expression::SPType VisitCondition(ConditionExpression::SPType spCondExpr);
  virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
  virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
  virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
  virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
  virtual Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spOpExpr);
  virtual Expression::SPType VisitBitVector(BitVectorExpression::SPType spIntExpr);
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
  MEDUSA_EXPORT Expression::SPType MakeBitVector(BitVector const& rValue);
  MEDUSA_EXPORT Expression::SPType MakeBitVector(u16 BitSize, ap_int Value);
  MEDUSA_EXPORT Expression::SPType MakeBoolean(bool Value);
  MEDUSA_EXPORT Expression::SPType MakeId(u32 Id, CpuInformation const* pCpuInfo);
  MEDUSA_EXPORT Expression::SPType MakeVecId(std::vector<u32> const& rVecId, CpuInformation const* pCpuInfo);
  MEDUSA_EXPORT Expression::SPType MakeTrack(Expression::SPType spTrkExpr, Address const& rCurAddr, u8 Pos);
  MEDUSA_EXPORT Expression::SPType MakeMem(u32 AccessSize, Expression::SPType spExprBase, Expression::SPType spExprOffset, bool Dereference = true);
  MEDUSA_EXPORT Expression::SPType MakeVar(std::string const& rName, VariableExpression::Type VarType, u16 BitSize = 0);

  MEDUSA_EXPORT Expression::SPType MakeCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr);
  MEDUSA_EXPORT Expression::SPType MakeTernaryCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spTrueExpr, Expression::SPType spFalseExpr);
  MEDUSA_EXPORT Expression::SPType MakeIfElseCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spThenExpr, Expression::SPType spElseExpr);
  MEDUSA_EXPORT Expression::SPType MakeWhileCond(ConditionExpression::Type CondType, Expression::SPType spRefExpr, Expression::SPType spTestExpr, Expression::SPType spBodyExpr);

  MEDUSA_EXPORT Expression::SPType MakeAssign(Expression::SPType spDstExpr, Expression::SPType spSrcExpr);
  MEDUSA_EXPORT Expression::SPType MakeUnOp(OperationExpression::Type OpType, Expression::SPType spExpr);
  MEDUSA_EXPORT Expression::SPType MakeBinOp(OperationExpression::Type OpType, Expression::SPType spLeftExpr, Expression::SPType spRightExpr);

  MEDUSA_EXPORT Expression::SPType MakeBind(Expression::LSPType const& rExprs);

  MEDUSA_EXPORT Expression::SPType MakeSym(SymbolicExpression::Type SymType, std::string const& rValue, Address const& rAddr, Expression::SPType spExpr = nullptr);
  MEDUSA_EXPORT Expression::SPType MakeSys(std::string const& rName, Address const& rAddr);

  [[nodiscard]] MEDUSA_EXPORT bool TestKind(Expression::Kind Kind, Expression::SPType spExpr);
}


MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXPRESSION_HPP_HPP
