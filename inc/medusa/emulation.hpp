#ifndef __MEDUSA_EMULATION_HPP__
#define __MEDUSA_EMULATION_HPP__

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
  typedef boost::shared_ptr<Emulator> SharedPtr;

  virtual ~Emulator(void);

  virtual std::string GetName(void) const = 0;

  virtual bool ReadRegister (u32 Register,            void* pValue,       u32 ValueSize) const;
  virtual bool WriteRegister(u32 Register,            void const* pValue, u32 ValueSize);
  virtual bool ReadMemory   (Address const& rAddress, void* pValue,       u32 ValueSize) const;
  virtual bool WriteMemory  (Address const& rAddress, void const* pValue, u32 ValueSize);

  virtual bool Execute(Address const& rAddress, Expression::SPtr spExpr) = 0;
  virtual bool Execute(Address const& rAddress, Expression::List const& rExprList) = 0;

  enum HookType
  {
    HookUnknown   = 0x0,
    HookOnRead    = 0x1,
    HookOnWrite   = 0x2,
    HookOnExecute = 0x4,
  };

  typedef std::function<void(CpuContext*, MemoryContext*)> HookCallback;

  virtual bool AddHook(Address const& rAddress, u32 Type, HookCallback Callback);
  virtual bool AddHook(Document const& rDoc, std::string const& rLabelName, u32 Type, HookCallback Callback);
  virtual bool RemoveHook(Address const& rAddress);

protected:
  Emulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt);

  struct HookInformation
  {
    HookInformation(u32 Type = HookUnknown, HookCallback Callback = nullptr) : m_Type(Type), m_Callback(Callback) {}
    u32          m_Type;
    HookCallback m_Callback;
  };

  bool TestHook(Address const& rAddress, u32 Type) const;

  CpuInformation const* m_pCpuInfo;
  CpuContext*           m_pCpuCtxt;
  MemoryContext*        m_pMemCtxt;
  typedef std::unordered_map<Address, HookInformation> HookAddressHashMap;
  HookAddressHashMap m_Hooks;
};

typedef Emulator* (*TGetEmulator)(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt);

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_EMULATION_HPP__