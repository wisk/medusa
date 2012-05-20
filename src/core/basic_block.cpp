#include "medusa/basic_block.hpp"

MEDUSA_NAMESPACE_USE

BasicBlockVertexProperties::BasicBlockVertexProperties(Address const& rAddr, size_t NumOfInsn)
  : m_Address(rAddr)
  , m_NumberOfInstruction(NumOfInsn)
{
}