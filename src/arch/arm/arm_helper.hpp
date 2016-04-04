#ifndef ARM_HELPER_HPP
#define ARM_HELPER_HPP

#include <tuple>
#include <medusa/expression.hpp>
#include "arm_def.hpp"

namespace arm
{
  enum SRType
  {
    SRType_LSL, SRType_LSR, SRType_ASR, SRType_ROR, SRType_RRX, SRType_Unknown,
  };

  std::tuple<medusa::u32, bool> LSL_C(medusa::u32 Value, medusa::s32 Amount);
  std::tuple<medusa::u32, bool> LSR_C(medusa::u32 Value, medusa::s32 Amount);
  std::tuple<medusa::u32, bool> ASR_C(medusa::s32 Value, medusa::s32 Amount);
  std::tuple<medusa::u32, bool> ROR_C(medusa::u32 Value, medusa::s32 Amount);
  std::tuple<medusa::u32, bool> RRX_C(medusa::u32 Value, bool CarryIn);

  std::tuple<SRType, medusa::u32> DecodeImmShift(medusa::u32 Type, medusa::u32 Imm);
  SRType DecodeRegShift(medusa::u32 Type);

  medusa::u32 Shift(medusa::u32 Value, medusa::u32 Type, medusa::u32 Amount, bool CarryIn);
  std::tuple<medusa::u32, bool> Shift_C(medusa::u32 Value, medusa::u32 Type, medusa::u32 Amount, bool CarryIn);
  std::tuple<medusa::u32, bool> ArmExpandImm_C(medusa::u32 Imm, bool CarryIn);

  medusa::u32 RegisterFromValue(char const* pRegTypeName, medusa::u32 RegVal);
  std::vector<medusa::u32> RegisterList(medusa::u32, medusa::u32 RegList);

  medusa::u32 ConditionFromValue(medusa::u32 CondVal);
  medusa::u32 ConditionFromName(char const* pCondName);
  medusa::u32 ArmExpandImm(medusa::u32 Imm);
  medusa::Expression::SPType DecodeRegShiftWithSources(medusa::CpuInformation const* pCpuInfo, medusa::u32 Rm, medusa::u32 Type, medusa::u32 Rs);
  medusa::Expression::SPType DecodeImmShiftWithSource(medusa::CpuInformation const* pCpuInfo, medusa::u32 Rm, medusa::u32 Type, medusa::u32 Imm);
}

#endif // !ARM_HELPER_HPP