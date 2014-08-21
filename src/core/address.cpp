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
    case 'p': m_Type = PhysicalType; break;
    case 'f': m_Type = FlatType;     break;
    case 's': m_Type = SegmentType;  break;
    case 'b': m_Type = BankType;     break;
    case 'v': m_Type = VirtualType;  break;
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
  case PhysicalType: TypeChr = 'p'; break;
  case FlatType:     TypeChr = 'f'; break;
  case SegmentType:  TypeChr = 's'; break;
  case BankType:     TypeChr = 'b'; break;
  case VirtualType:  TypeChr = 'v'; break;
  default:                          break;
  }
  std::ostringstream oss;

  oss << std::hex << std::noshowbase << std::setfill('0');

  oss << "addr(" << TypeChr << " ";

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