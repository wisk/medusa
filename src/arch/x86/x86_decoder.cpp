#include "x86_architecture.hpp"
#include <medusa/extend.hpp>

// ref: http://wiki.osdev.org/X86-64_Instruction_Encoding

static Expression::SPType __GetRegisterFromIndex(CpuInformation const* pCpuInfo, u8 RegIdx, u32 RegList[16])
{
  if (RegIdx >= 0x10)
    return nullptr;
  auto Reg = RegList[RegIdx];
  if (Reg == X86_Reg_Unknown)
    return nullptr;
  return Expr::MakeId(Reg, pCpuInfo);
}

static Expression::SPType __GetSegmentRegister(CpuInformation const* pCpuInfo, Instruction const& rInsn, u32 DefReg = X86_Reg_Unknown)
{
  u32 Reg = X86_Reg_Unknown;

  if (rInsn.GetPrefix() & X86_Prefix_CS)
    Reg = X86_Reg_Cs;
  else if (rInsn.GetPrefix() & X86_Prefix_DS)
    Reg = X86_Reg_Ds;
  else if (rInsn.GetPrefix() & X86_Prefix_ES)
    Reg = X86_Reg_Es;
  else if (rInsn.GetPrefix() & X86_Prefix_FS)
    Reg = X86_Reg_Fs;
  else if (rInsn.GetPrefix() & X86_Prefix_GS)
    Reg = X86_Reg_Gs;

  if (Reg != X86_Reg_Unknown)
    return Expr::MakeId(Reg, pCpuInfo);

  if (DefReg != X86_Reg_Unknown)
    return Expr::MakeId(DefReg, pCpuInfo);

  return nullptr;
}

static u8 __ModRmMemoryRegister(Instruction const& rInsn, x86::ModRM const& rModRm)
{
  return ((rInsn.GetPrefix() & X86_Prefix_REX_b) == X86_Prefix_REX_b) ?
    rModRm.Rm() | 0x08 : rModRm.Rm();
}

static u8 __ModRmRegister(Instruction const& rInsn, x86::ModRM const& rModRm)
{
  return ((rInsn.GetPrefix() & X86_Prefix_REX_r) == X86_Prefix_REX_r) ?
    rModRm.Reg() | 0x08 : rModRm.Reg();
}

