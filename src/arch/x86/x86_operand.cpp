#include "x86_architecture.hpp"
#include <medusa/extend.hpp>

template<u64 Imm> struct OperandImm8
{
  bool operator()(Operand* pOprd)
  {
    pOprd->SetValue(Imm);
    pOprd->SetType(O_IMM8 | O_NO_REF);
    return true;
  }
};

template<X86_Reg Reg, u32 OpType> struct OperandReg
{
  bool operator()(Operand* pOprd)
  {
    pOprd->SetReg(Reg);
    pOprd->SetType(OpType);
    return true;
  }
};

template<X86_Reg Reg, X86_Reg Seg, u32 OpType> struct OperandRegSeg
{
  bool operator()(Operand* pOprd)
  {
    pOprd->SetReg(Reg);
    pOprd->SetSeg(Seg);
    pOprd->SetType(OpType | O_SEG);
    return true;
  }
};

template<X86_Reg Reg>              struct OperandReg8  :    public OperandReg<Reg, O_REG8         >{};
template<X86_Reg Reg>              struct OperandReg16 :    public OperandReg<Reg, O_REG16        >{};
template<X86_Reg Reg>              struct OperandReg32 :    public OperandReg<Reg, O_REG32        >{};
template<X86_Reg Reg>              struct OperandReg64 :    public OperandReg<Reg, O_REG64        >{};
template<X86_Reg Reg>              struct OperandReg80 :    public OperandReg<Reg, O_REG80        >{};

template<X86_Reg Reg, X86_Reg Seg> struct OperandReg8Seg  : public OperandRegSeg<Reg, Seg, O_REG8 >{};
template<X86_Reg Reg, X86_Reg Seg> struct OperandReg16Seg : public OperandRegSeg<Reg, Seg, O_REG16>{};
template<X86_Reg Reg, X86_Reg Seg> struct OperandReg32Seg : public OperandRegSeg<Reg, Seg, O_REG32>{};
template<X86_Reg Reg, X86_Reg Seg> struct OperandReg64Seg : public OperandRegSeg<Reg, Seg, O_REG64>{};

template<X86_Reg Reg16, X86_Reg Reg32> struct OperandReg16_32
{
  bool operator()(X86_Bit Bit, Instruction& rInsn, Operand* pOprd)
  {
    OperandReg16<Reg16> OpReg16;
    OperandReg32<Reg32> OpReg32;

    switch (Bit)
    {
    case X86_Bit_16:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize) return OpReg32(pOprd);
      else                                       return OpReg16(pOprd);

    case X86_Bit_32:
    case X86_Bit_64:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize) return OpReg16(pOprd);
      else                                       return OpReg32(pOprd);

    default:                                     return false;
    }
  }
};

template<X86_Reg Reg16, X86_Reg Reg32, X86_Reg Reg64> struct OperandReg16_32_64
{
  bool operator()(X86_Bit Bit, Instruction& rInsn, Operand* pOprd)
  {
    OperandReg16<Reg16> OpReg16;
    OperandReg32<Reg32> OpReg32;
    OperandReg64<Reg64> OpReg64;

    switch (Bit)
    {
    case X86_Bit_16:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize)                      return OpReg32(pOprd);
      else                                                            return OpReg16(pOprd);

    case X86_Bit_64:
      if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w) return OpReg64(pOprd);

    case X86_Bit_32:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize)                      return OpReg16(pOprd);
      else                                                            return OpReg32(pOprd);

    default:                                                          return false;
    }
  }
};

template<X86_Reg Reg16, X86_Reg Reg32, X86_Reg Reg64> struct OperandReg16_32_64_Mem
{
  bool operator()(X86_Bit Bit, Instruction& rInsn, Operand* pOprd)
  {
    OperandReg16<Reg16> OpReg16;
    OperandReg32<Reg32> OpReg32;
    OperandReg64<Reg64> OpReg64;

    bool Res = false;

    switch (Bit)
    {
    case X86_Bit_16:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize)
      {
        Res = OpReg32(pOprd);
        pOprd->Type() |= O_MEM32;
        break;
      }
      else
      {
        Res = OpReg16(pOprd);
        pOprd->Type() |= O_MEM16;
        break;
      }

    case X86_Bit_64:
      if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      {
        Res = OpReg64(pOprd);
        pOprd->Type() |= O_MEM64;
        break;
      }

    case X86_Bit_32:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize)
      {
        Res = OpReg16(pOprd);
        pOprd->Type() |= O_MEM16;
        break;
      }

      else
      {
        Res = OpReg32(pOprd);
        pOprd->Type() |= O_MEM32;
        break;
      }

    default: return false;
    }

    return Res;
  }
};

