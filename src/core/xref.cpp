#include "medusa/xref.hpp"
#include <boost/foreach.hpp>

MEDUSA_NAMESPACE_BEGIN

bool XRefs::AddXRef(Address const& rTo, Address const& rFrom)
{
  m_XRefs.insert(TXRefs::value_type(rTo, rFrom));
  return true;
}

bool XRefs::RemoveRef(Address const& rFrom)
{
  m_XRefs.right.erase(rFrom);
  return true;
}

bool XRefs::From(Address const& rTo, TAddressList& rFromList)
{
  for (TXRefs::left_iterator It = m_XRefs.left.begin(); It != m_XRefs.left.end(); ++It)
    if (It->first == rTo)
      rFromList.push_back(It->second);

  return !rFromList.empty();
}

bool XRefs::To(Address const& rFrom, Address& rTo)
{
  try
  {
    rTo = m_XRefs.right.at(rFrom);
  }
  catch (std::out_of_range)
  {
    return false;
  }

  return true;
}

MEDUSA_NAMESPACE_END