static u32 __GetOperandSize(Instruction const& rInsn, u8 Mode)
{
  switch (Mode)
  {
  case 16:
    return (rInsn.GetPrefix() & X86_Prefix_OpSize) ? 32 : 16;

  case 64:
    if ((rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w)
      return 64;

    // TODO: Handle CS.d
  case 32:
    return (rInsn.GetPrefix() & X86_Prefix_OpSize) ? 16 : 32;

  default:
    assert(0 && "Invalid mode");
    return 0;
  }
}

static u8 __GetAddressSize(Instruction const& rInsn, u8 Mode)
{
  switch (Mode)
  {
  case 16:
    return (rInsn.GetPrefix() & X86_Prefix_AdSize) ? 32 : 16;

  case 32:
    return (rInsn.GetPrefix() & X86_Prefix_AdSize) ? 16 : 32;

  case 64:
    return (rInsn.GetPrefix() & X86_Prefix_AdSize) ? 32 : 64;

  default:
    assert(0 && "Invalid mode");
    return 0;
  }
}

static u32 s_GP8[16]    = { X86_Reg_Al,   X86_Reg_Cl,   X86_Reg_Dl,   X86_Reg_Bl,   X86_Reg_Ah,   X86_Reg_Ch,   X86_Reg_Dh,      X86_Reg_Bh,      X86_Reg_R8b,     X86_Reg_R9b,     X86_Reg_R10b,    X86_Reg_R11b,    X86_Reg_R12b,    X86_Reg_R13b,    X86_Reg_R14b,    X86_Reg_R15b    };
static u32 s_GP8Rex[16] = { X86_Reg_Al,   X86_Reg_Cl,   X86_Reg_Dl,   X86_Reg_Bl,   X86_Reg_Spl,  X86_Reg_Bpl,  X86_Reg_Sil,     X86_Reg_Dil,     X86_Reg_R8b,     X86_Reg_R9b,     X86_Reg_R10b,    X86_Reg_R11b,    X86_Reg_R12b,    X86_Reg_R13b,    X86_Reg_R14b,    X86_Reg_R15b    };
static u32 s_GP16[16]   = { X86_Reg_Ax,   X86_Reg_Cx,   X86_Reg_Dx,   X86_Reg_Bx,   X86_Reg_Sp,   X86_Reg_Bp,   X86_Reg_Si,      X86_Reg_Di,      X86_Reg_R8w,     X86_Reg_R9w,     X86_Reg_R10w,    X86_Reg_R11w,    X86_Reg_R12w,    X86_Reg_R13w,    X86_Reg_R14w,    X86_Reg_R15w    };
static u32 s_GP32[16]   = { X86_Reg_Eax,  X86_Reg_Ecx,  X86_Reg_Edx,  X86_Reg_Ebx,  X86_Reg_Esp,  X86_Reg_Ebp,  X86_Reg_Esi,     X86_Reg_Edi,     X86_Reg_R8d,     X86_Reg_R9d,     X86_Reg_R10d,    X86_Reg_R11d,    X86_Reg_R12d,    X86_Reg_R13d,    X86_Reg_R14d,    X86_Reg_R15d    };
static u32 s_GP64[16]   = { X86_Reg_Rax,  X86_Reg_Rcx,  X86_Reg_Rdx,  X86_Reg_Rbx,  X86_Reg_Rsp,  X86_Reg_Rbp,  X86_Reg_Rsi,     X86_Reg_Rdi,     X86_Reg_R8,      X86_Reg_R9,      X86_Reg_R10,     X86_Reg_R11,     X86_Reg_R12,     X86_Reg_R13,     X86_Reg_R14,     X86_Reg_R15     };
static u32 s_x87[16]    = { X86_Reg_St0,  X86_Reg_St1,  X86_Reg_St2,  X86_Reg_St3,  X86_Reg_St4,  X86_Reg_St5,  X86_Reg_St6,     X86_Reg_St7,     X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown };
static u32 s_MMX[16]    = { X86_Reg_Mm0,  X86_Reg_Mm1,  X86_Reg_Mm2,  X86_Reg_Mm3,  X86_Reg_Mm4,  X86_Reg_Mm5,  X86_Reg_Mm6,     X86_Reg_Mm7,     X86_Reg_Mm0,     X86_Reg_Mm1,     X86_Reg_Mm2,     X86_Reg_Mm3,     X86_Reg_Mm4,     X86_Reg_Mm5,     X86_Reg_Mm6,     X86_Reg_Mm7     };
static u32 s_XMM[16]    = { X86_Reg_Xmm0, X86_Reg_Xmm1, X86_Reg_Xmm2, X86_Reg_Xmm3, X86_Reg_Xmm4, X86_Reg_Xmm5, X86_Reg_Xmm6,    X86_Reg_Xmm7,    X86_Reg_Xmm8,    X86_Reg_Xmm9,    X86_Reg_Xmm10,   X86_Reg_Xmm11,   X86_Reg_Xmm12,   X86_Reg_Xmm13,   X86_Reg_Xmm14,   X86_Reg_Xmm15   };
//static u32 s_YMM[16]    = { X86_Reg_Ymm0, X86_Reg_Ymm1, X86_Reg_Ymm2, X86_Reg_Ymm3, X86_Reg_Ymm4, X86_Reg_Ymm5, X86_Reg_Ymm6,    X86_Reg_Ymm7,    X86_Reg_Ymm8,    X86_Reg_Ymm9,    X86_Reg_Ymm10,   X86_Reg_Ymm11,   X86_Reg_Ymm12,   X86_Reg_Ymm13,   X86_Reg_Ymm14,   X86_Reg_Ymm15   };
//static u32 s_ZMM[16]    = { X86_Reg_Zmm0, X86_Reg_Zmm1, X86_Reg_Zmm2, X86_Reg_Zmm3, X86_Reg_Zmm4, X86_Reg_Zmm5, X86_Reg_Zmm6,    X86_Reg_Zmm7,    X86_Reg_Zmm8,    X86_Reg_Zmm9,    X86_Reg_Zmm10,   X86_Reg_Zmm11,   X86_Reg_Zmm12,   X86_Reg_Zmm13,   X86_Reg_Zmm14,   X86_Reg_Zmm15   };
static u32 s_SEG[16]    = { X86_Reg_Es,   X86_Reg_Cs,   X86_Reg_Ss,   X86_Reg_Ds,   X86_Reg_Fs,   X86_Reg_Gs,   X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Es,      X86_Reg_Cs,      X86_Reg_Ss,      X86_Reg_Ds,      X86_Reg_Fs,      X86_Reg_Gs,      X86_Reg_Unknown, X86_Reg_Unknown };
static u32 s_CTR[16]    = { X86_Reg_Cr0,  X86_Reg_Cr1,  X86_Reg_Cr2,  X86_Reg_Cr3,  X86_Reg_Cr4,  X86_Reg_Cr5,  X86_Reg_Cr6,     X86_Reg_Cr7,     X86_Reg_Cr8,     X86_Reg_Cr9,     X86_Reg_Cr10,    X86_Reg_Cr11,    X86_Reg_Cr12,    X86_Reg_Cr13,    X86_Reg_Cr14,    X86_Reg_Cr15    };
static u32 s_DBG[16]    = { X86_Reg_Dr0,  X86_Reg_Dr1,  X86_Reg_Dr2,  X86_Reg_Dr3,  X86_Reg_Dr4,  X86_Reg_Dr5,  X86_Reg_Dr6,     X86_Reg_Dr7,     X86_Reg_Dr8,     X86_Reg_Dr9,     X86_Reg_Dr10,    X86_Reg_Dr11,    X86_Reg_Dr12,    X86_Reg_Dr13,    X86_Reg_Dr14,    X86_Reg_Dr15    };
static u32 s_TST[16]    = { X86_Reg_Tr0,  X86_Reg_Tr1,  X86_Reg_Tr2,  X86_Reg_Tr3,  X86_Reg_Tr4,  X86_Reg_Tr5,  X86_Reg_Tr6,     X86_Reg_Tr7,     X86_Reg_Tr0,     X86_Reg_Tr1,     X86_Reg_Tr2,     X86_Reg_Tr3,     X86_Reg_Tr4,     X86_Reg_Tr5,     X86_Reg_Tr6,     X86_Reg_Tr7     };

static x86::ModRM __GetModRm(BinaryStream const& rBinStrm, OffsetType Offset)
{
  u8 ModRmByte;
  rBinStrm.Read(Offset, ModRmByte);
  return ModRmByte;
}

static x86::Sib __GetSib(BinaryStream const& rBinStrm, OffsetType Offset)
{
  u8 SibByte;
  rBinStrm.Read(Offset, SibByte);
  return SibByte;
}

//ref: http://sandpile.org/x86/opc_rm16.htm
static Expression::SPType __DecodeModRmAddress16(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, OffsetType Offset, Instruction const& rInsn, u8& OprdLen)
{
  OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  // Mem
  Expression::SPType spOff;
  switch (ModRm.Rm())
  {
  case 0x0:
    spOff = Expr::MakeBinOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Bx, pCpuInfo), Expr::MakeId(X86_Reg_Si, pCpuInfo));
    break;
  case 0x1:
    spOff = Expr::MakeBinOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Bx, pCpuInfo), Expr::MakeId(X86_Reg_Di, pCpuInfo));
    break;
  case 0x2:
    spOff = Expr::MakeBinOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Bp, pCpuInfo), Expr::MakeId(X86_Reg_Si, pCpuInfo));
    break;
  case 0x3:
    spOff = Expr::MakeBinOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Bp, pCpuInfo), Expr::MakeId(X86_Reg_Di, pCpuInfo));
    break;
  case 0x4:
    spOff = Expr::MakeId(X86_Reg_Si, pCpuInfo);
    break;
  case 0x5:
    spOff = Expr::MakeId(X86_Reg_Di, pCpuInfo);
    break;

  case 0x6:
    {
      // Mode
      if ((ModRm.Mod()) == 0x0)
      {
        u16 Disp16;
        rBinStrm.Read(Offset + 1, Disp16);
        spOff = Expr::MakeBitVector(16, Disp16);
        OprdLen += sizeof(Disp16);
        break;
      }
      spOff = Expr::MakeId(X86_Reg_Bp, pCpuInfo);
    }

  case 0x7:
    spOff = Expr::MakeId(X86_Reg_Bx, pCpuInfo); break;

  default:
    return nullptr;
  }

  // Mode
  switch (ModRm.Mod())
  {
  case 0x0:
    return spOff;

  case 0x1:
    {
      u8 Disp8;
      rBinStrm.Read(Offset + 1, Disp8);
      OprdLen += sizeof(Disp8);
      return Expr::MakeBinOp(OperationExpression::OpAdd, spOff, Expr::MakeBitVector(16, static_cast<u16>(SignExtend<s16, 8>(Disp8))));
      break;
    }
  case 0x2:
    {
      u16 Disp16;
      rBinStrm.Read(Offset + 1, Disp16);
      OprdLen += sizeof(Disp16);
      return Expr::MakeBinOp(OperationExpression::OpAdd, spOff, Expr::MakeBitVector(16, Disp16));
      break;
    }
  default:
    return nullptr;
  }
}

