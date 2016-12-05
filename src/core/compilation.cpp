#include "medusa/compilation.hpp"

MEDUSA_NAMESPACE_BEGIN

void Compiler::SetEntryPoint(std::string const& rLabelName)
{
  m_EntryPoint = rLabelName;
}

MEDUSA_NAMESPACE_END