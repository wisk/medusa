#include "medusa/operand.hpp"

MEDUSA_NAMESPACE_BEGIN

u8 Operand::GetLength(void) const
{
    switch (m_Type & DS_MASK)
    {
    case DS_8BIT:  return sizeof(u8);
    case DS_16BIT: return sizeof(u16);
    case DS_32BIT: return sizeof(u32);
    case DS_64BIT: return sizeof(u64);
    default:       return 0;
    }
}

MEDUSA_NAMESPACE_END