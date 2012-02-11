#include "medusa/address.hpp"

MEDUSA_NAMESPACE_BEGIN

Address::Address(std::string const& rAddrName)
{
  m_Base       = 0;
  m_Offset     = 0;
  m_BaseSize   = 16;
  m_OffsetSize = 64;
  m_Type       = UnknownType;

  std::istringstream iss(rAddrName);

  if (rAddrName.find_first_not_of(':') != std::string::npos)
  {
    iss >> m_Base;
    iss.ignore();
  }

  iss >> m_Offset;
}

MEDUSA_NAMESPACE_END

std::ostream& operator<<(std::ostream& rOstrm, medusa::Address const& rAddr)
{
  rOstrm << rAddr.ToString();
  return rOstrm;
}

std::wostream& operator<<(std::wostream& rOstrm, medusa::Address const& rAddr)
{
  rOstrm << rAddr.ToWString();
  return rOstrm;
}