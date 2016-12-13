#include "medusa/compilation.hpp"

MEDUSA_NAMESPACE_BEGIN

std::string Compiler::GetFormat(void) const
{
  return m_Format;
}

std::string Compiler::GetEntryPoint(void) const
{
  return m_EntryPoint;
}

void Compiler::SetFormat(std::string const& rFormat)
{
  m_Format = rFormat;
}

void Compiler::SetEntryPoint(std::string const& rLabelName)
{
  m_EntryPoint = rLabelName;
}

bool Compiler::AddCode(std::string const& rCodeName, Expression::VSPType const& rCode)
{
  if (m_CodeMap.find(rCodeName) != std::end(m_CodeMap))
    return false;
  m_CodeMap[rCodeName] = rCode;
  return true;
}

bool Compiler::AddData(std::string const& rDataName, std::vector<u8> const& rData)
{
  if (m_DataMap.find(rDataName) != std::end(m_DataMap))
    return false;
  m_DataMap[rDataName] = rData;
  return true;
}

bool Compiler::Compile(Path const& rOutputFile)
{
  return false;
}

MEDUSA_NAMESPACE_END