#include "x86.hpp"
#include "x86_architecture.hpp"

static x86::ModRM GetModRm(BinaryStream const& rBinStrm, TOffset Offset)
{
  u8 ModRmByte;
  rBinStrm.Read(Offset, ModRmByte);
  return ModRmByte;
}

/* Addressing (R/M) */

static bool DecodeModRmAddress16(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;
  rBinStrm.Read(Offset, ModRm);

  // Mem
  switch (ModRm & 0x7)
  {
  case 0x0: pOprd->Reg() = X86_Reg_Bx; pOprd->SecReg() = X86_Reg_Si; pOprd->Type() |= (O_REG16 | O_SREG | O_MEM); break;
  case 0x1: pOprd->Reg() = X86_Reg_Bx; pOprd->SecReg() = X86_Reg_Di; pOprd->Type() |= (O_REG16 | O_SREG | O_MEM); break;
  case 0x2: pOprd->Reg() = X86_Reg_Bp; pOprd->SecReg() = X86_Reg_Si; pOprd->Type() |= (O_REG16 | O_SREG | O_MEM); break;
  case 0x3: pOprd->Reg() = X86_Reg_Bp; pOprd->SecReg() = X86_Reg_Di; pOprd->Type() |= (O_REG16 | O_SREG | O_MEM); break;
  case 0x4: pOprd->Reg() = X86_Reg_Si;                               pOprd->Type() |= (O_REG16 |          O_MEM); break;
  case 0x5: pOprd->Reg() = X86_Reg_Di;                               pOprd->Type() |= (O_REG16 |          O_MEM); break;
  case 0x6:
    {
      // Mode
      if ((ModRm >> 6) == 0x0)
      {
        u16 Disp16;
        rBinStrm.Read(Offset + 1, Disp16);
        pOprd->Value() = Disp16;
        pOprd->Type() |= (O_DISP16 | O_MEM);
        pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
        rInsn.Length() += sizeof(Disp16);
        break;
      }
      pOprd->Reg() = X86_Reg_Bp;                               pOprd->Type() |= (O_REG16 |          O_MEM); break;
    }

  case 0x7: pOprd->Reg() = X86_Reg_Bx;                               pOprd->Type() |= (O_REG16 |          O_MEM); break;
  default: return false;
  }

  // Mode
  switch (ModRm >> 6)
  {
  case 0x0: return true;
  case 0x1:
    {
      u8 Disp8;
      rBinStrm.Read(Offset + 1, Disp8);
      pOprd->Value() = Disp8;
      pOprd->Type() |= (O_DISP8 | O_MEM);
      pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
      rInsn.Length() += sizeof(Disp8);
      break;
    }
  case 0x2:
    {
      u16 Disp16;
      rBinStrm.Read(Offset + 1, Disp16);
      pOprd->Value() = Disp16;
      pOprd->Type() |= (O_DISP16 | O_MEM);
      pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
      rInsn.Length() += sizeof(Disp16);
      break;
    }
  default:  return false;
  }

  return true;
}