template<typename ConstType, u32 OpType> struct OperandRead
{
  bool operator()(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
  {
    ConstType ct;

    rBinStrm.Read(Offset, ct);
    pOprd->SetValue(ct);
    pOprd->SetType(OpType);
    pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
    rInsn.Length() += sizeof(ct);
    return true;
  }
};

template<typename ConstType, unsigned Pos> struct OperandReadSignExtendSetRelType
{
  bool operator()(BinaryStream const& rBinStrm, X86_Bit Bit, TOffset Offset, Instruction& rInsn, Operand* pOprd)
  {
    ConstType ct;

    rBinStrm.Read(Offset, ct);
    pOprd->SetValue(SignExtend<ConstType, Pos>(ct));
    switch (Bit)
    {
    case X86_Bit_16: pOprd->SetType(O_REL16); break;
    case X86_Bit_32: pOprd->SetType(O_REL32); break;
    case X86_Bit_64: pOprd->SetType(O_REL64); break;
    default:         pOprd->SetType(O_NONE);  break;
    }
    pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
    rInsn.Length() += sizeof(ct);
    return true;
  }
};

template<typename ConstType, u32 OpType, unsigned Pos> struct OperandReadSignExtend
{
  bool operator()(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
  {
    ConstType ct;

    rBinStrm.Read(Offset, ct);
    pOprd->SetValue(SignExtend<ConstType, Pos>(ct));
    pOprd->SetType(OpType);
    pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
    rInsn.Length() += sizeof(ct);
    return true;
  }
};

template<u32 OpType> struct OperandReadSign32Extend64
{
  bool operator()(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
  {
    u32 ct;

    rBinStrm.Read(Offset, ct);
    pOprd->SetValue(SignExtend<s64, 32>(ct));
    pOprd->SetType(OpType);
    pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
    rInsn.Length() += sizeof(ct);
    return true;
  }
};

struct OperandJb  : public OperandReadSignExtendSetRelType<s8,            8>{};
struct OperandJw  : public OperandReadSignExtend          <s16, O_REL16, 16>{};
struct OperandJd  : public OperandReadSignExtend          <s32, O_REL32, 32>{};
struct OperandJqs : public OperandReadSign32Extend64      <     O_REL64    >{};

struct OperandJv
{
  bool operator()(BinaryStream const& rBinStrm, X86_Bit Bit, TOffset Offset, Instruction& rInsn, Operand* pOprd)
  {
    OperandJw  OpJw;
    OperandJd  OpJd;
    OperandJqs OpJqs;

    switch (Bit)
    {
    case X86_Bit_16:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize) return OpJd(rBinStrm, Offset, rInsn, pOprd);
      else                                       return OpJw(rBinStrm, Offset, rInsn, pOprd);

    case X86_Bit_32:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize) return OpJw(rBinStrm, Offset, rInsn, pOprd);
      else                                       return OpJd(rBinStrm, Offset, rInsn, pOprd);

    case X86_Bit_64:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize) return OpJw(rBinStrm, Offset, rInsn, pOprd);
      else                                       return OpJqs(rBinStrm, Offset, rInsn, pOprd);

    default:                                     return false;
    }
  }
};

