#include <string>

#include "arm_helper.hpp"
#include <medusa/bits.hpp>
#include <medusa/extend.hpp>

namespace arm
{
  std::tuple<medusa::u32, bool> LSL_C(medusa::u32 Value, medusa::s32 Amount)
  {
    if (Amount <= 0)
      return std::make_tuple(Value, false);
    auto Result = Value << Amount;
    auto CarryOut = Value & (1 << (32 - Amount)) ? true : false;
    return std::make_tuple(Result, CarryOut);
  }

  std::tuple<medusa::u32, bool> LSR_C(medusa::u32 Value, medusa::s32 Amount)
  {
    if (Amount <= 0)
      return std::make_tuple(Value, false);
    auto Result = Value >> Amount;
    auto CarryOut = (Value & (1 << (Amount - 1))) ? true : false;
    return std::make_tuple(Result, CarryOut);
  }

  std::tuple<medusa::u32, bool> ASR_C(medusa::s32 Value, medusa::s32 Amount)
  {
    if (Amount <= 0)
      return std::make_tuple(Value, false);

    auto Result = Value >> Amount;
    auto CarryOut = (Value & (1 << (Amount - 1))) ? true : false;
    return std::make_tuple(Result, CarryOut);
  }

  std::tuple<medusa::u32, bool> ROR_C(medusa::u32 Value, medusa::s32 Amount)
  {
    if (Amount == 0)
      return std::make_tuple(Value, false);

    auto Shift = Amount % 32;
    auto Result = (Value >> Shift) | (Value << (32 - Shift));
    auto CarryOut = (Result & (1 << 31)) ? true : false;
    return std::make_tuple(Result, CarryOut);
  }

  std::tuple<medusa::u32, bool> RRX_C(medusa::u32 Value, bool CarryIn)
  {
    auto Result = (CarryIn ? (1 << 31) : 0x0) | (Value >> 1);
    auto CarryOut = (Value & 1) ? true : false;
    return std::make_tuple(Result, CarryOut);
  }


  std::tuple<SRType, medusa::u32> DecodeImmShift(medusa::u32 Type, medusa::u32 Imm)
  {
    switch (Type & 0x3)
    {
    case 0x0: return std::make_tuple(SRType_LSL, Imm);
    case 0x1: return std::make_tuple(SRType_LSR, Imm == 0 ? 0x20 : Imm);
    case 0x2: return std::make_tuple(SRType_ASR, Imm == 0 ? 0x20 : Imm);
    case 0x3:
      if (Imm == 0)
        return std::make_tuple(SRType_RRX, 1);
      else
        return std::make_tuple(SRType_ROR, Imm);
    default:
      return std::make_tuple(SRType_Unknown, 0);
    }
  }

  SRType DecodeRegShift(medusa::u32 Type)
  {
    switch (Type & 0x3)
    {
    case 0x0: return SRType_LSL;
    case 0x1: return SRType_LSR;
    case 0x2: return SRType_ASR;
    case 0x3: return SRType_ROR;
    default:  return SRType_Unknown;
    }
  }

  medusa::u32 RegisterFromValue(char const* pRegTypeName, medusa::u32 RegVal)
  {
    static const std::string GPR32 = "GPR32";
    medusa::u32 RegId = 0;
    if (GPR32 == pRegTypeName)
    {
      switch (RegVal)
      {
      case  0: return ARM_RegR0;
      case  1: return ARM_RegR1;
      case  2: return ARM_RegR2;
      case  3: return ARM_RegR3;
      case  4: return ARM_RegR4;
      case  5: return ARM_RegR5;
      case  6: return ARM_RegR6;
      case  7: return ARM_RegR7;
      case  8: return ARM_RegR8;
      case  9: return ARM_RegR9;
      case 10: return ARM_RegR10;
      case 11: return ARM_RegR11;
      case 12: return ARM_RegR12;
      case 13: return ARM_RegR13;
      case 14: return ARM_RegR14;
      case 15: return ARM_RegR15;
      default: break;
      }
    }

    return 0;
  }

  std::vector<medusa::u32> RegisterList(medusa::u32, medusa::u32 RegList)
  {
    std::vector<medusa::u32> Res;

    for (auto i = 0UL; i < 16; ++i)
      if (RegList & (1 << i))
        switch (i)
      {
        case  0: Res.push_back(ARM_RegR0);
        case  1: Res.push_back(ARM_RegR1);
        case  2: Res.push_back(ARM_RegR2);
        case  3: Res.push_back(ARM_RegR3);
        case  4: Res.push_back(ARM_RegR4);
        case  5: Res.push_back(ARM_RegR5);
        case  6: Res.push_back(ARM_RegR6);
        case  7: Res.push_back(ARM_RegR7);
        case  8: Res.push_back(ARM_RegR8);
        case  9: Res.push_back(ARM_RegR9);
        case 10: Res.push_back(ARM_RegR10);
        case 11: Res.push_back(ARM_RegR11);
        case 12: Res.push_back(ARM_RegR12);
        case 13: Res.push_back(ARM_RegR13);
        case 14: Res.push_back(ARM_RegR14);
        case 15: Res.push_back(ARM_RegR15);
      }

    return Res;
  }