static bool DecodeSib32(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  static u16 aReg[]          = { X86_Reg_Eax, X86_Reg_Ecx, X86_Reg_Edx,  X86_Reg_Ebx,  X86_Reg_Esp,     X86_Reg_Unknown, X86_Reg_Esi,  X86_Reg_Edi  };
  static u16 aRegIndex[]     = { X86_Reg_Eax, X86_Reg_Ecx, X86_Reg_Edx,  X86_Reg_Ebx,  X86_Reg_Unknown, X86_Reg_Ebp,     X86_Reg_Esi,  X86_Reg_Edi  };
  static u16 aRegRexB[]      = { X86_Reg_R8d, X86_Reg_R9d, X86_Reg_R10d, X86_Reg_R11d, X86_Reg_R12d,    X86_Reg_Unknown, X86_Reg_R14d, X86_Reg_R15d };
  static u16 aRegIndexRexX[] = { X86_Reg_R8d, X86_Reg_R9d, X86_Reg_R10d, X86_Reg_R11d, X86_Reg_R12d,    X86_Reg_R13d,    X86_Reg_R14d, X86_Reg_R15d };

  static u32 aScale[]    = { O_SCALE1, O_SCALE2, O_SCALE4, O_SCALE8 };

  u16* pReg      = (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? aRegRexB      : aReg;
  u16* pRegIndex = (rInsn.Prefix() & (X86_Prefix_REX_x & ~X86_Prefix_REX)) ? aRegIndexRexX : aRegIndex;
  u16  Reg101    = (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? X86_Reg_R13d  : X86_Reg_Ebp;
  u8   Mod;
  u8   Sib;

  rBinStrm.Read(Offset - 1, Mod);
  rBinStrm.Read(Offset, Sib);

  Mod >>= 6;

  if ((Sib & 0x7) == 0x5)
  {
    switch (Mod)
    {
      case 0x2:
        pOprd->Reg() = Reg101;
      case 0x0:
        {
          u32 Disp32;
          rBinStrm.Read(Offset + 1, Disp32);
          pOprd->Value() = Disp32;
          pOprd->Type() |= O_DISP32;
          pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
          rInsn.Length() += sizeof(Disp32);
          break;
        }

      case 0x1:
        {
          pOprd->Reg() = Reg101;

          u8 Disp8;
          rBinStrm.Read(Offset + 1, Disp8);
          pOprd->Value() = Disp8;
          pOprd->Type() |= O_DISP8;
          pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
          rInsn.Length() += sizeof(Disp8);
          break;
        }

      default: return false;
    }
  }
  else
  {
    pOprd->Reg()    = pReg[Sib & 0x7];

    switch (Mod)
    {
      case 0x2:
        {
          u32 Disp32;
          rBinStrm.Read(Offset + 1, Disp32);
          pOprd->Value() = Disp32;
          pOprd->Type() |= O_DISP32;
          pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
          rInsn.Length() += sizeof(Disp32);
        }
      case 0x0:
        pOprd->Type() |= O_REG32;
        break;

      case 0x1:
        {
          u8 Disp8;
          rBinStrm.Read(Offset + 1, Disp8);
          pOprd->Value() = Disp8;
          pOprd->Type() |= O_DISP8;
          pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
          rInsn.Length() += sizeof(Disp8);
          break;
        }
      default:  return false;
    };
  }

  pOprd->SecReg() = pRegIndex[(Sib >> 3) & 0x7];
  pOprd->Type()  |= (aScale[Sib >> 6] | O_REG32 | O_MEM | O_SREG);
  rInsn.Length() += sizeof(Sib);
  return true;
}

static bool DecodeModRmAddress32(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;
  static u16 aReg32[]     = { X86_Reg_Eax, X86_Reg_Ecx, X86_Reg_Edx,  X86_Reg_Ebx,  X86_Reg_Unknown, X86_Reg_Ebp, X86_Reg_Esi,  X86_Reg_Edi  };
  static u16 aReg32RexB[] = { X86_Reg_R8d, X86_Reg_R9d, X86_Reg_R10d, X86_Reg_R11d, X86_Reg_Unknown, X86_Reg_Eip, X86_Reg_R14d, X86_Reg_R15d };

  rBinStrm.Read(Offset, ModRm);

  // SIB
  if ((ModRm & 0x7) == 0x4 && (ModRm >> 6) != 0x3)
    return DecodeSib32(rBinStrm, Offset + sizeof(ModRm), rInsn, pOprd);

  // Mod == 0x0 and R/M == 0x5
  else if ((ModRm & 0xC7) == 0x5)
  {
    u32 Disp32;

    rBinStrm.Read(Offset + sizeof(ModRm), Disp32);
    pOprd->Type() |= (O_DISP32 | O_MEM);
    pOprd->Value() = Disp32;
    pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
    rInsn.Length() += sizeof(Disp32);
    return true;
  }

  else
  {
    pOprd->Reg() = (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? aReg32RexB[ModRm & 0x7] : aReg32[ModRm & 0x7];
    if (pOprd->Reg() != X86_Reg_Unknown)
      pOprd->Type() |= (O_REG32 | O_MEM);
    if (pOprd->Reg() == X86_Reg_Eip)
      pOprd->Type() |= O_REG_PC_REL;
  }

  // Mod
  switch (ModRm >> 6)
  {
  case 0x0: return true;
  case 0x1:
    {
      u8 Disp8;
      rBinStrm.Read(Offset + 1, Disp8);
      pOprd->Value() = Disp8;
      pOprd->Type() |= (O_DISP8 | O_MEM);
      pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
      rInsn.Length() += sizeof(Disp8);
      break;
    }
  case 0x2:
    {
      u32 Disp32;
      rBinStrm.Read(Offset + 1, Disp32);
      pOprd->Value() = Disp32;
      pOprd->Type() |= (O_DISP32 | O_MEM);
      pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
      rInsn.Length() += sizeof(Disp32);
      break;
    }
  default:  return false;
  }

  return true;
}

static bool DecodeSib64(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  static u16 aReg[]          = { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx,  X86_Reg_Rbx,  X86_Reg_Rsp,     X86_Reg_Unknown, X86_Reg_Rsi, X86_Reg_Rdi  };
  static u16 aRegRexB[]      = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10,  X86_Reg_R11,  X86_Reg_R12,     X86_Reg_Unknown, X86_Reg_R14, X86_Reg_R15d };
  static u16 aRegIndex[]     = { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx,  X86_Reg_Rbx,  X86_Reg_Unknown, X86_Reg_Rbp,     X86_Reg_Rsi, X86_Reg_Rdi  };
  static u16 aRegIndexRexX[] = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10,  X86_Reg_R11,  X86_Reg_R12,     X86_Reg_R13,     X86_Reg_R14, X86_Reg_R15d };

  static u32 aScale[]    = { O_SCALE1, O_SCALE2, O_SCALE4, O_SCALE8 };

  u16* pReg      = (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? aRegRexB      : aReg;
  u16* pRegIndex = (rInsn.Prefix() & (X86_Prefix_REX_x & ~X86_Prefix_REX)) ? aRegIndexRexX : aRegIndex;
  u16  Reg101    = (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? X86_Reg_R13   : X86_Reg_Rbp;
  u8   Mod;
  u8   Sib;

  rBinStrm.Read(Offset - 1, Mod);
  rBinStrm.Read(Offset, Sib);

  Mod >>= 6;

  if ((Sib & 0x7) == 0x5)
  {
    switch (Mod)
    {
      case 0x2:
        pOprd->Reg() = Reg101;
      case 0x0:
        {
          u32 Disp32;
          rBinStrm.Read(Offset + 1, Disp32);
          pOprd->Value() = Disp32;
          pOprd->Type() |= O_DISP32;
          pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
          rInsn.Length() += sizeof(Disp32);
          break;
        }

      case 0x1:
        {
          pOprd->Reg() = Reg101;

          u8 Disp8;
          rBinStrm.Read(Offset + 1, Disp8);
          pOprd->Value() = Disp8;
          pOprd->Type() |= O_DISP8;
          pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
          rInsn.Length() += sizeof(Disp8);
          break;
        }

      default: return false;
    }
  }
  else
  {
    pOprd->Reg()    = pReg[Sib & 0x7];

    switch (Mod)
    {
      case 0x2:
        {
          u32 Disp32;
          rBinStrm.Read(Offset + 1, Disp32);
          pOprd->Value() = Disp32;
          pOprd->Type() |= O_DISP32;
          pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
          rInsn.Length() += sizeof(Disp32);
        }
      case 0x0:
        pOprd->Type() |= O_REG32;
        break;

      case 0x1:
        {
          u8 Disp8;
          rBinStrm.Read(Offset + 1, Disp8);
          pOprd->Value() = Disp8;
          pOprd->Type() |= O_DISP8;
          pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
          rInsn.Length() += sizeof(Disp8);
          break;
        }
      default:  return false;
    };
  }

  pOprd->SecReg() = pRegIndex[(Sib >> 3) & 0x7];
  pOprd->Type()  |= (aScale[Sib >> 6] | O_REG64 | O_MEM | O_SREG);
  rInsn.Length() += sizeof(Sib);
  return true;
}

static bool DecodeModRmAddress64(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;
  static u16 aReg[]     = { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx, X86_Reg_Rbx, X86_Reg_Unknown, X86_Reg_Rbp, X86_Reg_Rsi, X86_Reg_Rdi };
  static u16 aRegRexB[] = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10, X86_Reg_R11, X86_Reg_Unknown, X86_Reg_Eip, X86_Reg_R14, X86_Reg_R15 };

  rBinStrm.Read(Offset, ModRm);

  // SIB
  if ((ModRm & 0x7) == 0x4 && (ModRm >> 6) != 0x3)
    return DecodeSib64(rBinStrm, Offset + 1, rInsn, pOprd);

  // Mod == 0x0 and R/M == 0x5
  else if ((ModRm & 0xC7) == 0x5)
  {
    u32 Disp32;

    rBinStrm.Read(Offset, Disp32);
    pOprd->Type() |= O_MEM;
    pOprd->Value() = Disp32;
    pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
    rInsn.Length() += sizeof(Disp32);
    return true;
  }

  else
  {
    pOprd->Reg() = (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? aRegRexB[ModRm & 0x7] : aReg[ModRm & 0x7];
    pOprd->Type() |= (O_REG32 | O_MEM);
  }

  // Mod
  switch (ModRm >> 6)
  {
  case 0x0: return true;
  case 0x1:
    {
      u8 Disp8;
      rBinStrm.Read(Offset + 1, Disp8);
      pOprd->Value() = Disp8;
      pOprd->Type() |= (O_DISP8 | O_MEM);
      pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
      rInsn.Length() += sizeof(Disp8);
      break;
    }
  case 0x2:
    {
      u32 Disp32;
      rBinStrm.Read(Offset + 1, Disp32);
      pOprd->Value() = Disp32;
      pOprd->Type() |= (O_DISP32 | O_MEM);
      pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
      rInsn.Length() += sizeof(Disp32);
      break;
    }
  default:  return false;
  }

  return true;
}

static bool DecodeModRmAddress(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, X86_Bit Bit)
{
  switch (Bit)
  {
  case X86_Bit_16:
    if (rInsn.Prefix() & X86_Prefix_AdSize) return DecodeModRmAddress32(rBinStrm, Offset, rInsn, pOprd);
    else                                    return DecodeModRmAddress16(rBinStrm, Offset, rInsn, pOprd);

  case X86_Bit_32:
    if (rInsn.Prefix() & X86_Prefix_AdSize) return DecodeModRmAddress16(rBinStrm, Offset, rInsn, pOprd);
    else                                    return DecodeModRmAddress32(rBinStrm, Offset, rInsn, pOprd);

  case X86_Bit_64:
    {
      u8 ModRm;
      u32 Disp32;

      rBinStrm.Read(Offset, ModRm);
      if ((ModRm & 0xC7) == 0x5)
      {
        u16 RegIp = (Bit == X86_Bit_64) ? X86_Reg_Rip : X86_Reg_Eip;
        rBinStrm.Read(Offset + sizeof(ModRm), Disp32);
        pOprd->Reg() = RegIp;
        pOprd->Value() = Disp32;
        pOprd->Type() |= (rInsn.Prefix() & X86_Prefix_AdSize) ?
          (O_REG32 | O_MEM | O_DISP32 | O_REG_PC_REL) : (O_REG64 | O_MEM | O_DISP32 | O_REG_PC_REL);
        pOprd->SetOffset(static_cast<u8>(rInsn.GetLength()));
        rInsn.Length() += sizeof(Disp32);
        return true;
      }
    }

    if (rInsn.Prefix() & X86_Prefix_AdSize) return DecodeModRmAddress32(rBinStrm, Offset, rInsn, pOprd);
    else                                    return DecodeModRmAddress64(rBinStrm, Offset, rInsn, pOprd);

  default: return false;
  };
}

/* General Purpose Register */

/* R/M */
static bool DecodeEbMod11(Instruction& rInsn, Operand* pOprd, u8 Reg)
{
  static u16 aReg[]         = { X86_Reg_Al,  X86_Reg_Cl,  X86_Reg_Dl,   X86_Reg_Bl,   X86_Reg_Ah,   X86_Reg_Ch,   X86_Reg_Dh,   X86_Reg_Bh   };
  static u16 aRegRex[]      = { X86_Reg_Al,  X86_Reg_Cl,  X86_Reg_Dl,   X86_Reg_Bl,   X86_Reg_Spl,  X86_Reg_Bpl,  X86_Reg_Sil,  X86_Reg_Dil  };
  static u16 aRegRexRexB[]  = { X86_Reg_R8b, X86_Reg_R9b, X86_Reg_R10b, X86_Reg_R11b, X86_Reg_R12b, X86_Reg_R13b, X86_Reg_R14b, X86_Reg_R15b };

  u16* pReg = NULL;

  if (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX))
    pReg = aRegRexRexB;
  else if (rInsn.Prefix() & X86_Prefix_REX)
    pReg = aRegRex;
  else
    pReg = aReg;

  pOprd->Type() |= O_REG8;
  pOprd->Reg() = pReg[Reg];
  return true;
}

static bool DecodeEwMod11(Instruction& rInsn, Operand* pOprd, u8 Reg)
{
  static u16 aReg[]     = { X86_Reg_Ax,  X86_Reg_Cx,  X86_Reg_Dx,   X86_Reg_Bx,   X86_Reg_Sp,   X86_Reg_Bp,   X86_Reg_Si,   X86_Reg_Di   };
  static u16 aRegRexB[] = { X86_Reg_R8w, X86_Reg_R9w, X86_Reg_R10w, X86_Reg_R11w, X86_Reg_R12w, X86_Reg_R13w, X86_Reg_R14w, X86_Reg_R15w };

  u16* pReg = NULL;
  if (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX))
    pReg = aRegRexB;
  else
    pReg = aReg;

  pOprd->Type() |= O_REG16;
  pOprd->Reg() = pReg[Reg];
  return true;
}

static bool DecodeEdMod11(Instruction& rInsn, Operand* pOprd, u8 Reg)
{
  static u16 aReg[]     = { X86_Reg_Eax, X86_Reg_Ecx, X86_Reg_Edx,  X86_Reg_Ebx,  X86_Reg_Esp,  X86_Reg_Ebp,  X86_Reg_Esi,  X86_Reg_Edi  };
  static u16 aRegRexB[] = { X86_Reg_R8d, X86_Reg_R9d, X86_Reg_R10d, X86_Reg_R11d, X86_Reg_R12d, X86_Reg_R13d, X86_Reg_R14d, X86_Reg_R15d };

  u16* pReg = NULL;
  if (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX))
    pReg = aRegRexB;
  else
    pReg = aReg;

  pOprd->Type() |= O_REG32;
  pOprd->Reg() = pReg[Reg];
  return true;
}