struct OperandOw : public OperandRead<u16, O_DISP16>{};
struct OperandOd : public OperandRead<u32, O_DISP32>{};
struct OperandOq : public OperandRead<u64, O_DISP64>{};
struct OperandOb // Not sure for this one...
{
  bool operator()(BinaryStream const& rBinStrm, X86_Bit Bit, TOffset Offset, Instruction& rInsn, Operand* pOprd)
  {
    OperandOw OpOw;
    OperandOd OpOd;
    OperandOq OpOq;

    switch (Bit)
    {
    case X86_Bit_16: if (!OpOw(rBinStrm, Offset, rInsn, pOprd)) return false; break;
    case X86_Bit_32: if (!OpOd(rBinStrm, Offset, rInsn, pOprd)) return false; break;
    case X86_Bit_64: if (!OpOq(rBinStrm, Offset, rInsn, pOprd)) return false; break;
    default:         return false;
    }

    pOprd->Type() |= O_MEM8;
    return true;
  }
};
struct OperandOv
{
  bool operator()(BinaryStream const& rBinStrm, X86_Bit Bit, TOffset Offset, Instruction& rInsn, Operand* pOprd)
  {
    OperandOw OpOw;
    OperandOd OpOd;
    OperandOq OpOq;

    switch (Bit)
    {
    case X86_Bit_16: if (!OpOw(rBinStrm, Offset, rInsn, pOprd)) return false; break;
    case X86_Bit_32: if (!OpOd(rBinStrm, Offset, rInsn, pOprd)) return false; break;
    case X86_Bit_64: if (!OpOq(rBinStrm, Offset, rInsn, pOprd)) return false; break;
    default:         return false;
    }

    u32 Mem = O_NONE;

    switch (Bit)
    {
    case X86_Bit_16:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize) Mem = O_MEM32;
      else                                       Mem = O_MEM16;
      break;

    case X86_Bit_64:
      if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      {
        Mem = O_MEM64;
        break;
      }
    case X86_Bit_32:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize) Mem = O_MEM16;
      else                                       Mem = O_MEM32;
      break;

    default: break;
    }

    pOprd->Type() |= Mem;
    return true;
  }
};

struct OperandIb  : public OperandRead<u8,  O_IMM8  | O_NO_REF>{};
struct OperandIw  : public OperandRead<u16, O_IMM16 | O_NO_REF>{};
struct OperandId  : public OperandRead<u32, O_IMM32>{};
struct OperandIq  : public OperandRead<u64, O_IMM64>{};
struct OperandIds : public OperandReadSign32Extend64<O_IMM64>{};
struct OperandIv
{
  bool operator()(BinaryStream const& rBinStrm, X86_Bit Bit, TOffset Offset, Instruction& rInsn, Operand* pOprd)
  {
    OperandIw OpIw;
    OperandId OpId;
    OperandIq OpIq;

    switch (Bit)
    {
    case X86_Bit_16:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize)                      return OpId(rBinStrm, Offset, rInsn, pOprd);
      else                                                            return OpIw(rBinStrm, Offset, rInsn, pOprd);

    case X86_Bit_64:
      if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w) return OpIq(rBinStrm, Offset, rInsn, pOprd);

    case X86_Bit_32:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize)                      return OpIw(rBinStrm, Offset, rInsn, pOprd);
      else                                                            return OpId(rBinStrm, Offset, rInsn, pOprd);

    default:                                                          return false;
    }
  }
};
struct OperandIz
{
  bool operator()(BinaryStream const& rBinStrm, X86_Bit Bit, TOffset Offset, Instruction& rInsn, Operand* pOprd)
  {
    OperandIw OpIw;
    OperandId OpId;
    OperandIds OpIds;

    switch (Bit)
    {
    case X86_Bit_16:
      if (rInsn.GetPrefix() & X86_Prefix_OpSize)                           return OpId(rBinStrm, Offset, rInsn, pOprd);
      else                                                                 return OpIw(rBinStrm, Offset, rInsn, pOprd);
    case X86_Bit_32:
    case X86_Bit_64:
      if       (rInsn.GetPrefix() & X86_Prefix_OpSize)                     return OpIw(rBinStrm, Offset, rInsn, pOprd);
      else if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w) return OpIds(rBinStrm, Offset, rInsn, pOprd);
      else                                                                 return OpId(rBinStrm, Offset, rInsn, pOprd);
    default:                                                               return false;
    }
  }
};

template<typename OffType, u32 OpType> struct OperandLogicAddr
{
  bool operator()(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
  {
    u16 Seg;
    OffType Off;

    rBinStrm.Read(Offset + sizeof(OffType), Seg);
    rBinStrm.Read(Offset, Off);
    rInsn.Length() += (sizeof(Seg) + sizeof(Off));
    pOprd->SetValue(Off);
    pOprd->SetSegValue(Seg);
    pOprd->SetType(OpType | O_SEG_VAL);
    return true;
  }
};

struct OperandLogicAddr16 : public OperandLogicAddr<u16, O_MEM16 | O_DISP16>{};
struct OperandLogicAddr32 : public OperandLogicAddr<u32, O_MEM32 | O_DISP32>{};

template<u32 OpType> struct OperandIbs : public OperandReadSignExtend<s8, OpType, 8>{};

static x86::ModRM GetModRm(BinaryStream const& rBinStrm, TOffset Offset)
{
  u8 ModRmByte;
  rBinStrm.Read(Offset, ModRmByte);
  return ModRmByte;
}

bool X86Architecture::Decode_1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandImm8<1> OpImm1;
  return OpImm1(pOprd);
}

