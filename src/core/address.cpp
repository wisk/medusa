#include "medusa/address.hpp"

MEDUSA_NAMESPACE_BEGIN

Address::Address(std::string const& rAddrName)
{
  m_Base       = 0;
  m_Offset     = 0;
  m_BaseSize   = 0;
  m_OffsetSize = static_cast<u8>(rAddrName.length() * 4);
  m_Type       = UnknownType;

  std::istringstream iss(rAddrName);
  std::string::size_type BaseSize;

  if ((BaseSize = rAddrName.find_first_of(':')) != std::string::npos)
  {
    m_BaseSize    = static_cast<u8>( BaseSize * 4);
    m_OffsetSize -= static_cast<u8>((BaseSize * 4) - sizeof(':'));
    iss >> std::hex >> m_Base;
    iss.ignore();
  }

  iss >> std::hex >> m_Offset;
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