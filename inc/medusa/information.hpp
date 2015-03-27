#ifndef MEDUSA_INFORMATION_HPP
#define MEDUSA_INFORMATION_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT CpuInformation
{
public:
  enum Type
  {
    StackPointerRegister,
    StackFrameRegister,
    ProgramBaseRegister,
    ProgramPointerRegister,
    AccumulatorRegister,
    CounterRegister,
    InvalidRegister
  };

  virtual char const* ConvertIdentifierToName(u32 Id)                   const = 0;
  virtual u32         ConvertNameToIdentifier(std::string const& rName) const = 0;
  virtual u32         GetRegisterByType(Type RegType, u8 Mode)          const = 0;
  virtual u32         GetSizeOfRegisterInBit(u32 Id)                    const = 0;
  virtual bool        IsRegisterAliased(u32 Id0, u32 Id1)               const = 0;
  virtual bool        NormalizeRegister(u32 Id, u8 Mode, u32& rExtId, u64& rMask) const
    // If no conversion is needed, it shall return the parameter
  { return false; }
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_INFORMATION_HPP