bool X86Architecture::Decode_3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandImm8<3> OpImm3;
  return OpImm3(pOprd);
}

bool X86Architecture::Decode_AH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_Ah> OpRegAh;
  return OpRegAh(pOprd);
}

bool X86Architecture::Decode_AL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_Al> OpRegAl;
  return OpRegAl(pOprd);
}

bool X86Architecture::Decode_AX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16<X86_Reg_Ax> OpRegAx;
  return OpRegAx(pOprd);
}

bool X86Architecture::Decode_Ap(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandLogicAddr16 OpLogicAddr16;
  OperandLogicAddr32 OpLogicAddr32;

  switch (static_cast<X86_Bit>(Mode))
  {
  case X86_Bit_16:
    if (rInsn.GetPrefix() & X86_Prefix_OpSize) return OpLogicAddr32(rBinStrm, Offset, rInsn, pOprd);
    else                                       return OpLogicAddr16(rBinStrm, Offset, rInsn, pOprd);

  case X86_Bit_32:
    if (rInsn.GetPrefix() & X86_Prefix_OpSize) return OpLogicAddr16(rBinStrm, Offset, rInsn, pOprd);
    else                                       return OpLogicAddr32(rBinStrm, Offset, rInsn, pOprd);

  default:                                     return false;
  }
}

bool X86Architecture::Decode_BH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_Bh> OpRegBh;
  return OpRegBh(pOprd);
}

bool X86Architecture::Decode_BL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_Bl> OpRegBl;
  return OpRegBl(pOprd);
}

bool X86Architecture::Decode_CH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_Ch> OpRegCh;
  return OpRegCh(pOprd);
}

bool X86Architecture::Decode_CL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_Cl> OpRegCl;
  return OpRegCl(pOprd);
}

bool X86Architecture::Decode_CS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16<X86_Reg_Cs> OpRegCs;
  return OpRegCs(pOprd);
}

bool X86Architecture::Decode_Cy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  static u16 aReg [] = { X86_Reg_Cr0, X86_Reg_Cr1, X86_Reg_Cr2,  X86_Reg_Cr3,  X86_Reg_Cr4,  X86_Reg_Cr5,  X86_Reg_Cr6,  X86_Reg_Cr7  };
  static u16 aRegR[] = { X86_Reg_Cr8, X86_Reg_Cr9, X86_Reg_Cr10, X86_Reg_Cr11, X86_Reg_Cr12, X86_Reg_Cr13, X86_Reg_Cr14, X86_Reg_Cr15 };

  x86::ModRM ModRm = GetModRm(rBinStrm, Offset);

  pOprd->Type() |= static_cast<X86_Bit>(Mode) == X86_Bit_64 ? O_REG64 : O_REG32;

  if (rInsn.GetPrefix() & X86_Prefix_REX_r)
    pOprd->Reg() = aRegR[ModRm.Reg()];
  else
    pOprd->Reg() = aReg[ModRm.Reg()];

  return true;
}

bool X86Architecture::Decode_DH(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_Dh> OpRegDh;
  return OpRegDh(pOprd);
}

bool X86Architecture::Decode_DL(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_Dl> OpRegDl;
  return OpRegDl(pOprd);
}

bool X86Architecture::Decode_DS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16<X86_Reg_Ds> OpRegDs;
  return OpRegDs(pOprd);
}

bool X86Architecture::Decode_DX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16<X86_Reg_Dx> OpRegDx;
  return OpRegDx(pOprd);
}

