#ifndef MEDUSA_XREF_HPP
#define MEDUSA_XREF_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

#include <unordered_map>

MEDUSA_NAMESPACE_BEGIN

//! XRefs handles cross-references using two synchronized multimaps.
class MEDUSA_EXPORT XRefs
{
public:
   XRefs(void) {}
  ~XRefs(void) {}

  using XRefMapType = std::unordered_multimap<Address, Address, std::hash<Address>>;

  //! Add new cross-reference
  bool AddXRef(Address const& rTo, Address const& rFrom);

  //! Remove a cross-reference by source address
  bool RemoveRef(Address const& rFrom);

  //! Get all addresses which access to a specific address
  bool From(Address const& rTo, Address::Vector& rFrom) const;

  //! Get accessed address from a specific address
  bool To(Address const& rFrom, Address::Vector& rTo) const;

  //! Test if a specific address has a cross-reference
  bool HasXRefTo(Address const& rFrom) const;

  //! Test if a specific address has a cross-reference
  bool HasXRefFrom(Address const& rTo) const;

  //! Erase all cross-references
  void EraseAll(void);

  //! Get the To→From map (left side)
  XRefMapType const& GetXRefsFrom(void) const { return m_ToFrom; }

private:
  XRefMapType m_ToFrom;  // key=To, value=From
  XRefMapType m_FromTo;  // key=From, value=To
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_XREF_HPP
