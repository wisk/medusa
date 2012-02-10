#include "medusa/label.hpp"
#include <algorithm>

MEDUSA_NAMESPACE_BEGIN

std::string Label::GetLabel(void) const
{
  if (m_Name.empty())
    return "";

  std::string Result;

  Result.resize(m_Name.size(), '_');
  std::transform(m_Name.begin(), m_Name.end(), Result.begin(), ConvertToLabel);
  return m_Prefix + Result;
}

char Label::ConvertToLabel(char c)
{
  if (!isalnum(c))
    return '_';
  return c;
}

MEDUSA_NAMESPACE_END