bool X86Architecture::Decode_Dy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  static u16 aReg [] = { X86_Reg_Dr0, X86_Reg_Dr1, X86_Reg_Dr2,  X86_Reg_Dr3,  X86_Reg_Dr4,  X86_Reg_Dr5,  X86_Reg_Dr6,  X86_Reg_Dr7  };
  static u16 aRegR[] = { X86_Reg_Dr8, X86_Reg_Dr9, X86_Reg_Dr10, X86_Reg_Dr11, X86_Reg_Dr12, X86_Reg_Dr13, X86_Reg_Dr14, X86_Reg_Dr15 };

  x86::ModRM ModRm = GetModRm(rBinStrm, Offset);

  pOprd->Type() |= static_cast<X86_Bit>(Mode) == X86_Bit_64 ? O_REG64 : O_REG32;

  if (rInsn.GetPrefix() & X86_Prefix_REX_r)
    pOprd->Reg() = aRegR[ModRm.Reg()];
  else
    pOprd->Reg() = aReg[ModRm.Reg()];

  return true;
}

bool X86Architecture::Decode_ES(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16<X86_Reg_Es> OpRegEs;
  return OpRegEs(pOprd);
}

bool X86Architecture::Decode_Fv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  /* XXX: By convention we can safetly ignore this operand */
  return true;
}

bool X86Architecture::Decode_Ho(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  return false; /* UNHANDLED OPERAND */
}

bool X86Architecture::Decode_Hx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  return false; /* UNHANDLED OPERAND */
}

bool X86Architecture::Decode_Ib(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandIb OpIb;
  return OpIb(rBinStrm, Offset, rInsn, pOprd);
}

bool X86Architecture::Decode_Ibs(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandIbs<O_IMM16> OpIbsw;
  OperandIbs<O_IMM32> OpIbsd;
  OperandIbs<O_IMM64> OpIbsq;

  auto const rFirstOprd = rInsn.FirstOperand();
  switch (rFirstOprd.GetLength())
  {
  case 2: return OpIbsw(rBinStrm, Offset, rInsn, pOprd);
  case 4: return OpIbsd(rBinStrm, Offset, rInsn, pOprd);
  case 8: return OpIbsq(rBinStrm, Offset, rInsn, pOprd);
  default:return Decode_Ib(rBinStrm, Offset, rInsn, pOprd, Mode);
  }
}

bool X86Architecture::Decode_Iv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandIv OpIv;
  return OpIv(rBinStrm, static_cast<X86_Bit>(Mode), Offset, rInsn, pOprd);
}

bool X86Architecture::Decode_Iw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandIw OpIw;
  return OpIw(rBinStrm, Offset, rInsn, pOprd);
}

bool X86Architecture::Decode_Iz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandIz OpIz;
  return OpIz(rBinStrm, static_cast<X86_Bit>(Mode), Offset, rInsn, pOprd);
}

bool X86Architecture::Decode_Jb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandJb OpJb;
  return OpJb(rBinStrm, static_cast<X86_Bit>(Mode), Offset, rInsn, pOprd);
}

bool X86Architecture::Decode_Jz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandJv OpJv;
  return OpJv(rBinStrm, static_cast<X86_Bit>(Mode), Offset, rInsn, pOprd);
}

bool X86Architecture::Decode_Ob(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandOb OpOb;
  return OpOb(rBinStrm, static_cast<X86_Bit>(Mode), Offset, rInsn, pOprd);
}

bool X86Architecture::Decode_Ov(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandOv OpOv;
  return OpOv(rBinStrm, static_cast<X86_Bit>(Mode), Offset, rInsn, pOprd);
}

bool X86Architecture::Decode_ST0(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg80<X86_Reg_St0> OpRegSt0;
  return OpRegSt0(pOprd);
}

bool X86Architecture::Decode_ST1(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg80<X86_Reg_St1> OpRegSt1;
  return OpRegSt1(pOprd);
}

bool X86Architecture::Decode_ST2(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg80<X86_Reg_St2> OpRegSt2;
  return OpRegSt2(pOprd);
}

bool X86Architecture::Decode_ST3(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg80<X86_Reg_St3> OpRegSt3;
  return OpRegSt3(pOprd);
}

bool X86Architecture::Decode_ST4(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg80<X86_Reg_St4> OpRegSt4;
  return OpRegSt4(pOprd);
}

bool X86Architecture::Decode_ST5(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg80<X86_Reg_St5> OpRegSt5;
  return OpRegSt5(pOprd);
}

bool X86Architecture::Decode_ST6(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg80<X86_Reg_St6> OpRegSt6;
  return OpRegSt6(pOprd);
}

