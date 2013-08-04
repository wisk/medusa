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
  /*
  In VC debug, isalnum can assert...
  src: http://msdn.microsoft.com/en-us/library/k84c0490(v=vs.71).aspx
  When used with a debug CRT library, isalnum will display a CRT assert if passed a parameter that isn't EOF or in the range of 0 through 0xFF.
  When used with a debug CRT library, isalnum will use the parameter as an index into an array, with undefined results if the parameter isn't EOF or in the range of 0 through 0xFF.
  */
  int n = c;
  if (n < 0 || n > 0xff)
    return '_';
  if (!isalnum(c) && c != '!' && c != '.')
    return '_';
  return c;
}

MEDUSA_NAMESPACE_END
