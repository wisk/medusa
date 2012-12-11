#ifndef __MEDUSA_CPU_HPP__
#define __MEDUSA_CPU_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"

MEDUSA_NAMESPACE_BEGIN

class CpuInformation
{
public:
  enum Type
  {
    StackPointerRegister,
    ProgramPointerRegister,
    FlagRegister,
    InvalidRegister
  };

  virtual char const* ConvertIdentifierToName(u32 Id) const = 0;
  virtual u32         GetRegisterByType(Type RegType) const = 0;
  virtual u32         GetSizeOfRegisterInBit(u32 Id) const = 0;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_CPU_HPP__