bool X86Architecture::Decode_ST7(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg80<X86_Reg_St7> OpRegSt7;
  return OpRegSt7(pOprd);
}

bool X86Architecture::Decode_Sw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  static u16 aReg[] = { X86_Reg_Es, X86_Reg_Cs, X86_Reg_Ss, X86_Reg_Ds, X86_Reg_Fs, X86_Reg_Gs, X86_Reg_Seg6, X86_Reg_Seg7 };
  pOprd->Type() |= O_REG16;
  pOprd->Reg() = aReg[GetModRm(rBinStrm, Offset).Reg()];
  return true;
}

bool X86Architecture::Decode_Ty(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  static u16 aReg [] = { X86_Reg_Tr0, X86_Reg_Tr1, X86_Reg_Tr2,  X86_Reg_Tr3,  X86_Reg_Tr4,  X86_Reg_Tr5,  X86_Reg_Tr6,  X86_Reg_Tr7  };

  x86::ModRM ModRm = GetModRm(rBinStrm, Offset);

  pOprd->Type() |= static_cast<X86_Bit>(Mode) == X86_Bit_64 ? O_REG64 : O_REG32;
  pOprd->Reg() = aReg[ModRm.Reg()];

  return true;
}

bool X86Architecture::Decode_Xb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_Si, X86_Reg_Esi, X86_Reg_Rsi> OpSi;
  if (OpSi(static_cast<X86_Bit>(Mode), rInsn, pOprd) == false)
    return false;
  pOprd->Type() |= O_MEM8;
  pOprd->SetSeg(X86_Reg_Ds);
  pOprd->Type() |= O_SEG;
  return true;
}

bool X86Architecture::Decode_Xv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64_Mem<X86_Reg_Si, X86_Reg_Esi, X86_Reg_Rsi> OpSi;
  if (OpSi(static_cast<X86_Bit>(Mode), rInsn, pOprd) == false)
    return false;
  pOprd->SetSeg(X86_Reg_Ds);
  pOprd->Type() |= O_SEG;
  return true;
}

bool X86Architecture::Decode_Xz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64_Mem<X86_Reg_Si, X86_Reg_Esi, X86_Reg_Rsi> OpSi;
  if (OpSi(static_cast<X86_Bit>(Mode), rInsn, pOprd) == false)
    return false;
  pOprd->SetSeg(X86_Reg_Ds);
  pOprd->Type() |= O_SEG;
  return true;
}

bool X86Architecture::Decode_Yb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_Di, X86_Reg_Edi, X86_Reg_Rdi> OpDi;
  if (OpDi(static_cast<X86_Bit>(Mode), rInsn, pOprd) == false)
    return false;
  pOprd->Type() |= O_MEM8;
  pOprd->SetSeg(X86_Reg_Es);
  pOprd->Type() |= O_SEG;
  return true;
}

bool X86Architecture::Decode_Yv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64_Mem<X86_Reg_Di, X86_Reg_Edi, X86_Reg_Rdi> OpDi;
  if (OpDi(static_cast<X86_Bit>(Mode), rInsn, pOprd) == false)
    return false;
  pOprd->SetSeg(X86_Reg_Es);
  pOprd->Type() |= O_SEG;
  return true;
}

bool X86Architecture::Decode_Yz(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64_Mem<X86_Reg_Di, X86_Reg_Edi, X86_Reg_Rdi> OpDi;
  if (OpDi(static_cast<X86_Bit>(Mode), rInsn, pOprd) == false)
    return false;
  pOprd->SetSeg(X86_Reg_Es);
  pOprd->Type() |= O_SEG;
  return true;
}

bool X86Architecture::Decode_d(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  return false; /* UNHANDLED OPERAND */
}

