#ifndef _EMUL_LLVM_
#define _EMUL_LLVM_

#include <medusa/emulation.hpp>

MEDUSA_NAMESPACE_USE

class LlvmEmulator : public medusa::Emulator
{
public:
           LlvmEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt);
  virtual ~LlvmEmulator(void);

  virtual void ReadRegister (u32 Register,            void* pValue,       u32 ValueSize) const;
  virtual void WriteRegister(u32 Register,            void const* pValue, u32 ValueSize);
  virtual void ReadMemory   (Address const& rAddress, void* pValue,       u32 ValueSize) const;
  virtual void WriteMemory  (Address const& rAddress, void const* pValue, u32 ValueSize);

  virtual bool AllocateMemory(Address const& rAddress, u32 Size, void **ppRawMemory);
  virtual bool FreeMemory    (Address const& rAddress);

  virtual void Execute(Expression const& rExpr);
  virtual void Execute(Expression::List const& rExprList);
protected:
};

#endif // !_EMUL_LLVM_