  medusa::u32 ConditionFromValue(medusa::u32 CondVal)
  {
    return CondVal;
  }

  medusa::u32 ConditionFromName(char const* pCondName)
  {
    static std::unordered_map<std::string, medusa::u32> const s_CondNameToVal
    {
      { "eq", ARM_Cond_Eq },
      { "ne", ARM_Cond_Ne },
      { "cs", ARM_Cond_Cs },
      { "hs", ARM_Cond_Hs },
      { "cc", ARM_Cond_Cc },
      { "lo", ARM_Cond_Lo },
      { "mi", ARM_Cond_Mi },
      { "pl", ARM_Cond_Pl },
      { "vs", ARM_Cond_Vs },
      { "vc", ARM_Cond_Vc },
      { "hi", ARM_Cond_Hi },
      { "ls", ARM_Cond_Ls },
      { "ge", ARM_Cond_Ge },
      { "lt", ARM_Cond_Lt },
      { "gt", ARM_Cond_Gt },
      { "le", ARM_Cond_Le },
      { "al", ARM_Cond_Al },
    };

    auto const itCond = s_CondNameToVal.find(pCondName);
    if (itCond == std::end(s_CondNameToVal))
      return ARM_Cond_Unknown;
    return itCond->second;
  }

  medusa::u32 Shift(medusa::u32 Value, medusa::u32 Type, medusa::u32 Amount, bool CarryIn)
  {
    auto Result = Shift_C(Value, Type, Amount, CarryIn);
    return std::get<0>(Result);
  }

  std::tuple<medusa::u32, bool> Shift_C(medusa::u32 Value, medusa::u32 Type, medusa::u32 Amount, bool CarryIn)
  {
    if (Amount == 0)
      return std::make_tuple(Value, CarryIn);

    switch (Type)
    {
    case SRType_LSL: return LSL_C(Value, Amount);
    case SRType_LSR: return LSR_C(Value, Amount);
    case SRType_ASR: return ASR_C(Value, Amount);
    case SRType_ROR: return ROR_C(Value, Amount);
    case SRType_RRX: return RRX_C(Value, CarryIn);
    default:         return std::make_tuple(0, false);
    }
  }

  medusa::u32 ArmExpandImm(medusa::u32 Imm)
  {
    // TODO(wisk): figure out how APSR.C works in this case...
    auto Result = ArmExpandImm_C(Imm, 0);
    return std::get<0>(Result);
  }

  std::tuple<medusa::u32, bool> ArmExpandImm_C(medusa::u32 Imm, bool CarryIn)
  {
    auto UnrotatedValue = medusa::ExtractBits<0, 12>(Imm);
    auto Result = Shift_C(UnrotatedValue, SRType_ROR, medusa::ExtractBits<8, 11>(Imm) << 1, CarryIn);
    return Result;
  }

  medusa::Expression::SPType DecodeRegShiftWithSources(medusa::CpuInformation const* pCpuInfo, medusa::u32 Rm, medusa::u32 Type, medusa::u32 Rs)
  {
    auto Shift = DecodeRegShift(Type);
    if (Shift == SRType_Unknown)
      return nullptr;

    auto spRmExpr = medusa::Expr::MakeId(Rm, pCpuInfo);
    auto spRsExpr = medusa::Expr::MakeId(Rs, pCpuInfo);
    switch (Shift)
    {
    case SRType_LSL: return medusa::Expr::MakeBinOp(medusa::OperationExpression::OpLls, spRmExpr, spRsExpr);
    case SRType_LSR: return medusa::Expr::MakeBinOp(medusa::OperationExpression::OpLrs, spRmExpr, spRsExpr);
    case SRType_ASR: return medusa::Expr::MakeBinOp(medusa::OperationExpression::OpArs, spRmExpr, spRsExpr);
    case SRType_ROR: return medusa::Expr::MakeBinOp(medusa::OperationExpression::OpRor, spRmExpr, spRsExpr);
    default:         return nullptr;
    }
  }

  medusa::Expression::SPType DecodeImmShiftWithSource(medusa::CpuInformation const* pCpuInfo, medusa::u32 Rm, medusa::u32 Type, medusa::u32 Imm)
  {
    auto Result = DecodeImmShift(Type, Imm);

    auto spRegExpr = medusa::Expr::MakeId(Rm, pCpuInfo);
    auto spImmExpr = medusa::Expr::MakeBitVector(32, std::get<0>(Result));
    return medusa::Expr::MakeBinOp(medusa::OperationExpression::OpAdd, spRegExpr, spImmExpr);
  }
}