static bool DecodeEqMod11(Instruction& rInsn, Operand* pOprd, u8 Reg)
{
  static u16 aReg[]     = { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx,  X86_Reg_Rbx,  X86_Reg_Rsp,  X86_Reg_Rbp,  X86_Reg_Rsi,  X86_Reg_Rdi };
  static u16 aRegRexB[] = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10,  X86_Reg_R11,  X86_Reg_R12,  X86_Reg_R13,  X86_Reg_R14,  X86_Reg_R15 };

  u16* pReg = NULL;
  if (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX))
    pReg = aRegRexB;
  else
    pReg = aReg;

  pOprd->Type() |= O_REG64;
  pOprd->Reg() = pReg[Reg];
  return true;
}

bool DecodeRq(Instruction& rInsn, Operand* pOprd, u8 Reg) { return DecodeEqMod11(rInsn, pOprd, Reg); }

static bool DecodeQMod11(Instruction& rInsn, Operand* pOprd, u8 Reg)
{
  static u16 aReg[] = { X86_Reg_Mm0, X86_Reg_Mm1, X86_Reg_Mm2, X86_Reg_Mm3, X86_Reg_Mm4, X86_Reg_Mm5, X86_Reg_Mm6, X86_Reg_Mm7 };
  pOprd->Type() |= O_REG80;
  pOprd->Reg() = aReg[Reg];
  return true;
}

