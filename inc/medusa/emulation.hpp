#ifndef __MEDUSA_EMULATION_HPP__
#define __MEDUSA_EMULATION_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/expression.hpp"
#include "medusa/cpu.hpp"
#include "medusa/database.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Emulator
{
public:
           Emulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt);
  virtual ~Emulator(void);

  virtual std::string GetName(void) const = 0;

  virtual void ReadRegister (u32 Register,            void* pValue,       u32 ValueSize) const;
  virtual void WriteRegister(u32 Register,            void const* pValue, u32 ValueSize);
  virtual void ReadMemory   (Address const& rAddress, void* pValue,       u32 ValueSize) const;
  virtual void WriteMemory  (Address const& rAddress, void const* pValue, u32 ValueSize);

  virtual void Execute(Expression const& rExpr) = 0;
  virtual void Execute(Expression::List const& rExprList) = 0;

protected:
  CpuInformation const* m_pCpuInfo;
  CpuContext*           m_pCpuCtxt;
  MemoryContext*        m_pMemCtxt;
};

typedef Emulator* (*TGetEmulator)(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt);

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_EMULATION_HPP__