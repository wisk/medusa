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
  auto ArchTag = m_rDoc.GetArchitectureTag(rAddr);
  auto spArch  = ModuleManager::Instance().GetArchitecture(ArchTag);
  if (spArch == nullptr)
    return false;



  Address LastAddr, CurAddr;
  Context SymCtxt;
  std::unordered_map<Address, bool> VstAddrs;
  std::stack<Address> StkAddrs;

  StkAddrs.push(rAddr);
  while (!StkAddrs.empty())
  {
    CurAddr = StkAddrs.top();
    StkAddrs.pop();

    while (true)
    {
      auto spInsn = std::dynamic_pointer_cast<Instruction>(m_rDoc.GetCell(CurAddr));
      if (spInsn == nullptr)
        break;

      //auto pInsnSem = spInsn->GetSemantic();
      //for (auto const pExpr : pInsnSem)
      //{

      //}

      Address::List NextAddrs;

      for (u8 i = 0; i < OPERAND_NO; ++i)
      {
        Address OprdAddr;
        if (spInsn->GetOperandReference(m_rDoc, i, CurAddr, OprdAddr))
          NextAddrs.push_back(OprdAddr);
      }

      if (spInsn->GetSubType() & Instruction::NoneType || spInsn->GetSubType() & Instruction::ConditionalType)
        NextAddrs.push_back(CurAddr + spInsn->GetLength());

      if (!Cb(SymCtxt, CurAddr, NextAddrs))
        break;

      if (NextAddrs.empty())
        break;

      while (NextAddrs.size() != 1)
      {
        StkAddrs.push(NextAddrs.front());
        NextAddrs.pop_front();
      }

      LastAddr = CurAddr;
      CurAddr  = NextAddrs.front();
    }
  };

  return true;
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