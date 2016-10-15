#include "medusa/address.hpp"

#include <boost/algorithm/string.hpp>
#include <medusa/exception.hpp>

MEDUSA_NAMESPACE_BEGIN

Address::Address(std::string const& rAddrName)
{
  std::string AddrStr = rAddrName;
  boost::algorithm::trim(AddrStr);
  m_Base       = 0;
  m_Offset     = 0;
  m_BaseSize   = 0;
  m_OffsetSize = static_cast<u8>(AddrStr.length() * 4);
  m_Type       = UnknownType;

  if (AddrStr.compare(0, 5, "addr(") == 0)
  {
    switch (AddrStr[5])
    {
    case 'p': m_Type = PhysicalType;     break;
    case 'a': m_Type = ArchitectureType; break;
    case 'e': m_Type = LinearType;       break;
    case 'r': m_Type = RelativeType;     break;
    case 'l': m_Type = LogicalType;      break;
    }
    AddrStr.erase(std::begin(AddrStr),   std::begin(AddrStr) + 7); // erase ^addr(?\s
    AddrStr.erase(std::end(AddrStr) - 1, std::end(AddrStr)      ); // erase )$
  }

  if (AddrStr.compare(0, 2, "0x") == 0)
    AddrStr.erase(std::begin(AddrStr), std::begin(AddrStr) + 2); // erase 0x

  std::string::size_type BaseOffset;
  if ((BaseOffset = AddrStr.find_first_of(':')) != std::string::npos)
  {
    std::istringstream issBase(AddrStr);
    issBase >> m_Base;
    m_BaseSize = static_cast<u8>(BaseOffset * 4);
    AddrStr.erase(std::begin(AddrStr), std::begin(AddrStr) + BaseOffset + 1); // erase base:

    if (AddrStr.compare(0, 2, "0x") == 0)
      AddrStr.erase(std::begin(AddrStr), std::begin(AddrStr) + 2); // erase 0x
  }

  std::istringstream issOffset(AddrStr);
  m_OffsetSize = static_cast<u8>(AddrStr.length() * 4);
  issOffset >> std::hex >> m_Offset;
}

std::string Address::Dump(void) const
{
  char TypeChr = '?';
  switch (m_Type)
  {
  case PhysicalType:     TypeChr = 'p'; break;
  case ArchitectureType: TypeChr = 'a'; break;
  case LinearType:       TypeChr = 'e'; break;
  case RelativeType:     TypeChr = 'r'; break;
  case LogicalType:      TypeChr = 'l'; break;
  default:                          break;
  }
  std::ostringstream oss;

  oss << std::hex << std::noshowbase << std::setfill('0');

  oss << "addr(" << TypeChr << " ";

  if (m_Type == LogicalType)
    oss << std::setw(m_BaseSize / 4) << m_Base << ":";

  oss << std::setw(m_OffsetSize / 4) << m_Offset << ")";
  return oss.str();
}

std::string Address::ToString(void) const
{
  std::ostringstream oss;

  oss << std::hex << std::setfill('0');

  switch (m_Type)
  {
  case PhysicalType:
    oss << "phys:";
    break;
  case RelativeType:
    oss << "rva:";
    break;
  case LogicalType:
      oss << std::setw(m_BaseSize / 4) << m_Base << ":";
      break;
  default:
    break;
  }

  oss << std::setw(m_OffsetSize / 4) << m_Offset;
  return oss.str();
}

OffsetType Address::SanitizeOffset(OffsetType Offset) const
{
  switch (m_OffsetSize)
  {
  case  8: return Offset & 0xff;
  case 16: return Offset & 0xffff;
  case 32: return Offset & 0xffffffff;
  case 64: return Offset & 0xffffffffffffffff;
  default: return Offset;
  }
}

void Address::SanitizeOffset(OffsetType& rOffset)
{
  switch (m_OffsetSize)
  {
  case  8: rOffset &= 0xff;
  case 16: rOffset &= 0xffff;
  case 32: rOffset &= 0xffffffff;
  case 64: rOffset &= 0xffffffffffffffff;
  default: break;
  }
}

bool Address::IsBetween(u64 Size, OffsetType Off) const
{
  return Off >= m_Offset && Off < m_Offset + Size;
}

bool Address::IsBetween(u64 Size, Address const& Addr) const
{
  //if (m_Type != Addr.m_Type)
  //  return false;
  if (Addr.m_Type != Address::UnknownType && Addr.m_Base != m_Base)
    return false;
  return Addr.m_Offset >= m_Offset && Addr.m_Offset < m_Offset + Size;
}

bool Address::operator==(Address const& rAddr) const
{
  return m_Base == rAddr.m_Base && m_Offset == rAddr.m_Offset;
}

bool Address::operator!=(Address const& rAddr) const
{
  return !(*this == rAddr);
}

Address Address::operator+(OffsetType Off) const
{
  Address Res = Address(m_Type, m_Base, SanitizeOffset(m_Offset + Off), m_BaseSize, m_OffsetSize);
  Res.SanitizeOffset();
  return Res;
}

Address Address::operator+(Address const& Addr) const
{
  Address Res = Address(m_Type, m_Base, SanitizeOffset(m_Offset + Addr.m_Offset), m_BaseSize, m_OffsetSize);
  Res.SanitizeOffset();
  return Res;
}

Address Address::operator+=(OffsetType Off)
{
  m_Offset = SanitizeOffset(m_Offset + Off);
  return *this;
}

bool Address::operator<(Address const& rAddr) const
{
  if (m_Base < rAddr.m_Base)
    return true;
  else if (m_Base == rAddr.m_Base)
    return m_Offset < rAddr.m_Offset;
  else
    return false;
}

bool Address::operator<=(Address const& rAddr) const
{
  if (m_Base < rAddr.m_Base)
    return true;
  else if (m_Base == rAddr.m_Base)
    return m_Offset <= rAddr.m_Offset;
  else
    return false;
}

bool Address::operator>(Address const& rAddr) const
{
  if (m_Base > rAddr.m_Base)
    return true;
  else if (m_Base == rAddr.m_Base)
    return m_Offset > rAddr.m_Offset;
  else
    return false;
}

bool Address::operator>=(Address const& rAddr) const
{
  if (m_Base > rAddr.m_Base)
    return true;
  else if (m_Base == rAddr.m_Base)
    return m_Offset >= rAddr.m_Offset;
  else
    return false;
}

MEDUSA_NAMESPACE_END

std::ostream& operator<<(std::ostream& rOstrm, medusa::Address const& rAddr)
{
  rOstrm << rAddr.ToString();
  return rOstrm;
}

std::istream& operator>>(std::istream& rIstrm, medusa::Address& rAddr)
{
  std::string AddrStr;
  bool OpenParen = false;
  bool Parse = true;
  while (rIstrm.good() && Parse)
  {
    char CurChr = rIstrm.get();
    switch (CurChr)
    {
    case '(': OpenParen = true; break;
    case ')': Parse = false; break;
    }
    AddrStr += CurChr;
  }
  rAddr = medusa::Address(AddrStr);
  return rIstrm;
}
