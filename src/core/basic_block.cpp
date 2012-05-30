#include "medusa/basic_block.hpp"

MEDUSA_NAMESPACE_USE

Address BasicBlockVertexProperties::GetFirstAddress(void) const
{
  if (m_Addresses.size() == 0) return Address();
  return *m_Addresses.begin();
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