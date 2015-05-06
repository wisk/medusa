#ifndef MEDUSA_EMULATION_HPP
#define MEDUSA_EMULATION_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/expression.hpp"
#include "medusa/context.hpp"
#include "medusa/document.hpp"

#include <unordered_map>
#include <functional>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Emulator
{
public:
  typedef std::shared_ptr<Emulator> SPType;

  virtual ~Emulator(void);

  virtual std::string GetName(void) const = 0;

  virtual bool ReadRegister (u32 Reg, void* pVal, u32 BitSize) const;
  template<typename _RegTy>
  bool ReadRegister(u32 Reg, _RegTy& rRegVal) const
  {
    return ReadRegister(Reg, &rRegVal, sizeof(rRegVal) * 8);
  }

  virtual bool WriteRegister(u32 Reg, void const* pVal, u32 BitSize);
  template<typename _RegTy>
  bool WriteRegister(u32 Reg, _RegTy RegVal)
  {
    return WriteRegister(Reg, &RegVal, sizeof(RegVal) * 8);
  }

  virtual bool ReadMemory(Address const& rAddr, void* pVal, u32 Size) const;
  template<typename _MemTy>
  bool ReadMemory(Address const& rAddr, _MemTy& rMemVal)
  {
    return ReadMemory(rAddr, &rMemVal, sizeof(rMemVal));
  }

  virtual bool WriteMemory(Address const& rAddr, void const* pVal, u32 Size);
  template<typename _MemTy>
  bool WriteMemory(Address const& rAddr, _MemTy const& rMemVal)
  {
    return WriteMemory(rAddr, &rMemVal, sizeof(rMemVal));
  }

  virtual bool Execute(Address const& rAddress, Expression::SPType spExpr);
  virtual bool Execute(Address const& rAddress, Expression::LSPType const& rExprList) = 0;

  enum HookType
  {
    HookUnknown   = 0x0,
    HookOnRead    = 0x1,
    HookOnWrite   = 0x2,
    HookOnExecute = 0x4,
  };

  typedef std::function<void(CpuContext*, MemoryContext*, Address const&)> HookCallback;

  virtual bool AddHookOnInstruction(HookCallback InsnCb);
  virtual void CallInstructionHook(void);
  virtual bool AddHook(Address const& rAddress, u32 Type, HookCallback Callback);
  virtual bool AddHook(Document const& rDoc, std::string const& rLabelName, u32 Type, HookCallback Callback);
  virtual bool RemoveHook(Address const& rAddress);
  virtual bool TestHook(Address const& rAddress, u32 Type) const;

protected:
  Emulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt);

  struct HookInformation
  {
    HookInformation(u32 Type = HookUnknown, HookCallback Callback = nullptr) : m_Type(Type), m_Callback(Callback) {}
    u32          m_Type;
    HookCallback m_Callback;
  };

  CpuInformation const* m_pCpuInfo;
  CpuContext*           m_pCpuCtxt;
  MemoryContext*        m_pMemCtxt;
  typedef std::unordered_map<Address, HookInformation> HookAddressHashMap;
  HookAddressHashMap    m_Hooks;
  HookCallback          m_InsnCb;
};

typedef Emulator* (*TGetEmulator)(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt);

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EMULATION_HPP
