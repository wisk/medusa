#include "x86_architecture.hpp"
#include <medusa/extend.hpp>

// ref: http://wiki.osdev.org/X86-64_Instruction_Encoding

static Expression::SPType __GetRegisterFromIndex(CpuInformation const* pCpuInfo, u8 RegIdx, u32 RegList[16])
{
  if (RegIdx >= sizeof(RegList))
    return nullptr;
  auto Reg = RegList[RegIdx];
  if (Reg == X86_Reg_Unknown)
    return nullptr;
  return Expr::MakeId(Reg, pCpuInfo);
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

static u32 s_GP8[16]    = { X86_Reg_Al,   X86_Reg_Cl,   X86_Reg_Dl,   X86_Reg_Bl,   X86_Reg_Ah,   X86_Reg_Ch,   X86_Reg_Dh,      X86_Reg_Bh,      X86_Reg_R8b,     X86_Reg_R9b,     X86_Reg_R10b,    X86_Reg_R11b,    X86_Reg_R12b,    X86_Reg_R13b,    X86_Reg_R14b,    X86_Reg_R15b    };
static u32 s_GP8Rex[16] = { X86_Reg_Al,   X86_Reg_Cl,   X86_Reg_Dl,   X86_Reg_Bl,   X86_Reg_Spl,  X86_Reg_Bpl,  X86_Reg_Sil,     X86_Reg_Dil,     X86_Reg_R8b,     X86_Reg_R9b,     X86_Reg_R10b,    X86_Reg_R11b,    X86_Reg_R12b,    X86_Reg_R13b,    X86_Reg_R14b,    X86_Reg_R15b    };
static u32 s_GP16[16]   = { X86_Reg_Ax,   X86_Reg_Cx,   X86_Reg_Dx,   X86_Reg_Bx,   X86_Reg_Sp,   X86_Reg_Bp,   X86_Reg_Si,      X86_Reg_Di,      X86_Reg_R8w,     X86_Reg_R9w,     X86_Reg_R10w,    X86_Reg_R11w,    X86_Reg_R12w,    X86_Reg_R13w,    X86_Reg_R14w,    X86_Reg_R15w    };
static u32 s_GP32[16]   = { X86_Reg_Eax,  X86_Reg_Ecx,  X86_Reg_Edx,  X86_Reg_Ebx,  X86_Reg_Esp,  X86_Reg_Ebp,  X86_Reg_Esi,     X86_Reg_Edi,     X86_Reg_R8d,     X86_Reg_R9d,     X86_Reg_R10d,    X86_Reg_R11d,    X86_Reg_R12d,    X86_Reg_R13d,    X86_Reg_R14d,    X86_Reg_R15d    };
static u32 s_GP64[16]   = { X86_Reg_Rax,  X86_Reg_Rcx,  X86_Reg_Rdx,  X86_Reg_Rbx,  X86_Reg_Rsp,  X86_Reg_Rbp,  X86_Reg_Rsi,     X86_Reg_Rdi,     X86_Reg_R8,      X86_Reg_R9,      X86_Reg_R10,     X86_Reg_R11,     X86_Reg_R12,     X86_Reg_R13,     X86_Reg_R14,     X86_Reg_R15     };
static u32 s_x87[16]    = { X86_Reg_St0,  X86_Reg_St1,  X86_Reg_St2,  X86_Reg_St3,  X86_Reg_St4,  X86_Reg_St5,  X86_Reg_St6,     X86_Reg_St7,     X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Unknown };
static u32 s_MMX[16]    = { X86_Reg_Mm0,  X86_Reg_Mm1,  X86_Reg_Mm2,  X86_Reg_Mm3,  X86_Reg_Mm4,  X86_Reg_Mm5,  X86_Reg_Mm6,     X86_Reg_Mm7,     X86_Reg_Mm0,     X86_Reg_Mm1,     X86_Reg_Mm2,     X86_Reg_Mm3,     X86_Reg_Mm4,     X86_Reg_Mm5,     X86_Reg_Mm6,     X86_Reg_Mm7     };
static u32 s_XMM[16]    = { X86_Reg_Xmm0, X86_Reg_Xmm1, X86_Reg_Xmm2, X86_Reg_Xmm3, X86_Reg_Xmm4, X86_Reg_Xmm5, X86_Reg_Xmm6,    X86_Reg_Xmm7,    X86_Reg_Xmm8,    X86_Reg_Xmm9,    X86_Reg_Xmm10,   X86_Reg_Xmm11,   X86_Reg_Xmm12,   X86_Reg_Xmm13,   X86_Reg_Xmm14,   X86_Reg_Xmm15   };
//static u32 s_YMM[16]    = { X86_Reg_Ymm0, X86_Reg_Ymm1, X86_Reg_Ymm2, X86_Reg_Ymm3, X86_Reg_Ymm4, X86_Reg_Ymm5, X86_Reg_Ymm6,    X86_Reg_Ymm7,    X86_Reg_Ymm8,    X86_Reg_Ymm9,    X86_Reg_Ymm10,   X86_Reg_Ymm11,   X86_Reg_Ymm12,   X86_Reg_Ymm13,   X86_Reg_Ymm14,   X86_Reg_Ymm15   };
static u32 s_SEG[16]    = { X86_Reg_Es,   X86_Reg_Cs,   X86_Reg_Ss,   X86_Reg_Ds,   X86_Reg_Fs,   X86_Reg_Gs,   X86_Reg_Unknown, X86_Reg_Unknown, X86_Reg_Es,      X86_Reg_Cs,      X86_Reg_Ss,      X86_Reg_Ds,      X86_Reg_Fs,      X86_Reg_Gs,      X86_Reg_Unknown, X86_Reg_Unknown };
static u32 s_CTR[16]    = { X86_Reg_Cr0,  X86_Reg_Cr1,  X86_Reg_Cr2,  X86_Reg_Cr3,  X86_Reg_Cr4,  X86_Reg_Cr5,  X86_Reg_Cr6,     X86_Reg_Cr7,     X86_Reg_Cr8,     X86_Reg_Cr9,     X86_Reg_Cr10,    X86_Reg_Cr11,    X86_Reg_Cr12,    X86_Reg_Cr13,    X86_Reg_Cr14,    X86_Reg_Cr15    };
static u32 s_DBG[16]    = { X86_Reg_Dr0,  X86_Reg_Dr1,  X86_Reg_Dr2,  X86_Reg_Dr3,  X86_Reg_Dr4,  X86_Reg_Dr5,  X86_Reg_Dr6,     X86_Reg_Dr7,     X86_Reg_Dr8,     X86_Reg_Dr9,     X86_Reg_Dr10,    X86_Reg_Dr11,    X86_Reg_Dr12,    X86_Reg_Dr13,    X86_Reg_Dr14,    X86_Reg_Dr15    };

static x86::ModRM __GetModRm(BinaryStream const& rBinStrm, TOffset Offset)
{
  u8 ModRmByte;
  rBinStrm.Read(Offset, ModRmByte);
  return ModRmByte;
}

static x86::Sib __GetSib(BinaryStream const& rBinStrm, TOffset Offset)
{
  u8 SibByte;
  rBinStrm.Read(Offset, SibByte);
  return SibByte;
}

//ref: http://sandpile.org/x86/opc_rm16.htm
static Expression::SPType __DecodeModRmAddress16(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, TOffset Offset, Instruction const& rInsn, u8& OprdLen)
{
  OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, Offset);

  // Mem
  Expression::SPType spOff;
  switch (ModRm.Rm())
  {
  case 0x0:
    spOff = Expr::MakeOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Bx, pCpuInfo), Expr::MakeId(X86_Reg_Si, pCpuInfo));
    break;
  case 0x1:
    spOff = Expr::MakeOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Bx, pCpuInfo), Expr::MakeId(X86_Reg_Di, pCpuInfo));
    break;
  case 0x2:
    spOff = Expr::MakeOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Bp, pCpuInfo), Expr::MakeId(X86_Reg_Si, pCpuInfo));
    break;
  case 0x3:
    spOff = Expr::MakeOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Bp, pCpuInfo), Expr::MakeId(X86_Reg_Di, pCpuInfo));
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
        spOff = Expr::MakeConst(16, Disp16);
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
      return Expr::MakeOp(OperationExpression::OpAdd, spOff, Expr::MakeConst(8, Disp8));
      break;
    }
  case 0x2:
    {
      u16 Disp16;
      rBinStrm.Read(Offset + 1, Disp16);
      OprdLen += sizeof(Disp16);
      return Expr::MakeOp(OperationExpression::OpAdd, spOff, Expr::MakeConst(8, Disp16));
      break;
    }
  default:
    return nullptr;
  }
}

