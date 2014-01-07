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

std::string Address::Dump(void) const
{
  char TypeChr = '?';
  switch (m_Type)
  {
  case PhysicalType: TypeChr = 'p'; break;
  case FlatType:     TypeChr = 'f'; break;
  case SegmentType:  TypeChr = 's'; break;
  case BankType:     TypeChr = 'b'; break;
  case VirtualType:  TypeChr = 'v'; break;
  default:                          break;
  }
  std::ostringstream oss;

  oss << std::hex << std::showbase << std::setfill('0');

  oss << "addr(" << TypeChr << ", ";

  if (m_Type != FlatType && m_Type != UnknownType)
    oss << std::setw(m_BaseSize / 4) << m_Base << ":";

  oss << std::setw(m_OffsetSize / 4) << m_Offset << ")";
  return oss.str();
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