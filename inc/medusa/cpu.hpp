#ifndef __MEDUSA_CPU_HPP__
#define __MEDUSA_CPU_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

#include <string>

MEDUSA_NAMESPACE_BEGIN

class CpuInformation
{
public:
  enum Type
  {
    StackPointerRegister,
    StackFrameRegister,
    ProgramPointerRegister,
    FlagRegister,
    InvalidRegister
  };

  virtual char const* ConvertIdentifierToName(u32 Id) const = 0;
  virtual u32         GetRegisterByType(Type RegType) const = 0;
  virtual u32         GetSizeOfRegisterInBit(u32 Id)  const = 0;
};

class CpuContext
{
public:
  virtual void ReadRegister (u32 Register, void*       pValue, u32 Size) const = 0;
  virtual void WriteRegister(u32 Register, void const* pValue, u32 Size)       = 0;

  virtual std::string ToString(void) const = 0;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_CPU_HPP__
