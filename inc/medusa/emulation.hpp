#ifndef MEDUSA_EMULATION_HPP
#define MEDUSA_EMULATION_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/expression.hpp"
#include "medusa/context.hpp"
#include "medusa/document.hpp"
#include "medusa/instruction.hpp"
#include "medusa/architecture.hpp"

#include <unordered_map>
#include <functional>

MEDUSA_NAMESPACE_BEGIN

class MEDUSA_EXPORT Emulator
{
public:
  typedef std::shared_ptr<Emulator> SPType;

  Emulator(void);
  virtual ~Emulator(void);

  virtual void Initialize(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt);

  virtual std::string GetName(void) const = 0;

  virtual bool ReadRegister(u32 Reg, void* pVal, u32 BitSize) const;
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

  enum ReturnType
  {
    Error,
    Break,
    Continue,
    Stop,
  };

  virtual ReturnType Execute(Expression::SPType spExpr);
  virtual ReturnType Execute(Expression::VSPType const& rExprs) = 0;
  virtual ReturnType Execute(Address const& rAddress);

  enum HookType
  {
    HookUnknown   = 0x0,
    HookOnRead    = 0x1,
    HookOnWrite   = 0x2,
    HookOnExecute = 0x4,
  };

  typedef std::function<ReturnType(CpuContext*, MemoryContext*, Address const&)> HookCallback;

  virtual bool AddHookOnInstruction(HookCallback InsnCb);
  virtual bool AddHook(Address const& rAddress, Emulator::HookType Type, HookCallback Callback);
  virtual bool AddHook(Document const& rDoc, std::string const& rLabelName, Emulator::HookType Type, HookCallback Callback);
  virtual bool RemoveHook(Address const& rAddress);
  virtual ReturnType CallInstructionHook(void);
  virtual ReturnType CallHookOnExecutionIfNeeded(Address const& rAddress) const;

  virtual bool InvalidateCache(void);

protected:
  typedef std::function<bool (Address const&, Instruction&, Architecture&, u8)> DisasmCbType;
  bool _Disassemble(Address const& rAddress, DisasmCbType Cb);

  // Semantic cache
  bool _IsSemanticCached(Address const& rAddress) const;
  bool _CacheSemantic(Address const& rAddress, Expression::VSPType& rExprs);
  bool _InvalidSemantic(Address const& rAddress);

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

  typedef std::unordered_map<Address, Expression::VSPType> SemanticCacheType;
  SemanticCacheType m_SemCache;
};

typedef Emulator* (*TGetEmulator)(void);

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_EMULATION_HPP
