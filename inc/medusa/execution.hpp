#ifndef MEDUSA_EXECUTION_HPP
#define MEDUSA_EXECUTION_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/document.hpp"
#include "medusa/architecture.hpp"
#include "medusa/os.hpp"
#include "medusa/context.hpp"
#include "medusa/emulation.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Execution
{
public:
  Execution(Document& rDoc, Architecture::SPType spArch, OperatingSystem::SPType spOs);
  ~Execution(void);

  bool Initialize(u8 Mode, u64 StackLinearAddress, u32 StackSize);
  bool SetEmulator(std::string const& rEmulatorName);

  void Execute(Address const& rAddr);

private:
  Document&               m_rDoc;
  Architecture::SPType    m_spArch;
  OperatingSystem::SPType m_spOs;
  CpuContext*             m_pCpuCtxt;
  MemoryContext*          m_pMemCtxt;
  CpuInformation const*   m_pCpuInfo;
  Emulator::SPType        m_spEmul;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXECUTION_HPP