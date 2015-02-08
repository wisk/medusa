#ifndef MEDUSA_ADDRESS_HPP
#define MEDUSA_ADDRESS_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <functional>
#include <cstdio>

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
    LogicalType,  //!
  } Type;

  typedef std::list<Address> List;
  typedef std::vector<Address> Vector;
  typedef std::shared_ptr<Address> SPType;

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
    : m_Type(LogicalType)
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
    : m_Type(LogicalType)
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

  std::string Dump(void) const;

  //! This method converts the current address to a string.
  std::string ToString(void) const;

  //! This method allows to mask an offset accordingly the current offset size.
  TOffset SanitizeOffset(TOffset Offset) const;
  void SanitizeOffset(TOffset& rOffset);

  Type      GetAddressingType(void) const   { return m_Type;                       }
  TBase     GetBase(void) const             { return m_Base;                       }
  TOffset   GetOffset(void) const           { return m_Offset;                     }
  u8        GetBaseSize(void) const         { return m_BaseSize;                   }
  u8        GetOffsetSize(void) const       { return m_OffsetSize;                 }

  void      SetBase(TBase Base)             { m_Base = Base;                       }
  void      SetOffset(TOffset Offset)       { m_Offset = Offset; SanitizeOffset(); }

  /*! \param Size is the size of the boundary.
   * \param Off is the offset of the boundary.
   * \return Returns true if this address is contained in [Off:Off+Size].
   */
  bool IsBetween(u64 Size, TOffset Off) const;
  bool IsBetween(u64 Size, Address const& Addr) const;

  //! This method returns true if base and offset are egal.
  bool operator==(Address const& rAddr) const;

  //! This method returns true if base and offset are different.
  bool operator!=(Address const& rAddr) const;

  //! This method returns an address where its offset is the current offset plus Off.
  Address operator+(TOffset Off) const;
  Address operator+(Address const& Addr) const;

  //! This method add a offset to the current address offset.
  Address operator+=(TOffset Off);

  //! This method returns true if both base and offset are inferior to rAddr.
  bool operator<(Address const& rAddr) const;

  //! This method returns true if both base and offset are inferior or equal to rAddr.
  bool operator<=(Address const& rAddr) const;

  //! This method returns true if both base and offset are superior to rAddr.
  bool operator>(Address const& rAddr) const;

  //! This method returns true if both base and offset are superior or equal to rAddr.
  bool operator>=(Address const& rAddr) const;

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

namespace std
{
  template<> class hash<medusa::Address>
  {
  public:
    size_t operator()(medusa::Address const& rAddr) const
    {
      auto Hash_u16 = std::hash<medusa::u16>();
      auto Hash_u64 = std::hash<medusa::u64>();
      return Hash_u16(rAddr.GetBase()) ^ Hash_u64(rAddr.GetOffset());
    }
  };
}

Medusa_EXPORT std::ostream& operator<<(std::ostream& rOstrm, medusa::Address const& rAddr);
Medusa_EXPORT std::istream& operator>>(std::istream& rIstrm, medusa::Address& rAddr);

#endif // MEDUSA_ADDRESS_HPP