bool X86Architecture::Decode_Eb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2: pOprd->Type() |= O_MEM8; return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
  case 0x3:                          return DecodeEbMod11(rInsn, pOprd, ModRm & 0x7);
  default:                           return false;
  }
}

bool X86Architecture::Decode_Ew(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;
  rBinStrm.Read(Offset, ModRm);
  u8 Reg = ModRm & 0x7;
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2: pOprd->Type() |= O_MEM16; return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
  case 0x3:                           return DecodeEwMod11(rInsn, pOprd, Reg);
  default:                            return false;
  }
}

bool X86Architecture::Decode_Ed(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;
  rBinStrm.Read(Offset, ModRm);
  u8 Reg = ModRm & 0x7;
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2: pOprd->Type() |= O_MEM32; return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
  case 0x3:                           return DecodeEdMod11(rInsn, pOprd, Reg);
  default:                            return false;
  }
}

bool X86Architecture::Decode_Ev(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;
  rBinStrm.Read(Offset, ModRm);
  u8 Reg = ModRm & 0x7;
  rInsn.Length() += sizeof(ModRm);

  u32 Mem = O_NONE;

  switch (static_cast<X86_Bit>(m_Cfg.Get("Bit")))
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

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2: pOprd->Type() |= Mem; return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
  case 0x3:
    switch (static_cast<X86_Bit>(m_Cfg.Get("Bit")))
    {
    case X86_Bit_64: if (rInsn.Prefix() & (X86_Prefix_REX_w & ~X86_Prefix_REX)) return DecodeEqMod11(rInsn, pOprd, Reg);
    case X86_Bit_32:                                                            return (rInsn.Prefix() & X86_Prefix_OpSize) ? DecodeEwMod11(rInsn, pOprd, Reg) : DecodeEdMod11(rInsn, pOprd, Reg);
    case X86_Bit_16:                                                            return (rInsn.Prefix() & X86_Prefix_OpSize) ? DecodeEdMod11(rInsn, pOprd, Reg) : DecodeEwMod11(rInsn, pOprd, Reg);
    default:                                                                    return false;
    };
  default:                                                                      return false;
  }
}