// ref: http://sandpile.org/x86/opc_sib.htm
static Expression::SPType __DecodeSib32(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, OffsetType Offset, Instruction const& rInsn, u8& rOprdLen)
{
  static u32 Reg[]          = { X86_Reg_Eax, X86_Reg_Ecx, X86_Reg_Edx,  X86_Reg_Ebx,  X86_Reg_Esp,     X86_Reg_Unknown, X86_Reg_Esi,  X86_Reg_Edi  };
  static u32 RegIndex[]     = { X86_Reg_Eax, X86_Reg_Ecx, X86_Reg_Edx,  X86_Reg_Ebx,  X86_Reg_Unknown, X86_Reg_Ebp,     X86_Reg_Esi,  X86_Reg_Edi  };
  static u32 RegRexB[]      = { X86_Reg_R8d, X86_Reg_R9d, X86_Reg_R10d, X86_Reg_R11d, X86_Reg_R12d,    X86_Reg_Unknown, X86_Reg_R14d, X86_Reg_R15d };
  static u32 RegIndexRexX[] = { X86_Reg_R8d, X86_Reg_R9d, X86_Reg_R10d, X86_Reg_R11d, X86_Reg_R12d,    X86_Reg_R13d,    X86_Reg_R14d, X86_Reg_R15d };

  static u32 aScale[]       = { 1, 2, 4, 8 };

  u32* pReg      = (rInsn.GetPrefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? RegRexB      : Reg;
  u32* pRegIndex = (rInsn.GetPrefix() & (X86_Prefix_REX_x & ~X86_Prefix_REX)) ? RegIndexRexX : RegIndex;
  u32  Reg101    = (rInsn.GetPrefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? X86_Reg_R13d  : X86_Reg_Ebp;

  auto const ModRm = __GetModRm(rBinStrm, Offset - 1);
  auto const Sib   = __GetSib(rBinStrm, Offset);

  Expression::SPType spReg, spDisp;

  if (Sib.Base() == 0x5)
  {
    switch (ModRm.Mod())
    {
    case 0x2:
      spReg = Expr::MakeId(Reg101, pCpuInfo);

    case 0x0:
      {
        u32 Disp32;
        rBinStrm.Read(Offset + sizeof(ModRm), Disp32);
        spDisp = Expr::MakeBitVector(32, Disp32);
        rOprdLen += sizeof(Disp32);
        break;
      }

    case 0x1:
      {
        spReg = Expr::MakeId(Reg101, pCpuInfo);

        u8 Disp8;
        rBinStrm.Read(Offset + sizeof(ModRm), Disp8);
        spDisp = Expr::MakeBitVector(32, static_cast<u32>(SignExtend<s32, 8>(Disp8)));
        rOprdLen += sizeof(Disp8);
        break;
      }

    default:
      return nullptr;
    }
  }
  else
  {
    spReg = Expr::MakeId(pReg[Sib.Base()], pCpuInfo);

    switch (ModRm.Mod())
    {
    case 0x0:
      break;

    case 0x1:
      {
        u8 Disp8;
        rBinStrm.Read(Offset + 1, Disp8);
        spDisp = Expr::MakeBitVector(32, static_cast<u32>(SignExtend<s32, 8>(Disp8)));
        rOprdLen += sizeof(Disp8);
        break;
      }

    case 0x2:
      {
        u32 Disp32;
        rBinStrm.Read(Offset + 1, Disp32);
        spDisp = Expr::MakeBitVector(32, Disp32);
        rOprdLen += sizeof(Disp32);
        break;
      }

    default:
      return nullptr;
    };
  }

  rOprdLen += sizeof(Sib);

  auto spSecReg = pRegIndex[Sib.Index()] ? Expr::MakeId(pRegIndex[Sib.Index()], pCpuInfo) : nullptr;
  auto spScale = Expr::MakeBitVector(32, aScale[Sib.Scale()]);

  if (spReg == nullptr && spSecReg == nullptr && spDisp != nullptr)
    return spDisp;

  auto const OpAdd = OperationExpression::OpAdd;
  auto const OpMul = OperationExpression::OpMul;

  Expression::SPType spOff;

  if (spReg != nullptr && spSecReg == nullptr)
    std::swap(spReg, spSecReg);

  auto spMul = Expr::MakeBinOp(OpMul, spSecReg, spScale);
  if (spReg == nullptr)
    spOff = spMul;
  else
    spOff = Expr::MakeBinOp(OpAdd, spReg, spMul);
  if (spDisp != nullptr)
    spOff = Expr::MakeBinOp(OpAdd, spOff, spDisp);
  return spOff;
}

static Expression::SPType __DecodeModRmAddress32(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, OffsetType Offset, Instruction const& rInsn, u8& rOprdLen)
{
  static u16 Reg32[]     = { X86_Reg_Eax, X86_Reg_Ecx, X86_Reg_Edx,  X86_Reg_Ebx,  X86_Reg_Unknown, X86_Reg_Ebp, X86_Reg_Esi,  X86_Reg_Edi  };
  static u16 Reg32RexB[] = { X86_Reg_R8d, X86_Reg_R9d, X86_Reg_R10d, X86_Reg_R11d, X86_Reg_Unknown, X86_Reg_Eip, X86_Reg_R14d, X86_Reg_R15d };

  auto const ModRm = __GetModRm(rBinStrm, Offset);

  // SIB
  if (ModRm.Mod() != 0x3 && ModRm.Rm() == 0x4)
    return __DecodeSib32(pCpuInfo, rBinStrm, Offset + sizeof(ModRm), rInsn, rOprdLen);

  // [Disp32]
  else if (ModRm.Mod() == 0x0 && ModRm.Rm() == 0x5)
  {
    u32 Disp32;
    rBinStrm.Read(Offset + sizeof(ModRm), Disp32);
    rOprdLen += sizeof(Disp32);
    return Expr::MakeBitVector(32, Disp32);
  }

  auto spReg = Expr::MakeId((rInsn.GetPrefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? Reg32RexB[ModRm.Rm()] : Reg32[ModRm.Rm()], pCpuInfo);
  Expression::SPType spDisp;

  // Mod
  switch (ModRm.Mod())
  {
  case 0x0:
    break;

  case 0x1:
    {
      u8 Disp8;
      rBinStrm.Read(Offset + sizeof(ModRm), Disp8);
      spDisp = Expr::MakeBitVector(32, static_cast<u32>(SignExtend<s32, 8>(Disp8)));
      rOprdLen += sizeof(Disp8);
      break;
    }
  case 0x2:
    {
      u32 Disp32;
      rBinStrm.Read(Offset + sizeof(ModRm), Disp32);
      spDisp = Expr::MakeBitVector(32, Disp32);
      rOprdLen += sizeof(Disp32);
      break;
    }
  default:
    return nullptr;
  }

  return spDisp == nullptr ? spReg : Expr::MakeBinOp(OperationExpression::OpAdd, spReg, spDisp);
}

static Expression::SPType __DecodeSib64(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8& rOprdLen)
{
  static u32 Reg[]          = { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx,  X86_Reg_Rbx,  X86_Reg_Rsp,     X86_Reg_Unknown, X86_Reg_Rsi, X86_Reg_Rdi };
  static u32 RegRexB[]      = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10,  X86_Reg_R11,  X86_Reg_R12,     X86_Reg_Unknown, X86_Reg_R14, X86_Reg_R15 };
  static u32 RegIndex[]     = { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx,  X86_Reg_Rbx,  X86_Reg_Unknown, X86_Reg_Rbp,     X86_Reg_Rsi, X86_Reg_Rdi };
  static u32 RegIndexRexX[] = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10,  X86_Reg_R11,  X86_Reg_R12,     X86_Reg_R13,     X86_Reg_R14, X86_Reg_R15 };

  static u32 aScale[]    = { 1, 2, 4, 8 };

  u32* pReg      = (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? RegRexB      : Reg;
  u32* pRegIndex = (rInsn.Prefix() & (X86_Prefix_REX_x & ~X86_Prefix_REX)) ? RegIndexRexX : RegIndex;
  u32  Reg101    = (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? X86_Reg_R13   : X86_Reg_Rbp;

  auto const ModRm = __GetModRm(rBinStrm, Offset - 1);
  auto const Sib   = __GetSib(rBinStrm, Offset);

  Expression::SPType spReg, spDisp;

  if (Sib.Base() == 0x5)
  {
    switch (ModRm.Mod())
    {
    case 0x2:
      spReg = Expr::MakeId(Reg101, pCpuInfo);

    case 0x0:
      {
        u32 Disp32;
        rBinStrm.Read(Offset + sizeof(ModRm), Disp32);
        spDisp = Expr::MakeBitVector(64, static_cast<u64>(SignExtend<s64, 32>(Disp32)));
        rOprdLen += sizeof(Disp32);
        break;
      }

    case 0x1:
      {
        spReg = Expr::MakeId(Reg101, pCpuInfo);

        u8 Disp8;
        rBinStrm.Read(Offset + sizeof(ModRm), Disp8);
        spDisp = Expr::MakeBitVector(64, static_cast<u64>(SignExtend<s64, 8>(Disp8)));
        rOprdLen += sizeof(Disp8);
        break;
      }

    default:
      return nullptr;
    }
  }
  else
  {
    spReg = Expr::MakeId(pReg[Sib.Base()], pCpuInfo);

    switch (ModRm.Mod())
    {
    case 0x0:
      break;

    case 0x1:
      {
        u8 Disp8;
        rBinStrm.Read(Offset + 1, Disp8);
        spDisp = Expr::MakeBitVector(64, static_cast<u64>(SignExtend<s64, 8>(Disp8)));
        rOprdLen += sizeof(Disp8);
        break;
      }

    case 0x2:
      {
        u32 Disp32;
        rBinStrm.Read(Offset + 1, Disp32);
        spDisp = Expr::MakeBitVector(64, static_cast<u64>(SignExtend<s64, 32>(Disp32)));
        rOprdLen += sizeof(Disp32);
        break;
      }

    default:
      return nullptr;
    };
  }

  rOprdLen += sizeof(Sib);

  auto spSecReg = pRegIndex[Sib.Index()] ? Expr::MakeId(pRegIndex[Sib.Index()], pCpuInfo) : nullptr;
  auto spScale = Expr::MakeBitVector(64, aScale[Sib.Scale()]);

  if (spReg == nullptr && spSecReg == nullptr && spDisp != nullptr)
    return spDisp;

  auto const OpAdd = OperationExpression::OpAdd;
  auto const OpMul = OperationExpression::OpMul;

  Expression::SPType spOff;

  if (spReg != nullptr && spSecReg == nullptr)
    std::swap(spReg, spSecReg);

  auto spMul = Expr::MakeBinOp(OpMul, spSecReg, spScale);
  if (spReg == nullptr)
    spOff = spMul;
  else
    spOff = Expr::MakeBinOp(OpAdd, spReg, spMul);
  if (spDisp != nullptr)
    spOff = Expr::MakeBinOp(OpAdd, spOff, spDisp);
  return spOff;
}

static Expression::SPType __DecodeModRmAddress64(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8& rOprdLen)
{
  static u16 Reg[]     = { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx, X86_Reg_Rbx, X86_Reg_Unknown, X86_Reg_Rbp, X86_Reg_Rsi, X86_Reg_Rdi };
  static u16 RegRexB[] = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10, X86_Reg_R11, X86_Reg_Unknown, X86_Reg_R13, X86_Reg_R14, X86_Reg_R15 };

  auto const ModRm = __GetModRm(rBinStrm, Offset);

  // SIB
  if (ModRm.Mod() != 0x3 && ModRm.Rm() == 0x4)
    return __DecodeSib64(pCpuInfo, rBinStrm, Offset + sizeof(ModRm), rInsn, rOprdLen);

  // [Disp32]
  // NOTE: [eip+Disp32] or [rip+Disp32] must be handled before
  else if (ModRm.Mod() == 0x0 && ModRm.Rm() == 0x5)
  {
    u32 Disp32;
    rBinStrm.Read(Offset + sizeof(ModRm), Disp32);
    rOprdLen += sizeof(Disp32);
    return Expr::MakeBitVector(64, Disp32);
  }

  auto spReg = Expr::MakeId((rInsn.GetPrefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? RegRexB[ModRm.Rm()] : Reg[ModRm.Rm()], pCpuInfo);
  Expression::SPType spDisp;

  // Mod
  switch (ModRm.Mod())
  {
  case 0x0:
    break;

  case 0x1:
    {
      u8 Disp8;
      rBinStrm.Read(Offset + sizeof(ModRm), Disp8);
      spDisp = Expr::MakeBitVector(64, static_cast<u64>(SignExtend<s64, 8>(Disp8)));
      rOprdLen += sizeof(Disp8);
      break;
    }
  case 0x2:
    {
      u32 Disp32;
      rBinStrm.Read(Offset + sizeof(ModRm), Disp32);
      spDisp = Expr::MakeBitVector(64, static_cast<u64>(SignExtend<s64, 32>(Disp32)));
      rOprdLen += sizeof(Disp32);
      break;
    }
  default:
    return nullptr;
  }

  return spDisp == nullptr ? spReg : Expr::MakeBinOp(OperationExpression::OpAdd, spReg, spDisp);
}

static Expression::SPType __DecodeModRmAddress(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8& rOprdLen, X86_Bit Bit)
{
  Expression::SPType spExpr;

  switch (Bit)
  {
  case X86_Bit_16:
    if (rInsn.Prefix() & X86_Prefix_AdSize)
      spExpr = __DecodeModRmAddress32(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);
    else
      spExpr = __DecodeModRmAddress16(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);
    break;

  case X86_Bit_32:
    if (rInsn.Prefix() & X86_Prefix_AdSize)
      spExpr = __DecodeModRmAddress16(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);
    else
      spExpr = __DecodeModRmAddress32(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);
    break;

  case X86_Bit_64:
    // Handle [eip+Disp32] or [rip+Disp32] (x86_64 only)
    {
      auto const ModRm = __GetModRm(rBinStrm, Offset);
      if (ModRm.Mod() == 0x00 && ModRm.Rm() == 0x05)
      {
        bool Is64 = rInsn.GetPrefix() & X86_Prefix_AdSize ? false : true;
        u32 Reg = Is64 ? X86_Reg_Rip : X86_Reg_Eip;
        u32 Disp32;
        if (!rBinStrm.Read(Offset + sizeof(ModRm), Disp32))
          return nullptr;

        rOprdLen += 4;
        if (Is64)
          spExpr = Expr::MakeBinOp(OperationExpression::OpAdd,
          /**/Expr::MakeId(Reg, pCpuInfo),
          /**/Expr::MakeBitVector(64, SignExtend<s64, 32>(Disp32)));
        else
          spExpr = Expr::MakeBinOp(OperationExpression::OpAdd,
          /**/Expr::MakeId(Reg, pCpuInfo),
          /**/Expr::MakeBitVector(32, Disp32));
        break;
      }
    }

    if (rInsn.Prefix() & X86_Prefix_AdSize)
      spExpr = __DecodeModRmAddress32(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);
    else
      spExpr = __DecodeModRmAddress64(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);
    break;

  default:
    return nullptr;
  };

  return spExpr;
}

Expression::SPType X86Architecture::__Decode_Ap(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto Ap16 = [&](void) -> Expression::SPType
  {
    u16 Seg;
    u16 Off;

    if (!rBinStrm.Read(Offset, Off))
      return nullptr;
    if (!rBinStrm.Read(Offset, Seg))
      return nullptr;

    auto spConstSeg = Expr::MakeBitVector(16, Seg);
    auto spConstOff = Expr::MakeBitVector(16, Off);
    return Expr::MakeMem(16, spConstSeg, spConstOff, false);
  };

  auto Ap32 = [&](void) -> Expression::SPType
  {
    u16 Seg;
    u32 Off;

    if (!rBinStrm.Read(Offset, Off))
      return nullptr;
    if (!rBinStrm.Read(Offset, Seg))
      return nullptr;

    auto spConstSeg = Expr::MakeBitVector(16, Seg);
    auto spConstOff = Expr::MakeBitVector(16, Off);
    return Expr::MakeMem(16, spConstSeg, spConstOff, false);
  };

  switch (Mode)
  {
  case X86_Bit_16:
    if (rInsn.GetPrefix() & X86_Prefix_AdSize)
      return Ap32();
    else
      return Ap16();

  case X86_Bit_32:
    if (rInsn.GetPrefix() & X86_Prefix_AdSize)
      return Ap16();
    else
      return Ap32();

  default:
    return nullptr;
  }
}

Expression::SPType X86Architecture::__Decode_By(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Cy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Dy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Eb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(8, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), (rInsn.GetPrefix() & X86_Prefix_REX) ? s_GP8Rex : s_GP8);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Ed(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(32, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_GP32);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Edb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Eq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(64, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_GP64);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Ev(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  switch (__GetOperandSize(rInsn, Mode))
  {
  case 16:
    return __Decode_Ew(rBinStrm, Offset, rInsn, Mode);

  case 32:
    return __Decode_Ed(rBinStrm, Offset, rInsn, Mode);

  case 64:
    return __Decode_Eq(rBinStrm, Offset, rInsn, Mode);

  default:
    assert(0 && "Invalid mode");
    return nullptr;
  }
}

Expression::SPType X86Architecture::__Decode_Ew(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(16, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_GP16);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Ey(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  switch (__GetOperandSize(rInsn, Mode))
  {
  case 16:
  case 32:
    return __Decode_Ed(rBinStrm, Offset, rInsn, Mode);

  case 64:
    return __Decode_Eq(rBinStrm, Offset, rInsn, Mode);

  default:
    assert(0 && "Invalid mode");
    return nullptr;
  }
}

Expression::SPType X86Architecture::__Decode_Eyb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Eyw(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Gb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), rInsn.GetPrefix() & X86_Prefix_REX ? s_GP8Rex : s_GP8);
}

Expression::SPType X86Architecture::__Decode_Gw(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_GP16);
}

Expression::SPType X86Architecture::__Decode_Gd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_GP32);
}

