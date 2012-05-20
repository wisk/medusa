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

bool XRefs::From(Address const& rTo, Address::List& rFromList) const
{
  for (TXRefs::left_const_iterator It = m_XRefs.left.begin(); It != m_XRefs.left.end(); ++It)
    if (It->first == rTo)
      rFromList.push_back(It->second);

  return !rFromList.empty();
}

bool XRefs::To(Address const& rFrom, Address& rTo) const
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

bool XRefs::HasXRef(Address const& rFrom) const
{
  for (TXRefs::right_const_iterator It = m_XRefs.right.begin(); It != m_XRefs.right.end(); ++It)
    if (It->first == rFrom)
      return true;

  return false;
}

void XRefs::EraseAll(void)
{
  m_XRefs.erase(m_XRefs.begin(), m_XRefs.end());
}

MEDUSA_NAMESPACE_END