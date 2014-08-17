#ifndef __MEDUSA_SYMBOLIC_HPP__
#define __MEDUSA_SYMBOLIC_HPP__

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/document.hpp"
#include "medusa/information.hpp"
#include "medusa/expression.hpp"

#include <tuple>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Symbolic
{
private:

  typedef std::tuple<u32, Address> TaintedRegister;

  class TaintedBlock
  {
  public:
    TaintedBlock(void);
    ~TaintedBlock(void);

    TaintedBlock& operator=(TaintedBlock const& rBlk);
    TaintedBlock(TaintedBlock const& rTaintedBlock);

    //! This method clone the expression automatically.
    void TaintExpression(Address const& rAddr, Taint::Context& rTaintCtxt, Expression const* pExpr);

    void AddParentBlock(Address const& rAddr);
    std::set<Address> const& GetParentBlocks(void) const;

    void AddConditionalExpression(Expression const* pExpr);
    Expression::List const& GetConditionalExpressions(void) const;

    bool Contains(Address const& rAddr) const;

    void ForEachAddress(std::function<bool(Address const& rAddress)> Callback) const;

  private:
    std::set<Address> m_Addresses;
    std::set<Address> m_ParentBlocks;
    Expression::List m_TaintedExprs;
    Expression::List m_CondExprs;
  };


public:
  class Medusa_EXPORT Context
  {
  public:
    bool AddBlock(Address const& rBlkAddr, TaintedBlock &rBlk);

    std::list<Expression const*> BacktrackRegister(Address const& RegAddr, u32 RegId) const;

  private:
    std::map<Address, TaintedBlock> m_TaintedBlocks;
  };

  Symbolic(Document &rDoc);
  ~Symbolic(void);

  typedef std::function<bool (Symbolic::Context const& rSymCtxt, Address const& rCurAddr, Address::List& rNextAddresses)> Callback;

  bool TaintRegister(u32 RegId, Address const& rAddr, Callback Cb);

private:
  bool _TaintBlock(Address const& rBlkAddr, Symbolic::TaintedBlock& rBlk);

  bool _DetermineNextAddresses(Symbolic::Context const& rSymCtxt, Address const& rCurAddr, Address::List& rNextAddresses) const;

  Document& m_rDoc;
  u32 m_PcRegId;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_SYMBOLIC_HPP__