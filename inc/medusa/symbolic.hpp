#ifndef MEDUSA_SYMBOLIC_HPP
#define MEDUSA_SYMBOLIC_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/instruction.hpp"
#include "medusa/export.hpp"
#include "medusa/document.hpp"
#include "medusa/information.hpp"
#include "medusa/expression.hpp"

#include <tuple>

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT Symbolic
{
private:

  class Block
  {
  public:
    using ExpressionMatcher = std::function<bool(Expression::SPType)>;

    Block(u32 PcRegid = 0);
    Block(Block const& rBlk);
    Block& operator=(Block const& rBlk);

    ~Block();

    void SetPcRegisterId(u32 PcRegId) { m_PcRegId = PcRegId; }

    void TrackExpression(Address const& rAddr, Track::Context& rTrackCtxt, Expression::SPType spExpr);
    void BackTrackId(Address const& rAddr, u32 Id, Expression::LSPType& rExprs) const;

    void AddExpression(Expression::SPType spExpr);
    Expression::SPType BackTrackExpression(ExpressionMatcher Matcher) const;
    Expression::LSPType const& GetTrackedExpressions() const { return m_TrackedExprs; }

    void AddParentBlock(Address const& rAddr);
    std::set<Address> const& GetParentBlocks() const;

    void AddConditionalExpression(Expression const* pExpr);
    Expression::LSPType const& GetConditionalExpressions() const;

    bool Contains(Address const& rAddr) const;
    Address GetLastAddress() const;
    bool IsEndOfBlock() const;

    void ForEachAddress(std::function<bool(Address const& rAddress)> Callback) const;

  private:
    u32                 m_PcRegId;
    std::set<Address>   m_Addresses;
    std::set<Address>   m_ParentBlocks;
    Expression::LSPType m_TrackedExprs;
    Expression::LSPType m_CondExprs;
  };


public:
  class MEDUSA_EXPORT Context
  {
  public:
    bool AddBlock(Address const& rBlkAddr, Block &rBlk);

    Expression::LSPType BacktrackRegister(Address const& RegAddr, u32 RegId) const;
    Expression::VSPType GetExpressions() const;

    Track::Context& GetTrackContext() { return m_TrackCtxt; }

    std::string ToString() const;

  private:
    Track::Context m_TrackCtxt;
    std::map<Address, Block> m_Blocks;
  };

  explicit Symbolic(Document &rDoc);
  ~Symbolic();

  using Callback = std::function<bool (Symbolic::Context const& rSymCtxt, Address const& rCurAddr, Address::Vector& rNextAddresses)>;

  void FollowFunction(bool FollowFunction);

  bool Execute(Address const& rAddr, Callback Cb);

private:
  bool _ExecuteBlock(Symbolic::Context& rCtxt, Address const& rBlkAddr, Symbolic::Block& rBlk);

  bool _DetermineNextAddresses(Symbolic::Context& rSymCtxt, Instruction const& rInsn, Address const& rCurAddr, Address::Vector& rNextAddresses) const;
  bool _ApplyCallingEffect(Address const& rImpFunc, Track::Context& rTrkCtxt, Symbolic::Block& rBlk);

  Document& m_rDoc;
  CpuInformation const* m_pCpuInfo;
  u32 m_PcRegId;
  u32 m_SpRegId;
  bool m_FollowFunction;
};

class MEDUSA_EXPORT Symbolic2
{
public:
  explicit Symbolic2(Document& rDoc);

  bool AddBlock(Address const& rAddr);
  bool GetBlock(Address const& rAddr, Expression::VSPType& rBlocks);

private:
  bool _DisassembleBasicBlock(Address const& rAddr, Expression::VSPType& rBlocks);

  Document& m_rDoc;
  std::unordered_map<Address, Expression::VSPType> m_Blocks;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_SYMBOLIC_HPP