// ref: http://sandpile.org/x86/opc_sib.htm
static Expression::SPType __DecodeSib32(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, TOffset Offset, Instruction const& rInsn, u8& rOprdLen)
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
          spDisp = Expr::MakeConst(32, Disp32);
          rOprdLen += sizeof(Disp32);
          break;
        }

      case 0x1:
        {
          spReg = Expr::MakeId(Reg101, pCpuInfo);

          u8 Disp8;
          rBinStrm.Read(Offset + sizeof(ModRm), Disp8);
          spDisp = Expr::MakeConst(8, Disp8);
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
      case 0x2:
        {
          u32 Disp32;
          rBinStrm.Read(Offset + 1, Disp32);
          spDisp = Expr::MakeConst(32, Disp32);
          rOprdLen += sizeof(Disp32);
        }

      case 0x1:
        {
          u8 Disp8;
          rBinStrm.Read(Offset + 1, Disp8);
          spDisp = Expr::MakeConst(8, Disp8);
          rOprdLen += sizeof(Disp8);
          break;
        }

      default:
        return nullptr;
    };
  }

  auto spSecReg = Expr::MakeId(pRegIndex[Sib.Index()], pCpuInfo);
  auto spScale = Expr::MakeConst(32, aScale[Sib.Scale()]);
  rOprdLen += sizeof(Sib);

  if (spReg == nullptr && spDisp != nullptr)
    return spDisp;

  auto const OpAdd = OperationExpression::OpAdd;
  auto const OpMul = OperationExpression::OpMul;

  auto spOff = Expr::MakeOp(OpAdd,
    spReg,
    Expr::MakeOp(OpMul, spSecReg, spScale));
  if (spDisp != nullptr)
    spOff = Expr::MakeOp(OpAdd, spOff, spDisp);
  return spOff;
}

