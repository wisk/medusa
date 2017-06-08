#include "medusa/function.hpp"
#include "medusa/util.hpp"

MEDUSA_NAMESPACE_BEGIN

Function::Function(u16 Size, u16 InsnCnt)
  : MultiCell(MultiCell::FunctionType, Size)
  , m_InsnCnt(InsnCnt)
{}

MEDUSA_NAMESPACE_END