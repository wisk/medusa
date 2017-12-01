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

class MEDUSA_EXPORT Address
{
public:

  typedef enum
  {
    UnknownType,      //! Used only in case of error or default
    DefaultType,      //! Defined by the loader
    PhysicalType,     //! Physical type or offset
    ArchitectureType, //! Relies on architecture to convert address (e.g. segmented address, mirror address, ...)
    LinearType,       //! Used for protected mode
    RelativeType,     //! Relative to a image base (RVA)
    LogicalType,      //! Base:Offset (used for x86, video games consoles with MBC, ...)
  } Type;

  typedef std::vector<Address> Vector;

  /*!
   * \param Type defines the kind of address.
   * \param Base is the base of this address.
   * \param Offset is the offset of this address.
   * \param BaseSize defines the base size in bits.
   * \param OffsetSize defines the offset size in bits.
   */
  Address(Type Type, BaseType Base, OffsetType Offset, u8 BaseSize, u8 OffsetSize)
    : m_Type(Type)
    , m_Base(Base)
    , m_Offset(Offset)
    , m_BaseSize(BaseSize)
    , m_OffsetSize(OffsetSize)
  { SanitizeOffset(); }

  Address(BaseType Base, OffsetType Offset, u8 BaseSize, u8 OffsetSize)
    : m_Type(LogicalType)
    , m_Base(Base)
    , m_Offset(Offset)
    , m_BaseSize(BaseSize)
    , m_OffsetSize(OffsetSize)
  { SanitizeOffset(); }

  Address(Type Type, BaseType Base, OffsetType Offset)
    : m_Type(Type)
    , m_Base(Base)
    , m_Offset(Offset)
    , m_BaseSize(16)
    , m_OffsetSize(64)
  {}

  Address(BaseType Base, OffsetType Offset)
    : m_Type(LogicalType)
    , m_Base(Base)
    , m_Offset(Offset)
    , m_BaseSize(16)
    , m_OffsetSize(64)
  {}

  Address(Type Type, OffsetType Offset)
    : m_Type(Type)
    , m_Base(0x0)
    , m_Offset(Offset)
    , m_BaseSize(16)
    , m_OffsetSize(64)
  {}

  Address(OffsetType Offset)
    : m_Type(DefaultType)
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

  //! This method converts the current address to a string.
  std::string ToString(void) const;

  //! This method allows to mask an offset accordingly the current offset size.
  OffsetType SanitizeOffset(OffsetType Offset) const;
  void SanitizeOffset(OffsetType& rOffset);

  Type       GetAddressingType(void) const { return m_Type;       }
  BaseType   GetBase(void)           const { return m_Base;       }
  OffsetType GetOffset(void)         const { return m_Offset;     }
  u8         GetBaseSize(void)       const { return m_BaseSize;   }
  u8         GetOffsetSize(void)     const { return m_OffsetSize; }

  // Set
  void      SetAddressingType(Type AddressingType) { m_Type       = AddressingType;             }
  void      SetBase(BaseType Base)                 { m_Base       = Base;                       }
  void      SetOffset(OffsetType Offset)           { m_Offset     = Offset; SanitizeOffset();   }
  void      SetBaseSize(u8 BaseSize)               { m_BaseSize   = BaseSize;                   }
  void      SetOffsetSize(u8 OffsetSize)           { m_OffsetSize = OffsetSize;                 }

  /*! \param Size is the size of the boundary.
   * \param Off is the offset of the boundary.
   * \return Returns true if this address is contained in [Off:Off+Size].
   */
  bool IsBetween(u64 Size, OffsetType Off) const;
  bool IsBetween(u64 Size, Address const& Addr) const;

  //! This method returns true if base and offset are egal.
  bool operator==(Address const& rAddr) const;

  //! This method returns true if base and offset are different.
  bool operator!=(Address const& rAddr) const;

  //! This method returns an address where its offset is the current offset plus Off.
  Address operator+(OffsetType Off) const;
  Address operator+(Address const& Addr) const;

  //! This method add a offset to the current address offset.
  Address operator+=(OffsetType Off);

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
  Type       m_Type;
  BaseType   m_Base;
  OffsetType m_Offset;
  u8         m_BaseSize;
  u8         m_OffsetSize;
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

  static inline std::ostream& operator<<(std::ostream& rOstream, medusa::Address const& rAddress)
  {
    rOstream << rAddress.ToString();
    return rOstream;
  }

  static inline std::ostream& operator<<(std::ostream& rOstream, medusa::Address::Vector const& rAddresses)
  {
    auto Size = rAddresses.size();

    if (Size == 0)
      return rOstream;

    rOstream << rAddresses[0].ToString();
    for (decltype(Size) i = 1; i < Size; ++i)
      rOstream << " " << rAddresses[i].ToString();

    return rOstream;
  }

  static inline std::istream& operator>>(std::istream& rIstream, medusa::Address& rAddress)
  {
    std::string AddrStr;
    rIstream >> AddrStr;
    rAddress = medusa::Address(AddrStr);
    return rIstream;
  }

  static inline std::istream& operator>>(std::istream& rIstream, medusa::Address::Vector& rAddresses)
  {
    std::string AddrStr;
    if (!(rIstream >> AddrStr))
      return rIstream;
    rAddresses.push_back(medusa::Address(AddrStr));

    while (!rIstream.eof())
    {
      rIstream.ignore(1, ' ');
      if (!rIstream)
        break;
      if (!(rIstream >> AddrStr))
        break;
      rAddresses.push_back(medusa::Address(AddrStr));
    }
    return rIstream;
  }
}

MEDUSA_EXPORT std::ostream& operator<<(std::ostream& rOstream, medusa::Address const& rAddress);
MEDUSA_EXPORT std::ostream& operator<<(std::ostream& rOstream, medusa::Address::Vector const& rAddresses);
MEDUSA_EXPORT std::istream& operator>>(std::istream& rIstream, medusa::Address& rAddress);
MEDUSA_EXPORT std::istream& operator>>(std::istream& rIstream, medusa::Address::Vector& rAddresses);

#endif // MEDUSA_ADDRESS_HPP