static Expression::SPType __DecodeModRmAddress32(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, TOffset Offset, Instruction const& rInsn, u8& rOprdLen)
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
    return Expr::MakeConst(32, Disp32);
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
      spDisp = Expr::MakeConst(8, Disp8);
      rOprdLen += sizeof(Disp8);
      break;
    }
  case 0x2:
    {
      u32 Disp32;
      rBinStrm.Read(Offset + sizeof(ModRm), Disp32);
      spDisp = Expr::MakeConst(32, Disp32);
      rOprdLen += sizeof(Disp32);
      break;
    }
  default:
    return nullptr;
  }

  return spDisp == nullptr ? spReg : Expr::MakeOp(OperationExpression::OpAdd, spReg, spDisp);
}

static Expression::SPType __DecodeSib64(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8& rOprdLen)
{
  static u32 Reg[]          = { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx,  X86_Reg_Rbx,  X86_Reg_Rsp,     X86_Reg_Unknown, X86_Reg_Rsi, X86_Reg_Rdi  };
  static u32 RegRexB[]      = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10,  X86_Reg_R11,  X86_Reg_R12,     X86_Reg_Unknown, X86_Reg_R14, X86_Reg_R15d };
  static u32 RegIndex[]     = { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx,  X86_Reg_Rbx,  X86_Reg_Unknown, X86_Reg_Rbp,     X86_Reg_Rsi, X86_Reg_Rdi  };
  static u32 RegIndexRexX[] = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10,  X86_Reg_R11,  X86_Reg_R12,     X86_Reg_R13,     X86_Reg_R14, X86_Reg_R15d };

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
          spDisp = Expr::MakeConst(32, Disp32);
          rOprdLen += sizeof(Disp32);
          break;
        }

      case 0x1:
        {
          spReg = Expr::MakeId(Reg101, pCpuInfo);

          u8 Disp8;
          rBinStrm.Read(Offset + sizeof(ModRm), Disp8);
          spDisp = Expr::MakeConst(8, Disp8);
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
      case 0x2:
        {
          u32 Disp32;
          rBinStrm.Read(Offset + 1, Disp32);
          spDisp = Expr::MakeConst(32, Disp32);
          rOprdLen += sizeof(Disp32);
        }

      case 0x1:
        {
          u8 Disp8;
          rBinStrm.Read(Offset + 1, Disp8);
          spDisp = Expr::MakeConst(8, Disp8);
          rOprdLen += sizeof(Disp8);
          break;
        }

      default:
        return nullptr;
    };
  }

  if (spReg == nullptr && spDisp != nullptr)
    return spDisp;

  auto spSecReg = Expr::MakeId(pRegIndex[Sib.Index()], pCpuInfo);
  auto spScale = Expr::MakeConst(32, aScale[Sib.Scale()]);
  rOprdLen += sizeof(Sib);

  auto const OpAdd = OperationExpression::OpAdd;
  auto const OpMul = OperationExpression::OpMul;
  auto spOff = Expr::MakeOp(OpAdd,
    spReg,
    Expr::MakeOp(OpMul, spSecReg, spScale));
  if (spDisp != nullptr)
    spOff = Expr::MakeOp(OpAdd, spOff, spDisp);
  return spOff;
}

