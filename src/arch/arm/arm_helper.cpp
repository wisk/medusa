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
      case  0: return ARM_Reg_R0;
      case  1: return ARM_Reg_R1;
      case  2: return ARM_Reg_R2;
      case  3: return ARM_Reg_R3;
      case  4: return ARM_Reg_R4;
      case  5: return ARM_Reg_R5;
      case  6: return ARM_Reg_R6;
      case  7: return ARM_Reg_R7;
      case  8: return ARM_Reg_R8;
      case  9: return ARM_Reg_R9;
      case 10: return ARM_Reg_R10;
      case 11: return ARM_Reg_R11;
      case 12: return ARM_Reg_R12;
      case 13: return ARM_Reg_R13;
      case 14: return ARM_Reg_R14;
      case 15: return ARM_Reg_R15;
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
        case  0: Res.push_back(ARM_Reg_R0);  break;
        case  1: Res.push_back(ARM_Reg_R1);  break;
        case  2: Res.push_back(ARM_Reg_R2);  break;
        case  3: Res.push_back(ARM_Reg_R3);  break;
        case  4: Res.push_back(ARM_Reg_R4);  break;
        case  5: Res.push_back(ARM_Reg_R5);  break;
        case  6: Res.push_back(ARM_Reg_R6);  break;
        case  7: Res.push_back(ARM_Reg_R7);  break;
        case  8: Res.push_back(ARM_Reg_R8);  break;
        case  9: Res.push_back(ARM_Reg_R9);  break;
        case 10: Res.push_back(ARM_Reg_R10); break;
        case 11: Res.push_back(ARM_Reg_R11); break;
        case 12: Res.push_back(ARM_Reg_R12); break;
        case 13: Res.push_back(ARM_Reg_R13); break;
        case 14: Res.push_back(ARM_Reg_R14); break;
        case 15: Res.push_back(ARM_Reg_R15); break;
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

    auto spRmExpr = medusa::Expr::MakeId(RegisterFromValue("GPR32", Rm), pCpuInfo);
    auto spRsExpr = medusa::Expr::MakeId(RegisterFromValue("GPR32", Rs), pCpuInfo);
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

    auto spRegExpr = medusa::Expr::MakeId(RegisterFromValue("GPR32", Rm), pCpuInfo);
    if (std::get<1>(Result) == 0x0)
      return spRegExpr;

    auto spImmExpr = medusa::Expr::MakeBitVector(32, std::get<0>(Result));
    medusa::OperationExpression::Type Op = medusa::OperationExpression::OpUnk;
    switch (std::get<0>(Result))
    {
    case SRType_LSL: Op = medusa::OperationExpression::OpLls; break;
    case SRType_LSR: Op = medusa::OperationExpression::OpLrs; break;
    case SRType_ASR: Op = medusa::OperationExpression::OpArs; break;
    case SRType_ROR: Op = medusa::OperationExpression::OpRor; break;
    case SRType_RRX:
    {
      auto spCarryExpr = medusa::Expr::MakeId(ARM_Fl_Cf, pCpuInfo);
      auto spCarry32Expr = medusa::Expr::MakeBinOp(medusa::OperationExpression::OpBcast, spCarryExpr, medusa::Expr::MakeBitVector(32, 32));
      auto spMsb = spCarry32Expr << 31;
      auto spRes = (spRegExpr >> spImmExpr) | spMsb;
      return spRes;
    }
    default: break;
    }

    return medusa::Expr::MakeBinOp(Op, spRegExpr, spImmExpr);
  }
}