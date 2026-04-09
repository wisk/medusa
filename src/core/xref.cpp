#include "medusa/xref.hpp"

MEDUSA_NAMESPACE_BEGIN

bool XRefs::AddXRef(Address const& rTo, Address const& rFrom)
{
  m_ToFrom.emplace(rTo, rFrom);
  m_FromTo.emplace(rFrom, rTo);
  return true;
}

bool XRefs::RemoveRef(Address const& rFrom)
{
  auto range = m_FromTo.equal_range(rFrom);
  for (auto it = range.first; it != range.second; ++it)
  {
    // Remove corresponding entries from m_ToFrom
    auto revRange = m_ToFrom.equal_range(it->second);
    for (auto revIt = revRange.first; revIt != revRange.second; )
    {
      if (revIt->second == rFrom)
        revIt = m_ToFrom.erase(revIt);
      else
        ++revIt;
    }
  }
  m_FromTo.erase(range.first, range.second);
  return true;
}

bool XRefs::From(Address const& rTo, Address::Vector& rFrom) const
{
  auto range = m_ToFrom.equal_range(rTo);
  for (auto it = range.first; it != range.second; ++it)
    rFrom.push_back(it->second);
  return !rFrom.empty();
}

bool XRefs::To(Address const& rFrom, Address::Vector& rTo) const
{
  auto range = m_FromTo.equal_range(rFrom);
  for (auto it = range.first; it != range.second; ++it)
    rTo.push_back(it->second);
  return !rTo.empty();
}

bool XRefs::HasXRefTo(Address const& rFrom) const
{
  return m_FromTo.find(rFrom) != m_FromTo.end();
}

bool XRefs::HasXRefFrom(Address const& rTo) const
{
  return m_ToFrom.find(rTo) != m_ToFrom.end();
}

void XRefs::EraseAll()
{
  m_ToFrom.clear();
  m_FromTo.clear();
}

MEDUSA_NAMESPACE_END
