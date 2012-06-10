#ifndef __MEDUSA_BASIC_BLOCK_HPP__
#define __MEDUSA_BASIC_BLOCK_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT BasicBlockVertexProperties
{
public:
  BasicBlockVertexProperties(void) {}
  BasicBlockVertexProperties(Address::List const& rAddresses) : m_Addresses(rAddresses)
  {
    m_Addresses.sort();
  }

  Address::List const& GetAddresses(void)             const { return m_Addresses;        }
  Address              GetFirstAddress(void) const;
  size_t               GetNumberOfInstruction(void) const { return m_Addresses.size(); }
  bool                 Contains(Address const& rAddr) const;
  bool                 Split(Address const& rAddr, Address::List& rAddresses);

  Address::List m_Addresses;
};

class Medusa_EXPORT BasicBlockEdgeProperties
{
public:
  enum Type
  {
    Unknown,
    Unconditional,
    True,
    False,
    Next
  };

  BasicBlockEdgeProperties(Type Type = Unknown) : m_Type(Type) {}

private:
  Type m_Type;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_BASIC_BLOCK_HPP__