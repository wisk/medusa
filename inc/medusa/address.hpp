#ifndef _MEDUSA_ADDRESS_
#define _MEDUSA_ADDRESS_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <boost/shared_ptr.hpp>

MEDUSA_NAMESPACE_BEGIN

/*!
 * This class holds an address in a generic way.
 */

class Medusa_EXPORT Address
{
public:
  typedef enum
  {
    UnknownType,  //! Unknown type, used only in case of error or request.
    PhysicalType, //! Physical type, unused.
    FlatType,     //! Flat type, unused.
    SegmentType,  //! Segment type, unused (designed for real mode).
    BankType,     //! Bank type, used for video games consoles.
    VirtualType,  //! Virtual type, used for protected mode.
  } Type;

  typedef std::list<Address> List;
  typedef boost::shared_ptr<Address> SPtr;

  /*!
   * \param Type defines the kind of address.
   * \param Base is the base of this address.
   * \param Offset is the offset of this address.
   * \param BaseSize defines the base size in bits.
   * \param OffsetSize defines the offset size in bits.
   */
  Address(Type Type, TBase Base, TOffset Offset, u8 BaseSize, u8 OffsetSize)
    : m_Type(Type)
    , m_Base(Base)
    , m_Offset(Offset)
    , m_BaseSize(BaseSize)
    , m_OffsetSize(OffsetSize)
  { SanitizeOffset(); }

  Address(TBase Base, TOffset Offset, u8 BaseSize, u8 OffsetSize)
    : m_Type(FlatType)
    , m_Base(Base)
    , m_Offset(Offset)
    , m_BaseSize(BaseSize)
    , m_OffsetSize(OffsetSize)
  { SanitizeOffset(); }

  Address(Type Type, TBase Base, TOffset Offset)
    : m_Type(Type)
    , m_Base(Base)
    , m_Offset(Offset)
    , m_BaseSize(16)
    , m_OffsetSize(64)
  {}

  Address(TBase Base, TOffset Offset)
    : m_Type(FlatType)
    , m_Base(Base)
    , m_Offset(Offset)
    , m_BaseSize(16)
    , m_OffsetSize(64)
  {}

  Address(Type Type, TOffset Offset)
    : m_Type(Type)
    , m_Base(0x0)
    , m_Offset(Offset)
    , m_BaseSize(16)
    , m_OffsetSize(64)
  {}

  Address(TOffset Offset)
    : m_Type(FlatType)
    , m_Base(0x0)
    , m_Offset(Offset)
    , m_BaseSize(16)
    , m_OffsetSize(64)
  {}

  Address(std::string const& rAddrName);

  Address(void)
    : m_Type(UnknownType)
    , m_Base(0x0)
    , m_Offset(0x0)
    , m_BaseSize(16)
    , m_OffsetSize(64)
  {}

  //! This method allows to mask an offset accordingly the current offset size.
  TOffset SanitizeOffset(TOffset Offset) const
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

  //! This method allows to mask an offset accordingly the current offset size.
  void SanitizeOffset(TOffset& rOffset)
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

  //! This method converts the current address to a string.
  std::string ToString(void) const
  {
    std::ostringstream oss;

    oss << std::hex << std::setfill('0');

    if (m_Type != FlatType)
      oss << std::setw(m_BaseSize / 4) << m_Base << ":";

    oss << std::setw(m_OffsetSize / 4) << m_Offset;
    return oss.str();
  }

  //! This method converts the current address to a wstring.
  std::wstring ToWString(void) const
  {
    std::wostringstream oss;

    oss << std::hex << std::setfill(L'0');

    if (m_Type != FlatType)
      oss << std::setw(m_BaseSize / 4) << m_Base << L":";

    oss << std::setw(m_OffsetSize / 4) << m_Offset;
    return oss.str();
  }

  Type      GetAddressingType(void) const   { return m_Type;                       }
  TBase     GetBase(void) const             { return m_Base;                       }
  TOffset   GetOffset(void) const           { return m_Offset;                     }
  u8        GetBaseSize(void) const         { return m_BaseSize;                   }
  u8        GetOffsetSize(void) const       { return m_OffsetSize;                 }

  void      SetOffset(TOffset Offset)       { m_Offset = Offset; SanitizeOffset(); }

  /*! \param Size is the size of the boundary.
   * \param Off is the offset of the boundary.
   * \return Returns true if this address is contained in [Off:Off+Size].
   */
  bool IsBetween(u64 Size, TOffset Off) const
  {
    return Off >= m_Offset && Off < m_Offset + Size;
  }

  bool IsBetween(u64 Size, Address const& Addr) const
  {
    if (Addr.m_Type != Addr.m_Type)
      return false;
    if (Addr.m_Type != Address::UnknownType && Addr.m_Base != m_Base)
      return false;
    return Addr.m_Offset >= m_Offset && Addr.m_Offset < m_Offset + Size;
  }

  //! This method returns true if base and offset are egal.
  bool operator==(Address const& rAddr) const
  {
    return m_Base == rAddr.m_Base && m_Offset == rAddr.m_Offset;
  }

  //! This method returns an address where its offset is the current offset plus Off.
  Address operator+(TOffset Off) const
  {
    Address Res = Address(m_Type, m_Base, SanitizeOffset(m_Offset + Off), m_BaseSize, m_OffsetSize);
    Res.SanitizeOffset();
    return Res;
  }

  Address operator+(Address const& Addr) const
  {
    Address Res = Address(m_Type, m_Base, SanitizeOffset(m_Offset + Addr.m_Offset), m_BaseSize, m_OffsetSize);
    Res.SanitizeOffset();
    return Res;
  }

  //! This method add a offset to the current address offset.
  Address operator+=(TOffset Off)
  {
    m_Offset = SanitizeOffset(m_Offset + Off);
    return *this;
  }

  //! This method returns true if both base and offset are inferior to rAddr.
  bool operator<(Address const& rAddr) const
  {
    if (m_Base < rAddr.m_Base)
      return true;
    else if (m_Base == rAddr.m_Base)
      return m_Offset < rAddr.m_Offset;
    else
      return false;
  }

protected:
  void SanitizeOffset(void) { SanitizeOffset(m_Offset); }

private:
  Type      m_Type;
  TBase     m_Base;
  TOffset   m_Offset;
  u8        m_BaseSize;
  u8        m_OffsetSize;
};

MEDUSA_NAMESPACE_END

Medusa_EXPORT std::ostream& operator<<(std::ostream& rOstrm, medusa::Address const& rAddr);
Medusa_EXPORT std::wostream& operator<<(std::wostream& rOstrm, medusa::Address const& rAddr);

#endif // _MEDUSA_ADDRESS_