Expression::SPType X86Architecture::__Decode_Gq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_GP64);
}

Expression::SPType X86Architecture::__Decode_Gv(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  switch (__GetOperandSize(rInsn, Mode))
  {
  case 16:
    return __Decode_Gw(rBinStrm, Offset, rInsn, Mode);

  case 32:
    return __Decode_Gd(rBinStrm, Offset, rInsn, Mode);

  case 64:
    return __Decode_Gq(rBinStrm, Offset, rInsn, Mode);

  default:
    assert(0 && "Invalid mode");
    return nullptr;
  }
}

Expression::SPType X86Architecture::__Decode_Gy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  /* FIXME: */
  return __Decode_Gv(rBinStrm, Offset, rInsn, Mode);
}

Expression::SPType X86Architecture::__Decode_Ho(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_MMX);
}

Expression::SPType X86Architecture::__Decode_Hx(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Hy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  switch (__GetOperandSize(rInsn, Mode))
  {
  case 32:
    return __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_GP32);
  case 64:
    return __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_GP64);
  default:
    return nullptr;
  }
}

// TODO: handle sign extension
Expression::SPType X86Architecture::__Decode_Ibs(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  u8 Value;

  if (!rBinStrm.Read(Offset, Value))
    return nullptr;
  ++rInsn.Size();

  u8 BitSize = 8;
  auto spOprd0 = rInsn.GetOperand(0);
  if (spOprd0 != nullptr)
    BitSize = spOprd0->GetBitSize();

  return Expr::MakeBitVector(BitSize, SignExtend<s64, 8>(Value));
}