bool X86Architecture::Decode_eAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32<X86_Reg_Ax, X86_Reg_Eax> OpRegA;
  return OpRegA(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_eBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32<X86_Reg_Bp, X86_Reg_Ebp> OpRegBp;
  return OpRegBp(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_eBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32<X86_Reg_Bx, X86_Reg_Ebx> OpRegB;
  return OpRegB(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_eCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32<X86_Reg_Cx, X86_Reg_Ecx> OpRegC;
  return OpRegC(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_eDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32<X86_Reg_Di, X86_Reg_Edi> OpRegDi;
  return OpRegDi(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_eDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32<X86_Reg_Dx, X86_Reg_Edx> OpRegD;
  return OpRegD(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_eSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32<X86_Reg_Si, X86_Reg_Esi> OpRegSi;
  return OpRegSi(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_eSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32<X86_Reg_Sp, X86_Reg_Esp> OpRegSp;
  return OpRegSp(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_FS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16<X86_Reg_Fs> OpRegFs;
  return OpRegFs(pOprd);
}

bool X86Architecture::Decode_GS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16<X86_Reg_Gs> OpRegGs;
  return OpRegGs(pOprd);
}

bool X86Architecture::Decode_q(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  return false; /* UNHANDLED OPERAND */
}

bool X86Architecture::Decode_r10(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_R10w, X86_Reg_R10d, X86_Reg_R10> OpRegR10;
  return OpRegR10(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_r10b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_R10b> OpRegR10b;
  return OpRegR10b(pOprd);
}

bool X86Architecture::Decode_r11(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_R11w, X86_Reg_R11d, X86_Reg_R11> OpRegR11;
  return OpRegR11(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_r11b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_R11b> OpRegR11b;
  return OpRegR11b(pOprd);
}

bool X86Architecture::Decode_r12(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_R12w, X86_Reg_R12d, X86_Reg_R12> OpRegR12;
  return OpRegR12(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_r12b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_R12b> OpRegR12b;
  return OpRegR12b(pOprd);
}

bool X86Architecture::Decode_r13(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_R13w, X86_Reg_R13d, X86_Reg_R13> OpRegR13;
  return OpRegR13(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_r13b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_R13b> OpRegR13b;
  return OpRegR13b(pOprd);
}

bool X86Architecture::Decode_r14(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_R14w, X86_Reg_R14d, X86_Reg_R14> OpRegR14;
  return OpRegR14(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_r14b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_R14b> OpRegR14b;
  return OpRegR14b(pOprd);
}

bool X86Architecture::Decode_r15(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_R15w, X86_Reg_R15d, X86_Reg_R15> OpRegR15;
  return OpRegR15(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_r15b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_R15b> OpRegR15b;
  return OpRegR15b(pOprd);
}

bool X86Architecture::Decode_r8(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_R8w, X86_Reg_R8d, X86_Reg_R8> OpRegR8;
  return OpRegR8(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_r8b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_R8b> OpRegR8b;
  return OpRegR8b(pOprd);
}

bool X86Architecture::Decode_r9(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_R9w, X86_Reg_R9d, X86_Reg_R9> OpRegR9;
  return OpRegR9(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_r9b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg8<X86_Reg_R9b> OpRegR9b;
  return OpRegR9b(pOprd);
}

bool X86Architecture::Decode_rAX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_Ax, X86_Reg_Eax, X86_Reg_Rax> OpRegA;
  return OpRegA(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_rBP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_Bp, X86_Reg_Ebp, X86_Reg_Rbp> OpRegBp;
  return OpRegBp(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_rBX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_Bx, X86_Reg_Ebx, X86_Reg_Rbx> OpRegB;
  return OpRegB(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_rCX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_Cx, X86_Reg_Ecx, X86_Reg_Rcx> OpRegC;
  return OpRegC(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_rDI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_Di, X86_Reg_Edi, X86_Reg_Rdi> OpRegDi;
  return OpRegDi(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_rDX(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_Dx, X86_Reg_Edx, X86_Reg_Rdx> OpRegD;
  return OpRegD(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_rSI(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_Si, X86_Reg_Esi, X86_Reg_Rsi> OpRegSi;
  return OpRegSi(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_rSP(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16_32_64<X86_Reg_Sp, X86_Reg_Esp, X86_Reg_Rsp> OpRegSp;
  return OpRegSp(static_cast<X86_Bit>(Mode), rInsn, pOprd);
}

bool X86Architecture::Decode_SS(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)
{
  OperandReg16<X86_Reg_Ss> OpRegSs;
  return OpRegSs(pOprd);
}

bool X86Architecture::Decode_Woqo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Wq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Moq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Wod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Uod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Wow(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Vy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Hy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_By(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_b(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_x(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_w(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Woo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Wy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Edb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Eyb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Eyw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Lx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Mod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Mqo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Mv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Vod(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Voq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Wd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Wob(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
bool X86Architecture::Decode_Woq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode){ return false; } // TODO
