#ifndef ARM_HELPER_HPP
#define ARM_HELPER_HPP

#include <medusa/expression.hpp>
#include "arm_def.hpp"

namespace arm
{
  medusa::u32 RegisterFromValue(char const* pRegTypeName, medusa::u32 RegVal);
  std::vector<medusa::u32> RegisterList(medusa::u32 RegListBitSize, medusa::u32 RegList);

  medusa::u32 ConditionFromValue(medusa::u32 CondVal);
  medusa::u32 ConditionFromName(char const* pCondName);

  medusa::u32 ExpandImm(medusa::u32 Imm);

  medusa::Expression::SPType DecodeRegWithSources(medusa::u32 Rm, medusa::u32 Type, medusa::u32 Rs);
  medusa::Expression::SPType DecodeImmShiftWithSource(medusa::u32 Rm, medusa::u32 Type, medusa::u32 Imm);
}

#endif // !ARM_HELPER_HPP