Expression::SPType X86Architecture::__Decode_Jb(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto RegPc = m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, Mode);
  if (RegPc == 0)
    return nullptr;
  auto RegPcSize = m_CpuInfo.GetSizeOfRegisterInBit(RegPc);
  if (RegPcSize == 0)
    return nullptr;

  u8 Value;
  if (!rBinStrm.Read(Offset, Value))
    return nullptr;
  rInsn.Size() += sizeof(Value);

  return Expr::MakeBinOp(OperationExpression::OpAdd,
    Expr::MakeId(RegPc, &m_CpuInfo),
    Expr::MakeBitVector(RegPcSize, SignExtend<s64, 8>(Value)));
}

Expression::SPType X86Architecture::__Decode_Jz(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto RegPc = m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, Mode);
  if (RegPc == 0)
    return nullptr;
  auto RegPcSize = m_CpuInfo.GetSizeOfRegisterInBit(RegPc);
  if (RegPcSize == 0)
    return nullptr;

  switch (__GetOperandSize(rInsn, Mode))
  {
  case 16:
    {
      u16 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      rInsn.Size() += sizeof(Value);

      return Expr::MakeBinOp(OperationExpression::OpAdd,
        Expr::MakeId(RegPc, &m_CpuInfo),
        Expr::MakeBitVector(RegPcSize, SignExtend<s64, 16>(Value)));
    }

  case 32: case 64:
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      rInsn.Size() += sizeof(Value);

      return Expr::MakeBinOp(OperationExpression::OpAdd,
        Expr::MakeId(RegPc, &m_CpuInfo),
        Expr::MakeBitVector(RegPcSize, SignExtend<s64, 32>(Value)));
    }

  default:
    return nullptr;
  }
}

