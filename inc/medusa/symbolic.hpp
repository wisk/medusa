#ifndef __MEDUSA_SYMBOLIC_HPP__
#define __MEDUSA_SYMBOLIC_HPP__

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/document.hpp"
#include "medusa/information.hpp"
#include "medusa/expression.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Symbolic
{
private:
  class TaintedBlock
  {
  public:
    //! This method clone the expression automatically.
    void TaintRegister(Address const& rAddr, u32 RegId, Expression const* pExpr);

    ~TaintedBlock(void);

  private:
    typedef std::unordered_map<u32, Expression*> TaintedRegisterMap;
    typedef std::map<Address, TaintedRegisterMap> AddressedTaintedRegisterMap;
    AddressedTaintedRegisterMap m_TaintedRegisters;
  };


public:
  class Medusa_EXPORT Context
  {

  };


  Symbolic(Document &rDoc);
  ~Symbolic(void);

  typedef std::function<bool (Symbolic::Context const& rSymCtxt, Address const& rCurAddr, Address::List& rNextAddresses)> Callback;

  bool TaintRegister(u32 RegId, Address const& rAddr, Callback Cb);
  bool TaintRegister(CpuInformation::Type RegType, Address const& rAddr, Callback Cb);

private:
  bool _TaintBlock(Address const& rBlkAddr, Symbolic::TaintedBlock& rBlk);

  Document& m_rDoc;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_SYMBOLIC_HPP__