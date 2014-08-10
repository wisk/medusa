#include "medusa/symbolic.hpp"
#include "medusa/module.hpp"

MEDUSA_NAMESPACE_USE;

Symbolic::Symbolic(Document& rDoc)
  : m_rDoc(rDoc)
{
}

Symbolic::~Symbolic(void)
{
}

bool Symbolic::TaintRegister(u32 RegId, Address const& rAddr, Symbolic::Callback Cb)
{
  return false;
}

bool Symbolic::TaintRegister(CpuInformation::Type RegisterType, Address const& rAddress, Symbolic::Callback Cb)
{
  auto ArchTag  = m_rDoc.GetArchitectureTag(rAddress);
  auto ArchMode = m_rDoc.GetMode(rAddress);
  auto spArch   = ModuleManager::Instance().GetArchitecture(ArchTag);
  if (spArch == nullptr)
    return false;

  auto const pCpuInfo = spArch->GetCpuInformation();
  if (pCpuInfo == nullptr)
    return false;

  u32 RegId = pCpuInfo->GetRegisterByType(RegisterType, ArchMode);
  if (RegId == 0)
    return false;

  return TaintRegister(RegId, rAddress, Cb);
}