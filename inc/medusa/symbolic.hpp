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
public:
  class Medusa_EXPORT Context
  {
  };


  Symbolic(Document &rDoc);
  ~Symbolic(void);

  typedef std::function<bool (Symbolic::Context const& rSymCtxt, Address const& rAddr)> Callback;

  bool TaintRegister(u32 RegId, Address const& rAddr, Callback Cb);
  bool TaintRegister(CpuInformation::Type RegType, Address const& rAddr, Callback Cb);

private:
  Document& m_rDoc;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_SYMBOLIC_HPP__