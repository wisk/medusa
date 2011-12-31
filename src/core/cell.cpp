#include "medusa/cell.hpp"

MEDUSA_NAMESPACE_BEGIN

void Cell::Load(SerializeEntity::SPtr spSrlzEtt)
{
  spSrlzEtt->GetField("type",     m_Type    );
  spSrlzEtt->GetField("comment",  m_Comment );
}

SerializeEntity::SPtr Cell::Save(void)
{
  SerializeEntity::SPtr Cell(new SerializeEntity("cell"));
  Cell->AddField("type",    m_Type    );
  Cell->AddField("comment", m_Comment );
  return Cell;
}

MEDUSA_NAMESPACE_END
