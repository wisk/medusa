#include "medusa/function.hpp"
#include "medusa/util.hpp"

MEDUSA_NAMESPACE_BEGIN

Function::Function(MultiCell::Id FunctionId, u16 Size, u16 InsnCnt)
  : MultiCell(FunctionId, MultiCell::FunctionType, Size)
  , m_InsnCnt(InsnCnt)
  , m_ParamCnt()
{}

Function::Function(std::string Name, u16 Size, u16 InsnCnt)
  : MultiCell(Sha1(Name), MultiCell::FunctionType, Size)
  , m_InsnCnt(InsnCnt)
  , m_ParamCnt()
{
}

MEDUSA_NAMESPACE_END