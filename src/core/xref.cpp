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
  auto const& rLeft = m_XRefs.left;
  auto itXRef = rLeft.find(rTo);

  for (; itXRef != std::end(rLeft) && itXRef->first == rTo; ++itXRef)
    rFromList.push_back(itXRef->second);

  return !rFromList.empty();
}

bool XRefs::To(Address const& rFrom, Address::List& rToList) const
{
  auto const& rRight = m_XRefs.right;
  auto itXRef = rRight.find(rFrom);

  for (; itXRef != std::end(rRight) && itXRef->first == rFrom; ++itXRef)
    rToList.push_back(itXRef->second);

  return !rToList.empty();
}

bool XRefs::HasXRefTo(Address const& rFrom) const
{
  return m_XRefs.right.find(rFrom) != std::end(m_XRefs.right);
}

bool XRefs::HasXRefFrom(Address const& rTo) const
{
  return m_XRefs.left.find(rTo) != std::end(m_XRefs.left);
}

void XRefs::EraseAll(void)
{
  m_XRefs.erase(m_XRefs.begin(), m_XRefs.end());
}

MEDUSA_NAMESPACE_END