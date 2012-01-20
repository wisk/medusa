#include "medusa/value.hpp"

MEDUSA_NAMESPACE_BEGIN

void Value::Load(SerializeEntity::SPtr spSrlzEtt)
{
  Cell::Load(spSrlzEtt);
  spSrlzEtt->GetField("value_type", m_ValueType);
}

SerializeEntity::SPtr Value::Save(void)
{
  SerializeEntity::SPtr Value(new SerializeEntity("value"));
  Value->AddField("type",       m_Type      );
  Value->AddField("comment",    m_Comment   );
  Value->AddField("value_type", m_ValueType );
  return Value;
}

MEDUSA_NAMESPACE_END
