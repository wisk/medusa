#include "medusa/function.hpp"

MEDUSA_NAMESPACE_BEGIN

bool Function::Contains(Address const& rAddress) const
{
  return m_Cfg.Contains(rAddress);
}

MEDUSA_NAMESPACE_END