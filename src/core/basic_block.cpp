#include "medusa/basic_block.hpp"

MEDUSA_NAMESPACE_USE

Address BasicBlockVertexProperties::GetFirstAddress(void) const
{
  if (m_Addresses.empty()) return Address();
  return m_Addresses.front();
}

Address BasicBlockVertexProperties::GetLastAddress(void) const
{
  if (m_Addresses.empty()) return Address();
  return m_Addresses.back();
}

bool BasicBlockVertexProperties::Contains(Address const& rAddr) const
{
  return std::find(std::begin(m_Addresses), std::end(m_Addresses), rAddr) != std::end(m_Addresses);
}

bool BasicBlockVertexProperties::Split(Address const& rAddr, Address::List& rAddresses)
{
  if (GetFirstAddress() == rAddr) return false;
  if (m_Addresses.size() < 2)     return false;

  auto itAddr = std::find(std::begin(m_Addresses), std::end(m_Addresses), rAddr);
  if (itAddr == std::end(m_Addresses)) return false;

  rAddresses.splice(std::begin(rAddresses), m_Addresses, itAddr, std::end(m_Addresses));

  return true;
}

bool BasicBlockVertexProperties::GetPreviousAddress(Address const& rAddr, Address& rPrevAddr) const
{
  auto itAddr = std::find(std::begin(m_Addresses), std::end(m_Addresses), rPrevAddr);
  if (itAddr == std::end(m_Addresses))
    return false;
  if (itAddr == std::begin(m_Addresses))
    return false;
  --itAddr;
  rPrevAddr = *itAddr;
  return true;
}

bool BasicBlockVertexProperties::GetNextAddress(Address const& rAddr, Address& rNextAddr) const
{
  auto itAddr = std::find(std::begin(m_Addresses), std::end(m_Addresses), rNextAddr);
  if (itAddr == std::end(m_Addresses))
    return false;
  ++itAddr;
  if (itAddr == std::end(m_Addresses))
    return false;
  rNextAddr = *itAddr;
  return true;
}