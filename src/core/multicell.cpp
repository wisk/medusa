#include "medusa/multicell.hpp"

MEDUSA_NAMESPACE_BEGIN

std::string MultiCell::Dump(void) const
{
  char TypeChr = '?';
  switch (m_Type)
  {
  case FunctionType: TypeChr = 'f'; break;
  case StructType:   TypeChr = 's'; break;
  case ArrayType:    TypeChr = 'a'; break;
  default:                          break;
  }
  std::ostringstream oss;
  oss << std::hex << std::showbase;
  oss << "mc(" << TypeChr << ", " << m_Size << ")";
  return oss.str();
}

MEDUSA_NAMESPACE_END