bool X86Architecture::Decode_Ey(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;
  rBinStrm.Read(Offset, ModRm);
  u8 Reg = ModRm & 0x7;
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2: return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
  case 0x3:
    switch (static_cast<X86_Bit>(m_Cfg.Get("Bit")))
    {
    case X86_Bit_64: pOprd->Type() |= O_MEM64; return DecodeEqMod11(rInsn, pOprd, Reg);
    case X86_Bit_32: pOprd->Type() |= O_MEM32; return DecodeEdMod11(rInsn, pOprd, Reg);
    case X86_Bit_16: pOprd->Type() |= O_MEM16; return DecodeEdMod11(rInsn, pOprd, Reg);
    default:                                   return false;
    };
  default:  return false;
  }
}

bool X86Architecture::Decode_Rv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;
  rBinStrm.Read(Offset, ModRm);
  u8 Reg = ModRm & 0x7;
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2:
  case 0x3:
    switch (static_cast<X86_Bit>(m_Cfg.Get("Bit")))
    {
    case X86_Bit_16: return (rInsn.Prefix() & X86_Prefix_OpSize) ? DecodeEdMod11(rInsn, pOprd, Reg) : DecodeEwMod11(rInsn, pOprd, Reg);
    case X86_Bit_32: return (rInsn.Prefix() & X86_Prefix_OpSize) ? DecodeEwMod11(rInsn, pOprd, Reg) : DecodeEdMod11(rInsn, pOprd, Reg);
    case X86_Bit_64: return (rInsn.Prefix() & X86_Prefix_OpSize) ? DecodeEdMod11(rInsn, pOprd, Reg) : DecodeEqMod11(rInsn, pOprd, Reg);
    default:         return false;
    };
  default:  return false;
  }
}

