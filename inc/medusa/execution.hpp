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
  Execution(Document& rDoc);
  ~Execution(void);

  bool Initialize(std::vector<std::string> const& rArgs, std::vector<std::string> const& rEnv, std::string const& rCurWrkDir);
  bool SetEmulator(std::string const& rEmulatorName);

  void Execute(Address const& rAddr);

  bool HookInstruction(Emulator::HookCallback HkCb);
  bool HookFunction(std::string const& rFuncName, Emulator::HookCallback HkCb);
  bool Hook(Address const& rAddress, u32 Type, Emulator::HookCallback Callback);
  bool Hook(std::string const& rLabelName, u32 Type, Emulator::HookCallback Callback);
  std::string GetHookName(void) const;
  Address GetHookAddress(std::string const& rHkFuncName) const;

  MemoryContext* GetMemoryContext(void) { return m_pMemCtxt; }
  // LATER: implement thread instead of cpu context
  CpuContext* GetCpuContext(void) { return m_pCpuCtxt; }

private:
  Execution(Execution const&);
  Execution const& operator=(Execution const&);

private:
  Document&               m_rDoc;
  Architecture::SPType    m_spArch;
  OperatingSystem::SPType m_spOs;
  CpuContext*             m_pCpuCtxt;
  MemoryContext*          m_pMemCtxt;
  CpuInformation const*   m_pCpuInfo;
  Emulator::SPType        m_spEmul;

  mutable std::mutex         m_HookMutex;
  std::map<u64, std::string> m_HookName;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EXECUTION_HPP