static Expression::SPType __DecodeModRmAddress64(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8& rOprdLen)
{
  static u16 Reg[]     = { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx, X86_Reg_Rbx, X86_Reg_Unknown, X86_Reg_Rbp, X86_Reg_Rsi, X86_Reg_Rdi };
  static u16 RegRexB[] = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10, X86_Reg_R11, X86_Reg_Unknown, X86_Reg_Eip, X86_Reg_R14, X86_Reg_R15 };

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
    return Expr::MakeConst(32, Disp32);
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
      spDisp = Expr::MakeConst(8, Disp8);
      rOprdLen += sizeof(Disp8);
      break;
    }
  case 0x2:
    {
      u32 Disp32;
      rBinStrm.Read(Offset + sizeof(ModRm), Disp32);
      spDisp = Expr::MakeConst(32, Disp32);
      rOprdLen += sizeof(Disp32);
      break;
    }
  default:
    return nullptr;
  }

  return spDisp == nullptr ? spReg : Expr::MakeOp(OperationExpression::OpAdd, spReg, spDisp);
}

static Expression::SPType __DecodeModRmAddress(CpuInformation* pCpuInfo, BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8& rOprdLen, X86_Bit Bit)
{
  switch (Bit)
  {
  case X86_Bit_16:
    if (rInsn.Prefix() & X86_Prefix_AdSize)
      return __DecodeModRmAddress32(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);
    else
      return __DecodeModRmAddress16(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);

  case X86_Bit_32:
    if (rInsn.Prefix() & X86_Prefix_AdSize)
      return __DecodeModRmAddress16(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);
    else
      return __DecodeModRmAddress32(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);

  case X86_Bit_64:
    // Handle [eip+Disp32] or [rip+Disp32] (x86_64 only)
    {
      auto const ModRm = __GetModRm(rBinStrm, Offset);
      if (ModRm.Mod() == 0x00 && ModRm.Rm() == 0x05)
      {
        u32 Reg = rInsn.GetPrefix() & X86_Prefix_AdSize ? X86_Reg_Eip : X86_Reg_Rip;
        u32 Disp32;
        if (!rBinStrm.Read(Offset + sizeof(ModRm), Disp32))
          return nullptr;

        rOprdLen += 4;

        return Expr::MakeOp(OperationExpression::OpAdd,
          Expr::MakeId(Reg, pCpuInfo),
          Expr::MakeConst(32, Disp32));
      }
    }

    if (rInsn.Prefix() & X86_Prefix_AdSize)
      return __DecodeModRmAddress32(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);
    else
      return __DecodeModRmAddress64(pCpuInfo, rBinStrm, Offset, rInsn, rOprdLen);

  default:
    return nullptr;
  };
}