bool X86Architecture::Decode_Ry(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;
  rBinStrm.Read(Offset, ModRm);
  u8 Reg = ModRm & 0x7;
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2:
  case 0x3:
    switch (static_cast<X86_Bit>(m_Cfg.Get("Bit")))
    {
    case X86_Bit_16:
    case X86_Bit_32: return DecodeEdMod11(rInsn, pOprd, Reg);
    case X86_Bit_64: return DecodeEqMod11(rInsn, pOprd, Reg);
    default:         return false;
    };
  default:  return false;
  }
}

bool X86Architecture::Decode_M(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  x86::ModRM ModRm = GetModRm(rBinStrm, Offset);
  if (ModRm.Mod() == 0x3)
    return false;
  rInsn.Length()++;
  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
}

/* Reg */

bool X86Architecture::Decode_Gb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  static u16 aReg[]         = { X86_Reg_Al,  X86_Reg_Cl,  X86_Reg_Dl,   X86_Reg_Bl,   X86_Reg_Ah,   X86_Reg_Ch,   X86_Reg_Dh,   X86_Reg_Bh   };
  static u16 aRegRex[]      = { X86_Reg_Al,  X86_Reg_Cl,  X86_Reg_Dl,   X86_Reg_Bl,   X86_Reg_Spl,  X86_Reg_Bpl,  X86_Reg_Sil,  X86_Reg_Dil  };
  static u16 aRegRexRexR[]  = { X86_Reg_R8b, X86_Reg_R9b, X86_Reg_R10b, X86_Reg_R11b, X86_Reg_R12b, X86_Reg_R13b, X86_Reg_R14b, X86_Reg_R15b };

  u8 Reg = GetModRm(rBinStrm, Offset).Reg();

  u16* pReg = NULL;
  if (rInsn.Prefix() & (X86_Prefix_REX_r & ~X86_Prefix_REX))
    pReg = aRegRexRexR;
  else if (rInsn.Prefix() & X86_Prefix_REX)
    pReg = aRegRex;
  else
    pReg = aReg;

  pOprd->Type() |= O_REG8;
  pOprd->Reg() = pReg[Reg];
  return true;
}

bool DecodeGw(Instruction& rInsn, Operand* pOprd, u8 Reg)
{
  static u16 aReg[]     = { X86_Reg_Ax,  X86_Reg_Cx,  X86_Reg_Dx,   X86_Reg_Bx,   X86_Reg_Sp,   X86_Reg_Bp,   X86_Reg_Si,   X86_Reg_Di   };
  static u16 aRegRexR[] = { X86_Reg_R8w, X86_Reg_R9w, X86_Reg_R10w, X86_Reg_R11w, X86_Reg_R12w, X86_Reg_R13w, X86_Reg_R14w, X86_Reg_R15w };

  u16* pReg = NULL;
  if (rInsn.Prefix() & (X86_Prefix_REX_r & ~X86_Prefix_REX))
    pReg = aRegRexR;
  else
    pReg = aReg;

  pOprd->Type() |= O_REG16;
  pOprd->Reg() = pReg[Reg];
  return true;
}

bool DecodeGd(Instruction& rInsn, Operand* pOprd, u8 Reg)
{
  static u16 aReg[] =     { X86_Reg_Eax, X86_Reg_Ecx, X86_Reg_Edx,  X86_Reg_Ebx,  X86_Reg_Esp,  X86_Reg_Ebp,  X86_Reg_Esi,  X86_Reg_Edi  };
  static u16 aRegRexR[] = { X86_Reg_R8d, X86_Reg_R9d, X86_Reg_R10d, X86_Reg_R11d, X86_Reg_R12d, X86_Reg_R13d, X86_Reg_R14d, X86_Reg_R15d };

  u16* pReg = NULL;
  if (rInsn.Prefix() & (X86_Prefix_REX_r & ~X86_Prefix_REX))
    pReg = aRegRexR;
  else
    pReg = aReg;

  pOprd->Type() |= O_REG32;
  pOprd->Reg() = pReg[Reg];
  return true;
}

bool DecodeGq(Instruction& rInsn, Operand* pOprd, u8 Reg)
{
  static u16 aReg[] =     { X86_Reg_Rax, X86_Reg_Rcx, X86_Reg_Rdx,  X86_Reg_Rbx,  X86_Reg_Rsp,  X86_Reg_Rbp,  X86_Reg_Rsi,  X86_Reg_Rdi };
  static u16 aRegRexR[] = { X86_Reg_R8,  X86_Reg_R9,  X86_Reg_R10,  X86_Reg_R11,  X86_Reg_R12,  X86_Reg_R13,  X86_Reg_R14,  X86_Reg_R15 };

  u16* pReg = NULL;
  if (rInsn.Prefix() & (X86_Prefix_REX_r & ~X86_Prefix_REX))
    pReg = aRegRexR;
  else
    pReg = aReg;

  pOprd->Type() |= O_REG64;
  pOprd->Reg() = pReg[Reg];
  return true;
}

