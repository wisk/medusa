#include "medusa/value.hpp"

MEDUSA_NAMESPACE_BEGIN

template<typename T> void Value<T>::Load(SerializeEntity::SPtr spSrlzEtt)
{
  Cell::Load(spSrlzEtt);
  spSrlzEtt->GetField("value_type", m_ValueType );
  spSrlzEtt->GetField("data",       m_Data      );
}

template<typename T> SerializeEntity::SPtr Value<T>::Save(void)
{
  SerializeEntity::SPtr Value(new SerializeEntity("value"));
  Value->AddField("type",       m_Type      );
  Value->AddField("comment",    m_Comment   );
  Value->AddField("value_type", m_ValueType );
  Value->AddField("data",       m_Data      );
  Value->AddField("data_size",  static_cast<u8>(sizeof(T)));
  return Value;
}

template class Value<u8>;
template class Value<u16>;
template class Value<u32>;
template class Value<u64>;

MEDUSA_NAMESPACE_END