Expression::SPType X86Architecture::__Decode_Ap(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  auto Ap16 = [&](void) -> Expression::SPType
  {
    u16 Seg;
    u16 Off;

    if (!rBinStrm.Read(rOffset, Off))
      return nullptr;
    if (!rBinStrm.Read(rOffset, Seg))
      return nullptr;

    auto spConstSeg = Expr::MakeConst(16, Seg);
    auto spConstOff = Expr::MakeConst(16, Off);
    return Expr::MakeMem(16, spConstSeg, spConstOff, false);
  };

  auto Ap32 = [&](void) -> Expression::SPType
  {
    u16 Seg;
    u32 Off;

    if (!rBinStrm.Read(rOffset, Off))
      return nullptr;
    if (!rBinStrm.Read(rOffset, Seg))
      return nullptr;

    auto spConstSeg = Expr::MakeConst(16, Seg);
    auto spConstOff = Expr::MakeConst(16, Off);
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

Expression::SPType X86Architecture::__Decode_By(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Cy(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Dy(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Eb(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(8, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), (rInsn.GetPrefix() & X86_Prefix_REX) ? s_GP8Rex : s_GP8);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Ed(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(32, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_GP32);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Edb(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Eq(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(64, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_GP64);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Ev(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  switch (__GetOperandSize(rInsn, Mode))
  {
  case 16:
    return __Decode_Ew(rBinStrm, rOffset, rInsn, Mode);

  case 32:
    return __Decode_Ed(rBinStrm, rOffset, rInsn, Mode);

  case 64:
    return __Decode_Eq(rBinStrm, rOffset, rInsn, Mode);

  default:
    assert(0 && "Invalid mode");
    return nullptr;
  }
}

Expression::SPType X86Architecture::__Decode_Ew(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(16, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_GP16);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Ey(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  if (Mode == X86_Bit_16)
    return nullptr;
  return __Decode_Ev(rBinStrm, rOffset, rInsn, Mode);
}

Expression::SPType X86Architecture::__Decode_Eyb(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Eyw(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Fv(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Gb(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, rOffset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), rInsn.GetPrefix() & X86_Prefix_REX ? s_GP8Rex : s_GP8);
}

Expression::SPType X86Architecture::__Decode_Gw(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, rOffset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_GP16);
}

Expression::SPType X86Architecture::__Decode_Gd(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, rOffset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_GP32);
}

Expression::SPType X86Architecture::__Decode_Gq(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, rOffset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_GP64);
}

Expression::SPType X86Architecture::__Decode_Gv(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  switch (__GetOperandSize(rInsn, Mode))
  {
  case 16:
    return __Decode_Gw(rBinStrm, rOffset, rInsn, Mode);

  case 32:
    return __Decode_Gd(rBinStrm, rOffset, rInsn, Mode);

  case 64:
    return __Decode_Gq(rBinStrm, rOffset, rInsn, Mode);

  default:
    assert(0 && "Invalid mode");
    return nullptr;
  }
}

Expression::SPType X86Architecture::__Decode_Gy(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  /* FIXME: */
  return __Decode_Gv(rBinStrm, rOffset, rInsn, Mode);
}

Expression::SPType X86Architecture::__Decode_Ho(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Hx(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Hy(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Ibs(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Lx(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_M(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  u8 OprdLen = 0;
  Expression::SPType spOprd = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
  if (spOprd == nullptr)
    return nullptr;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Ma(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Md(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Mo(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Mod(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Moq(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Mp(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Mq(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Mqo(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Mv(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Mw(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Mx(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_My(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Nq(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Ob(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Ov(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Pq(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, rOffset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_MMX);
}

Expression::SPType X86Architecture::__Decode_Qd(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Qq(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(64, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_MMX);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Rv(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Ry(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Sw(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Ty(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Uo(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Uod(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Uq(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Ux(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Vo(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  auto const ModRm = __GetModRm(rBinStrm, rOffset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_XMM);
}

Expression::SPType X86Architecture::__Decode_Vod(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Vx(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  /* TODO: Handle YMM register here */
  auto const ModRm = __GetModRm(rBinStrm, rOffset);
  return __GetRegisterFromIndex(&m_CpuInfo, __ModRmRegister(rInsn, ModRm), s_XMM);
}

Expression::SPType X86Architecture::__Decode_Vy(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Wd(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Wo(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(128, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Wob(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(8, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Wod(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(32, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

// NOTE: What's the different between Wo.o and Wo?
Expression::SPType X86Architecture::__Decode_Woo(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(128, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Woq(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(64, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Woqo(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Wow(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  Expression::SPType spOprd;
  u8 OprdLen = 0;
  auto const ModRm = __GetModRm(rBinStrm, rOffset);

  switch (ModRm.Mod())
  {
  case 0: case 1: case 2:
    {
      auto spOff = __DecodeModRmAddress(&m_CpuInfo, rBinStrm, rOffset, rInsn, OprdLen, static_cast<X86_Bit>(Mode));
      if (spOff == nullptr)
        return nullptr;
      spOprd = Expr::MakeMem(16, nullptr, spOff);
      break;
    }

  case 3:
    spOprd = __GetRegisterFromIndex(&m_CpuInfo, __ModRmMemoryRegister(rInsn, ModRm), s_XMM);
    break;
  }

  rInsn.Length() += OprdLen;
  return spOprd;
}

Expression::SPType X86Architecture::__Decode_Wq(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Wx(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_Wy(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_b(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m16int(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m32fp(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m32int(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m64fp(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m64int(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m80bcd(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m80dec(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_m80fp(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_w(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}

Expression::SPType X86Architecture::__Decode_x(BinaryStream const& rBinStrm, TOffset& rOffset, Instruction& rInsn, u8 Mode)
{
  return nullptr; /* TODO */
}