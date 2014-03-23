#ifndef __MEDUSA_EXECUTION_HPP__
#define __MEDUSA_EXECUTION_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/medusa.hpp"
#include "medusa/architecture.hpp"
#include "medusa/os.hpp"
#include "medusa/context.hpp"
#include "medusa/emulation.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Execution
{
public:
  Execution(Medusa* pCore, Architecture::SharedPtr spArch, OperatingSystem::SharedPtr spOs);
  ~Execution(void);

  bool Initialize(u8 Mode, u64 StackLinearAddress, u32 StackSize);
  bool SetEmulator(std::string const& rEmulatorName);

  void Execute(Address const& rAddr);

private:
  Medusa*                    m_pCore;
  Architecture::SharedPtr    m_spArch;
  OperatingSystem::SharedPtr m_spOs;
  CpuContext*                m_pCpuCtxt;
  MemoryContext*             m_pMemCtxt;
  CpuInformation const*      m_pCpuInfo;
  Emulator::SharedPtr        m_spEmul;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_EXECUTION_HPP__