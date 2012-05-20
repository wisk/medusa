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
  BasicBlockVertexProperties(Address const& rAddr = Address(), size_t NumOfInsn = 0);

  Address const& GetAddress(void)             const { return m_Address;             }
  size_t         GetNumberOfInstruction(void) const { return m_NumberOfInstruction; }

  Address m_Address;
  size_t m_NumberOfInstruction;
};

class Medusa_EXPORT BasicBlockEdgeProperties
{
public:
  enum Type
  {
    Unknown,
    Unconditional,
    True,
    False
  };

  BasicBlockEdgeProperties(Type Type = Unknown) : m_Type(Type) {}

private:
  Type m_Type;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_BASIC_BLOCK_HPP__