bool X86Architecture::Decode_Gw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  return DecodeGw(rInsn, pOprd, GetModRm(rBinStrm, Offset).Reg());
}

bool X86Architecture::Decode_Gv(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 Reg;
  rBinStrm.Read(Offset, Reg);
  Reg = (Reg >> 3) & 0x7;
  switch (static_cast<X86_Bit>(m_Cfg.Get("Bit")))
  {
  case X86_Bit_64: if     (rInsn.Prefix() & (X86_Prefix_REX_w & ~X86_Prefix_REX))   return DecodeGq(rInsn, pOprd, Reg);
  case X86_Bit_32: return (rInsn.Prefix() & X86_Prefix_OpSize) ? DecodeGw(rInsn, pOprd, Reg) : DecodeGd(rInsn, pOprd, Reg);
  case X86_Bit_16: return (rInsn.Prefix() & X86_Prefix_OpSize) ? DecodeGd(rInsn, pOprd, Reg) : DecodeGw(rInsn, pOprd, Reg);
  default:         return false;
  };
}

bool X86Architecture::Decode_Gy(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 Reg;
  rBinStrm.Read(Offset, Reg);
  Reg = (Reg >> 3) & 0x7;
  switch (static_cast<X86_Bit>(m_Cfg.Get("Bit")))
  {
  case X86_Bit_16:
  case X86_Bit_32: return DecodeGd(rInsn, pOprd, Reg);
  case X86_Bit_64: return DecodeGq(rInsn, pOprd, Reg);
  default:         return false;
  };
}

/* MMX Register */

/* R/M */

static bool DecodeNMod11(Instruction& rInsn, Operand* pOprd, u8 Reg)
{
  static u16 aReg[] = { X86_Reg_Mm0, X86_Reg_Mm1, X86_Reg_Mm2, X86_Reg_Mm3, X86_Reg_Mm4, X86_Reg_Mm5, X86_Reg_Mm6, X86_Reg_Mm7 };
  pOprd->Type() |= O_REG80;
  pOprd->Reg() = aReg[Reg];
  return true;
}

bool X86Architecture::Decode_Nq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2:
  case 0x3: return DecodeNMod11(rInsn, pOprd, ModRm & 0x7);
  default:  return false;
  }
}

bool X86Architecture::Decode_Qd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2: return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
  case 0x3: return DecodeNMod11(rInsn, pOprd, ModRm & 0x7);
  default:  return false;
  }
}

bool X86Architecture::Decode_Qq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2: return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
  case 0x3: return DecodeNMod11(rInsn, pOprd, ModRm & 0x7);
  default:  return false;
  }
}

/* Reg */

bool X86Architecture::Decode_Pq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  static u16 aReg[] = { X86_Reg_Mm0, X86_Reg_Mm1, X86_Reg_Mm2, X86_Reg_Mm3, X86_Reg_Mm4, X86_Reg_Mm5, X86_Reg_Mm6, X86_Reg_Mm7 };
  u8 Reg = GetModRm(rBinStrm, Offset).Reg();
  pOprd->Type() |= O_REG80;
  pOprd->Reg() = aReg[Reg];
  return true;
}

/* XMM Register */

/* R/M */

static bool DecodeUMod11(Instruction& rInsn, Operand* pOprd, u8 Reg)
{
  static u16 aReg[]     = { X86_Reg_Xmm0, X86_Reg_Xmm1, X86_Reg_Xmm2,  X86_Reg_Xmm3,  X86_Reg_Xmm4,  X86_Reg_Xmm5,  X86_Reg_Xmm6,  X86_Reg_Xmm7  };
  static u16 aRegRexB[] = { X86_Reg_Xmm8, X86_Reg_Xmm9, X86_Reg_Xmm10, X86_Reg_Xmm11, X86_Reg_Xmm12, X86_Reg_Xmm13, X86_Reg_Xmm14, X86_Reg_Xmm15 };

  pOprd->Type() |= O_REG128;
  pOprd->Reg()   = (rInsn.Prefix() & (X86_Prefix_REX_b & ~X86_Prefix_REX)) ? aRegRexB[Reg] : aReg[Reg];
  return true;
}

bool X86Architecture::Decode_Uo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2:
  case 0x3: return DecodeUMod11(rInsn, pOprd, ModRm & 0x7);
  default:  return false;
  }
}

bool X86Architecture::Decode_Uq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2:
  case 0x3: return DecodeUMod11(rInsn, pOprd, ModRm & 0x7);
  default:  return false;
  }
}

