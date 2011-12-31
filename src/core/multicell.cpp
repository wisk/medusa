#include "medusa/multicell.hpp"

MEDUSA_NAMESPACE_BEGIN

void MultiCell::Load(SerializeEntity::SPtr spSrlzEtt)
{
}

SerializeEntity::SPtr MultiCell::Save(void)
{
  SerializeEntity::SPtr spMultiCell(new SerializeEntity("mcell"));

  return spMultiCell;
}

MEDUSA_NAMESPACE_END