Expression::SPType X86Architecture::__Decode_Lx(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_M(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  auto AccessSize = __GetOperandSize(rInsn, Mode);
  if (AccessSize == 0)
    return nullptr;

  rInsn.Size() += OprdLen;

  return Expr::MakeMem(AccessSize, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

Expression::SPType X86Architecture::__Decode_Ma(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  auto AccessSize = __GetOperandSize(rInsn, Mode);
  if (AccessSize == 0)
    return nullptr;

  rInsn.Size() += OprdLen;

  // a → z:z
  return Expr::MakeMem(AccessSize * 2, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

Expression::SPType X86Architecture::__Decode_Md(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  rInsn.Size() += OprdLen;

  return Expr::MakeMem(32, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

Expression::SPType X86Architecture::__Decode_Mo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  rInsn.Size() += OprdLen;

  return Expr::MakeMem(128, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

Expression::SPType X86Architecture::__Decode_Mod(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  rInsn.Size() += OprdLen;

  return Expr::MakeMem(32, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

Expression::SPType X86Architecture::__Decode_Moq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  rInsn.Size() += OprdLen;

  return Expr::MakeMem(64, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

Expression::SPType X86Architecture::__Decode_Mp(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  auto AccessSize = __GetOperandSize(rInsn, Mode);
  if (AccessSize == 0)
    return nullptr;

  rInsn.Size() += OprdLen;

  // p → w:v
  return Expr::MakeMem(16 + AccessSize, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

Expression::SPType X86Architecture::__Decode_Mq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  rInsn.Size() += OprdLen;

  return Expr::MakeMem(64, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

//http://www.officedaytime.com/tips/simdimg/si.php?f=vpgatherdq
Expression::SPType X86Architecture::__Decode_Mqo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Mv(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  auto AccessSize = __GetOperandSize(rInsn, Mode);
  if (AccessSize == 0)
    return nullptr;

  rInsn.Size() += OprdLen;

  return Expr::MakeMem(AccessSize, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

Expression::SPType X86Architecture::__Decode_Mw(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  rInsn.Size() += OprdLen;

  return Expr::MakeMem(16, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

Expression::SPType X86Architecture::__Decode_Mx(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_My(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  ++rInsn.Size();
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;

  auto AccessSize = __GetOperandSize(rInsn, Mode);
  if (AccessSize == 0)
    return nullptr;
  // TODO:
  if (AccessSize != 32 && AccessSize != 64)
    AccessSize = 32;

  rInsn.Size() += OprdLen;

  return Expr::MakeMem(AccessSize, __GetSegmentRegister(&m_CpuInfo, rInsn), spOprd);
}

Expression::SPType X86Architecture::__Decode_Nq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  if (ModRm.Mod() != 0x03)
    return nullptr;
  return __Decode_Qq(rBinStrm, Offset, rInsn, Mode);
}

Expression::SPType X86Architecture::__Decode_Ob(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOff;
  switch (__GetAddressSize(rInsn, Mode))
  {
  case 16:
    {
      u16 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      spOff = Expr::MakeBitVector(16, Value);
      rInsn.Size() += sizeof(Value);
      break;
    }

  case 32:
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      spOff = Expr::MakeBitVector(32, Value);
      rInsn.Size() += sizeof(Value);
      break;
    }

  case 64:
    {
      u64 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      spOff = Expr::MakeBitVector(64, Value);
      rInsn.Size() += sizeof(Value);
      break;
    }

  default:
    return nullptr;
  }

  return Expr::MakeMem(8, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
}

Expression::SPType X86Architecture::__Decode_Ov(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOff;
  switch (__GetAddressSize(rInsn, Mode))
  {
  case 16:
    {
      u16 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      spOff = Expr::MakeBitVector(16, Value);
      rInsn.Size() += sizeof(Value);
      break;
    }

  case 32:
    {
      u32 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      spOff = Expr::MakeBitVector(32, Value);
      rInsn.Size() += sizeof(Value);
      break;
    }

  case 64:
    {
      u64 Value;
      if (!rBinStrm.Read(Offset, Value))
        return nullptr;
      spOff = Expr::MakeBitVector(64, Value);
      rInsn.Size() += sizeof(Value);
      break;
    }

  default:
    return nullptr;
  }

  auto AccessSize = __GetOperandSize(rInsn, Mode);
  if (AccessSize == 0)
    return nullptr;

  return Expr::MakeMem(AccessSize, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
}

Expression::SPType X86Architecture::__Decode_Pq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_MMX);
}

Expression::SPType X86Architecture::__Decode_Qd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(32, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_MMX);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Qq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(64, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_MMX);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Rv(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  if (ModRm.Mod() != 0x03)
    return nullptr;
  return __Decode_Ev(rBinStrm, Offset, rInsn, Mode);
}

Expression::SPType X86Architecture::__Decode_Ry(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  if (ModRm.Mod() != 0x03)
    return nullptr;
  return __Decode_Ey(rBinStrm, Offset, rInsn, Mode);
}

Expression::SPType X86Architecture::__Decode_Sw(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_SEG);
}

Expression::SPType X86Architecture::__Decode_Ty(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_TST);
}

Expression::SPType X86Architecture::__Decode_Uo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(128, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Uod(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(32, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_MMX);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Uoq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
  {
    auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
    if (spOff == nullptr)
      return nullptr;
    spOprd = Expr::MakeMem(64, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
    break;
  }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_MMX);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Uq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(64, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_MMX);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Ux(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  if (ModRm.Mod() != 0x03)
    return nullptr;
  return __Decode_Wx(rBinStrm, Offset, rInsn, Mode);
}

Expression::SPType X86Architecture::__Decode_Vo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_XMM);
}

Expression::SPType X86Architecture::__Decode_Vod(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_XMM);
}

Expression::SPType X86Architecture::__Decode_Voq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_XMM);
}

Expression::SPType X86Architecture::__Decode_Vx(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  /* TODO: Handle YMM register here */
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_XMM);
}

Expression::SPType X86Architecture::__Decode_Vy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_XMM);
}

Expression::SPType X86Architecture::__Decode_Wd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, Offset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_XMM);
}

Expression::SPType X86Architecture::__Decode_Wo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(128, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Wob(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(8, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Wod(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(32, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

// NOTE: What's the different between Wo.o and Wo?
Expression::SPType X86Architecture::__Decode_Woo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(128, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Woq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(64, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Woqo(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Wow(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(16, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Wq(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Wx(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  ++rInsn.Size();

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, Offset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(128, __GetSegmentRegister(&m_CpuInfo, rInsn), spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Size() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Wy(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_b(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m16int(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m32fp(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m32int(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m64fp(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m64int(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m80bcd(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m80dec(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m80fp(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_w(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_x(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}