bool X86Architecture::Decode_Ux(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2:
  case 0x3: return DecodeUMod11(rInsn, pOprd, ModRm & 0x7);
  default:  return false;
  }
}

bool X86Architecture::Decode_Wo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2: return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
  case 0x3: return DecodeUMod11(rInsn, pOprd, (ModRm >> 3) & 0x7);
  default:  return false;
  }
}

bool X86Architecture::Decode_Wx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);

  switch (ModRm >> 6)
  {
  case 0x0:
  case 0x1:
  case 0x2: return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
  case 0x3: return DecodeUMod11(rInsn, pOprd, (ModRm >> 3) & 0x7);
  default:  return false;
  }
}

/* Reg */

bool X86Architecture::Decode_Vo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  static u16 aReg[]     = { X86_Reg_Xmm0, X86_Reg_Xmm1, X86_Reg_Xmm2,  X86_Reg_Xmm3,  X86_Reg_Xmm4,  X86_Reg_Xmm5,  X86_Reg_Xmm6,  X86_Reg_Xmm7  };
  static u16 aRegRexR[] = { X86_Reg_Xmm8, X86_Reg_Xmm9, X86_Reg_Xmm10, X86_Reg_Xmm11, X86_Reg_Xmm12, X86_Reg_Xmm13, X86_Reg_Xmm14, X86_Reg_Xmm15 };
  u8 Reg = GetModRm(rBinStrm, Offset).Reg();
  pOprd->Type() |= O_REG128;
  pOprd->Reg()   = (rInsn.Prefix() & (X86_Prefix_REX_r & ~X86_Prefix_REX)) ? aRegRexR[Reg] : aReg[Reg];
  return true;
}

bool X86Architecture::Decode_Vx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  static u16 aReg[]     = { X86_Reg_Xmm0, X86_Reg_Xmm1, X86_Reg_Xmm2,  X86_Reg_Xmm3,  X86_Reg_Xmm4,  X86_Reg_Xmm5,  X86_Reg_Xmm6,  X86_Reg_Xmm7  };
  static u16 aRegRexR[] = { X86_Reg_Xmm8, X86_Reg_Xmm9, X86_Reg_Xmm10, X86_Reg_Xmm11, X86_Reg_Xmm12, X86_Reg_Xmm13, X86_Reg_Xmm14, X86_Reg_Xmm15 };
  u8 Reg = GetModRm(rBinStrm, Offset).Reg();
  pOprd->Type() |= O_REG128;
  pOprd->Reg()   = (rInsn.Prefix() & (X86_Prefix_REX_r & ~X86_Prefix_REX)) ? aRegRexR[Reg] : aReg[Reg];
  return true;
}

/* FPU memory operands */

bool X86Architecture::Decode_m16int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);
  pOprd->Type() |= O_MEM16;
  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
}

bool X86Architecture::Decode_m32fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);
  pOprd->Type() |= O_MEM32;
  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
}

bool X86Architecture::Decode_m32int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);
  pOprd->Type() |= O_MEM32;
  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
}

bool X86Architecture::Decode_m64fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);
  pOprd->Type() |= O_MEM64;
  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
}

bool X86Architecture::Decode_m64int(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);
  pOprd->Type() |= O_MEM64;
  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
}

bool X86Architecture::Decode_m80fp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);
  pOprd->Type() |= O_MEM80;
  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
}

bool X86Architecture::Decode_m80dec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);
  pOprd->Type() |= O_MEM80;
  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
}

bool X86Architecture::Decode_m80bcd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);
  pOprd->Type() |= O_MEM80;
  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
}

/* M : memory operands only */

bool X86Architecture::Decode_Ma(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  return false; /* UNHANDLED OPERAND */
}

bool X86Architecture::Decode_Md(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  return false; /* UNHANDLED OPERAND */
}

bool X86Architecture::Decode_Mo(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  return false; /* UNHANDLED OPERAND */
}

bool X86Architecture::Decode_Mp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  return false; /* UNHANDLED OPERAND */
}

bool X86Architecture::Decode_Mq(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  return false; /* UNHANDLED OPERAND */
}

bool X86Architecture::Decode_Mw(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);
  pOprd->Type() |= O_MEM16;
  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, static_cast<X86_Bit>(m_Cfg.Get("Bit")));
}

bool X86Architecture::Decode_Mx(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  return false; /* UNHANDLED OPERAND */
}

/* LATER: Forbid != MEM */
bool X86Architecture::Decode_My(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)
{
  auto Bit = static_cast<X86_Bit>(m_Cfg.Get("Bit"));
  if (Bit == X86_Bit_16)
    return false;

  u8 ModRm;

  rBinStrm.Read(Offset, ModRm);
  rInsn.Length() += sizeof(ModRm);

  return DecodeModRmAddress(rBinStrm, Offset, rInsn, pOprd, Bit);
}
