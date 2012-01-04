#ifndef _MEDUSA_XREF_
#define _MEDUSA_XREF_

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

#include <list>
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>

MEDUSA_NAMESPACE_BEGIN

//! XRefs handles a cross-reference.
class XRefs
{
public:
   XRefs(void) {}
  ~XRefs(void) {}

  typedef boost::bimap<
    boost::bimaps::multiset_of<Address>,
    Address
  > TXRefs;

  typedef std::list<Address> TAddressList;

  bool AddXRef(Address const& rTo, Address const& rFrom);
  bool RemoveRef(Address const& rFrom);
  bool From(Address const& rTo, TAddressList& rFromList);
  bool To(Address const& rFrom, Address& rTo);

  void EraseAll(void);

  TXRefs const& GetAllXRefs(void) const { return m_XRefs; }

private:
  TXRefs m_XRefs;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_XREF_
