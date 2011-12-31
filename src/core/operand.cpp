#include "medusa/operand.hpp"

MEDUSA_NAMESPACE_BEGIN

void Operand::Load(SerializeEntity::SPtr spSrlzEtt)
{
  spSrlzEtt->GetField("type", m_Type);
  spSrlzEtt->GetField("name", m_Name);
  spSrlzEtt->GetField("reg", m_Reg);
  spSrlzEtt->GetField("sec_reg", m_SecReg);
  spSrlzEtt->GetField("seg", m_Seg);
  spSrlzEtt->GetField("value", m_Value);
  spSrlzEtt->GetField("seg_value", m_SegValue);
}

SerializeEntity::SPtr Operand::Save(void)
{
  SerializeEntity::SPtr Operand(new SerializeEntity("operand"));
  Operand->AddField("type", m_Type);
  Operand->AddField("name", m_Name);
  Operand->AddField("reg", m_Reg);
  Operand->AddField("sec_reg", m_SecReg);
  Operand->AddField("seg", m_Seg);
  Operand->AddField("value", m_Value);
  Operand->AddField("seg_value", m_SegValue);
  return Operand;
}

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