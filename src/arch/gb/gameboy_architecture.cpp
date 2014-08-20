#include "gameboy_architecture.hpp"
#include <sstream>
#include <medusa/medusa.hpp>

GameBoyArchitecture::TRegName const GameBoyArchitecture::m_RegName[] =
{
  { GB_RegA,        "A" },
  { GB_RegB,        "B" },
  { GB_RegC,        "C" },
  { GB_RegD,        "D" },
  { GB_RegE,        "E" },
  { GB_RegF,        "F" },
  { GB_RegH,        "H" },
  { GB_RegL,        "L" },
  { GB_RegSp,       "Sp"},
  { GB_RegPc,       "Pc"},
  { GB_RegAF,       "AF"},
  { GB_RegBC,       "BC"},
  { GB_RegDE,       "DE"},
  { GB_RegHL,       "HL"},
  { GB_Invalid_Reg, ""  }
};

char const* GameBoyArchitecture::GameBoyCpuInformation::ConvertIdentifierToName(u32 Id) const
{
  switch (Id)
  {
  default: return "";

    /* Register */
  case GB_RegA: return "a"; case GB_RegF: return "f"; case GB_RegAF: return "af";
  case GB_RegB: return "b"; case GB_RegC: return "c"; case GB_RegBC: return "bc";
  case GB_RegD: return "d"; case GB_RegE: return "e"; case GB_RegDE: return "de";
  case GB_RegH: return "h"; case GB_RegL: return "l"; case GB_RegHL: return "hl";

    /* Pseudo-register */
  case GB_RegPc: return "pc"; case GB_RegSp: return "sp"; case GB_RegFl: return "flags";

    /* Flag */
  case GB_FlCf: return "cf"; case GB_FlHf: return "hf";
  case GB_FlNf: return "nf"; case GB_FlZf: return "zf";
  }
}

u32 GameBoyArchitecture::GameBoyCpuInformation::ConvertNameToIdentifier(std::string const& rName) const
{
  static std::unordered_map<std::string, u32> s_NameToId;
  if (s_NameToId.empty())
  {
    s_NameToId["a"] = GB_RegA; s_NameToId["f"] = GB_RegF; s_NameToId["af"] = GB_RegAF;
    s_NameToId["b"] = GB_RegB; s_NameToId["c"] = GB_RegC; s_NameToId["bc"] = GB_RegBC;
    s_NameToId["d"] = GB_RegD; s_NameToId["e"] = GB_RegE; s_NameToId["de"] = GB_RegDE;
    s_NameToId["h"] = GB_RegH; s_NameToId["l"] = GB_RegL; s_NameToId["hl"] = GB_RegHL;
    s_NameToId["pc"] = GB_RegPc; s_NameToId["sp"] = GB_RegSp; s_NameToId["flags"] = GB_RegFl;
    s_NameToId["cf"] = GB_FlCf; s_NameToId["hl"] = GB_FlHf;
    s_NameToId["nf"] = GB_FlNf; s_NameToId["zf"] = GB_FlZf;
  }
  auto itId = s_NameToId.find(rName);
  if (itId == std::end(s_NameToId))
    return 0;

  return itId->second;
}

u32 GameBoyArchitecture::GameBoyCpuInformation::GetRegisterByType(CpuInformation::Type RegType, u8 Mode) const
{
  switch (RegType)
  {
  default:                     return 0;
  case StackPointerRegister:   return GB_RegSp;
  case StackFrameRegister:     return 0;
  case ProgramPointerRegister: return GB_RegPc;
  case FlagRegister:           return GB_RegFl;
  }
}

u32 GameBoyArchitecture::GameBoyCpuInformation::GetSizeOfRegisterInBit(u32 Id) const
{
  switch (Id)
  {
  default: return 0;

    /* Register */
  case GB_RegA: case GB_RegF:
  case GB_RegB: case GB_RegC:
  case GB_RegD: case GB_RegE:
  case GB_RegH: case GB_RegL:
    return 8;
  case GB_RegAF: case GB_RegBC:
  case GB_RegDE: case GB_RegHL:
    return 16;

    /* Pseudo-register */
  case GB_RegPc: case GB_RegSp:
    return 16;
  case GB_RegFl:
    return 8;

    /* Flag */
  case GB_FlCf: case GB_FlHf:
  case GB_FlNf: case GB_FlZf:
    return 1;
  }
}

bool GameBoyArchitecture::GameBoyCpuInformation::IsRegisterAliased(u32 Id0, u32 Id1) const
{
  if (Id0 == Id1)
    return true;

  if (Id0 > Id1)
    std::swap(Id0, Id1);

  switch (Id0)
  {
  case GB_RegAF: return Id1 == GB_RegA || Id1 == GB_RegF;
  case GB_RegBC: return Id1 == GB_RegB || Id1 == GB_RegC;
  case GB_RegDE: return Id1 == GB_RegD || Id1 == GB_RegE;
  case GB_RegHL: return Id1 == GB_RegH || Id1 == GB_RegL;
  }
  return false;
}

bool GameBoyArchitecture::Translate(Address const& rVirtAddr, TOffset& rPhysOff)
{
  return true;
}

bool GameBoyArchitecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);
  bool Result;

  if (Opcode == 0xCB)
  {
    rBinStrm.Read(Offset + 1, Opcode);
    Result = (this->*m_CbPrefix[Opcode])(rBinStrm, Offset + 1, rInsn);
  }
  else
    Result = (this->*m_OpcodeMap[Opcode])(rBinStrm, Offset, rInsn);

  // LATER: clean this
  for (u8 i = 0; i < OPERAND_NO; ++i)
  {
    auto& rOperand = *rInsn.Operand(i);
    if (rOperand.GetType() & O_REG)
    {
      switch (rOperand.GetReg())
      {
      case GB_RegA: case GB_RegB: case GB_RegC: case GB_RegD:
      case GB_RegE: case GB_RegF: case GB_RegH: case GB_RegL:
        rOperand.Type() |= O_REG8; break;
      case GB_RegAF: case GB_RegBC: case GB_RegDE: case GB_RegHL:
      case GB_RegPc: case GB_RegSp:
        rOperand.Type() |= O_REG16; break;
      default: break;
      }
    }
    if (rOperand.GetType() & O_MEM)
      rOperand.Type() |= O_MEM8;

    if (rOperand.GetType() & O_REL)
      rOperand.Type() |= O_REL16;

    if (rOperand.GetType() & O_ABS)
      rOperand.Type() |= O_ABS16;
  }

  return Result;
}

u16 GameBoyArchitecture::GetRegisterByOpcode(u8 Opcode)
{
  u8 Reg = Opcode & 0x7;

  switch (Reg)
  {
  case 0x0: return GB_RegB;
  case 0x1: return GB_RegC;
  case 0x2: return GB_RegD;
  case 0x3: return GB_RegE;
  case 0x4: return GB_RegH;
  case 0x5: return GB_RegL;
  case 0x6: return GB_RegHL;
  case 0x7: return GB_RegA;
  default:  return GB_Invalid_Reg;
  }
}

bool GameBoyArchitecture::Insn_Invalid(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("invalid");
  rInsn.Opcode() = GB_Invalid_Insn;
  return false;
}

bool GameBoyArchitecture::Insn_Nop(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.Opcode() = GB_Nop;
  rInsn.Length() = 1;
  rInsn.SetName("nop");

  return true;
}

bool GameBoyArchitecture::Insn_Inc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u16 Reg;
  u32 Type = O_REG;
  u8  Opcode;

  rInsn.SetName("inc");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Inc;

  rBinStrm.Read(Offset, Opcode);
  switch (Opcode)
  {
  case 0x03: Reg = GB_RegBC;  break;
  case 0x13: Reg = GB_RegDE;  break;
  case 0x23: Reg = GB_RegHL;  break;
  case 0x33: Reg = GB_RegSp;  break;
  case 0x3c: Reg = GB_RegA;   break;
  case 0x04: Reg = GB_RegB;   break;
  case 0x0C: Reg = GB_RegC;   break;
  case 0x14: Reg = GB_RegD;   break;
  case 0x1C: Reg = GB_RegE;   break;
  case 0x24: Reg = GB_RegH;   break;
  case 0x2C: Reg = GB_RegL;   break;
  case 0x34: Reg = GB_RegHL; Type |= O_MEM; break;
  default: return false;
  }

  Operand& FrstOperand = rInsn.FirstOperand();
  FrstOperand.Type()   = Type;
  FrstOperand.Reg()    = Reg;

  rInsn.SetUpdatedFlags(GB_FlZf | GB_FlHf);
  rInsn.SetFixedFlags(GB_FlNf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pOprdExpr = FrstOperand.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr != nullptr)
  {
    auto pExpr = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(OperationExpression::OpAdd, pOprdExpr, new ConstantExpression(pOprdExpr->GetSizeInBit(), 1))
    );
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Dec(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u16 Reg;
  u32 Type = O_REG;
  u8  Opcode;

  rInsn.SetName("dec");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Dec;

  rBinStrm.Read(Offset, Opcode);
  switch (Opcode)
  {
  case 0x0B: Reg = GB_RegBC;  break;
  case 0x1B: Reg = GB_RegDE;  break;
  case 0x2B: Reg = GB_RegHL;  break;
  case 0x3B: Reg = GB_RegSp;  break;
  case 0x3D: Reg = GB_RegA;   break;
  case 0x05: Reg = GB_RegB;   break;
  case 0x0D: Reg = GB_RegC;   break;
  case 0x15: Reg = GB_RegD;   break;
  case 0x1D: Reg = GB_RegE;   break;
  case 0x25: Reg = GB_RegH;   break;
  case 0x2D: Reg = GB_RegL;   break;
  case 0x35: Reg = GB_RegHL; Type |= O_MEM; break;
  default: return false;
  }

  Operand& FrstOperand =  rInsn.FirstOperand();
  FrstOperand.Type()   = Type;
  FrstOperand.Reg()    = Reg;

  rInsn.SetUpdatedFlags(GB_FlZf | GB_FlHf);
  rInsn.SetClearedFlags(GB_FlNf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pOprdExpr = FrstOperand.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr != nullptr)
  {
    auto pExpr = Expr::MakeAssign(
      pOprdExpr->Clone(),
      Expr::MakeOp(OperationExpression::OpSub, pOprdExpr, Expr::MakeConst(pOprdExpr->GetSizeInBit(), 1)));
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Add(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  bool Res = false;
  u8 Opcode;
  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  rInsn.SetName("add");
  rInsn.Opcode() = GB_Add;

  FrstOp.Type() = O_REG;
  FrstOp.Reg()  = GB_RegA;
  ScdOp.Type()  = O_REG;

  rBinStrm.Read(Offset, Opcode);

  if ((Opcode & 0x0F) == 0x09)
  {
    FrstOp.Reg() = GB_RegHL;

    switch (Opcode)
    {
    case 0x09: ScdOp.Reg() = GB_RegBC; break;
    case 0x19: ScdOp.Reg() = GB_RegDE; break;
    case 0x29: ScdOp.Reg() = GB_RegHL; break;
    case 0x39: ScdOp.Reg() = GB_RegSp; break;
    default: return false;
    }

    rInsn.Length() = 1;
    Res = true;
  }

  else if (Opcode >= 0x80 && Opcode <= 0x87)
  {
    if (Opcode == 0x86)
      ScdOp.Type() |= O_MEM;
    ScdOp.Reg() = GetRegisterByOpcode(Opcode);

    rInsn.Length() = 1;
    Res = true;;
  }
  
  // A, n
  else if (Opcode == 0xC6)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    Res = true;
  }

  // ADD sp, n
  else if (Opcode == 0xE8)
  {
    FrstOp.Type() = O_REG16;
    FrstOp.Reg()  = GB_RegSp;

    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    Res = true;
  }

  rInsn.SetUpdatedFlags(GB_FlZf | GB_FlHf | GB_FlCf);
  rInsn.SetClearedFlags(GB_FlNf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  if (Res == true)
  {
    auto pLeftOprd  = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
    auto pRightOprd = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

    if (pLeftOprd != nullptr && pRightOprd != nullptr)
    {
      auto pExpr = Expr::MakeAssign(
        pLeftOprd->Clone(),
        Expr::MakeOp(OperationExpression::OpAdd, pLeftOprd, pRightOprd));
      rInsn.SetSemantic(pExpr);
    }
  }

  return Res;
}

bool GameBoyArchitecture::Insn_Sub(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  bool Res = false;
  u8 Opcode;
  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  rInsn.SetName("sub");
  rInsn.Opcode() = GB_Sub;
  FrstOp.Type()  = O_REG;
  FrstOp.Reg()   = GB_RegA;

  rBinStrm.Read(Offset, Opcode);

  // A, Reg8
  if (Opcode >= 0x90 && Opcode <= 0x97)
  {
    ScdOp.Type() = O_REG;

    // A, (HL)
    if (Opcode == 0x96)
      ScdOp.Type() |= O_MEM;
    ScdOp.Reg() = GetRegisterByOpcode(Opcode);

    rInsn.Length() = 1;
    Res = true;
  }

  // A, n
  else if (Opcode == 0xD6)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    Res = true;
  }

  rInsn.SetUpdatedFlags(GB_FlZf | GB_FlHf | GB_FlCf);
  rInsn.SetFixedFlags(GB_FlNf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  if (Res == true)
  {
    auto pLeftOprd  = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
    auto pRightOprd = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

    if (pLeftOprd != nullptr && pRightOprd != nullptr)
    {
      auto pExpr = Expr::MakeAssign(
        pLeftOprd->Clone(),
          new OperationExpression(OperationExpression::OpSub,
            pLeftOprd,
            pRightOprd
          ));
      rInsn.SetSemantic(pExpr);
    }
  }

  return Res;
}

bool GameBoyArchitecture::Insn_Adc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  bool Res = false;
  u8 Opcode;

  rBinStrm.Read(Offset, Opcode);

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp = rInsn.SecondOperand();

  rInsn.SetName("adc");
  rInsn.Opcode() = GB_Adc;
  FrstOp.Type() = O_REG;
  FrstOp.Reg()  = GB_RegA;

  // A, Reg8
  if (Opcode >= 0x88 && Opcode <= 0x8f)
  {
    ScdOp.Type() = O_REG;

    // A, (HL)
    if (Opcode == 0x8E)
      ScdOp.Type() |= O_MEM;
    ScdOp.Reg() = GetRegisterByOpcode(Opcode);

    rInsn.Length() = 1;
    Res = true;
  }

  // A, n
  else if (Opcode == 0xCE)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    Res = true;
  }

  rInsn.SetUpdatedFlags(GB_FlZf | GB_FlHf | GB_FlCf);
  rInsn.SetClearedFlags(GB_FlNf);

  if (Res == true)
  {
    auto pLeftOprd  = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
    auto pRightOprd = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

    if (pLeftOprd != nullptr && pRightOprd != nullptr)
    {
      auto pExpr = Expr::MakeAssign(
        pLeftOprd->Clone(),
          new OperationExpression(OperationExpression::OpAdd,
            pLeftOprd,
            new OperationExpression(
              OperationExpression::OpAdd,
              pRightOprd,
              new IdentifierExpression(GB_FlCf, &m_CpuInfo))));
      rInsn.SetSemantic(pExpr);
    }
  }

  return Res;
}

bool GameBoyArchitecture::Insn_Sbc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  bool Res = false;
  u8 Opcode;

  rBinStrm.Read(Offset, Opcode);

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  rInsn.SetName("sbc");
  rInsn.Opcode() = GB_Sbc;

  FrstOp.Type()  = O_REG;
  FrstOp.Reg()   = GB_RegA;

  // A, Reg8
  if (Opcode >= 0x98 && Opcode <= 0x9f)
  {
    ScdOp.Type() = O_REG;

    // A, (HL)
    if (Opcode == 0x9E)
      ScdOp.Type() |= O_MEM;
    ScdOp.Reg() = GetRegisterByOpcode(Opcode);

    rInsn.Length() = 1;
    Res = true;
  }

  // A, n
  else if (Opcode == 0xDE)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    Res = true;
  }

  rInsn.SetUpdatedFlags(GB_FlZf | GB_FlHf | GB_FlCf);
  rInsn.SetFixedFlags(GB_FlNf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  if (Res == true)
  {
    auto pLeftOprd  = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
    auto pRightOprd = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

    if (pLeftOprd != nullptr && pRightOprd != nullptr)
    {
      auto pExpr = Expr::MakeAssign(
        pLeftOprd->Clone(),
          new OperationExpression(OperationExpression::OpSub,
            pLeftOprd,
            new OperationExpression(
              OperationExpression::OpSub,
              pRightOprd,
              new IdentifierExpression(GB_FlCf, &m_CpuInfo))));
      rInsn.SetSemantic(pExpr);
    }
  }

  return Res;
}

bool GameBoyArchitecture::Insn_Daa(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("daa");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Daa;
  return true;
}

bool GameBoyArchitecture::Insn_And(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  bool Res = false;
  u8 Opcode;

  rBinStrm.Read(Offset, Opcode);

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  rInsn.SetName("and");
  rInsn.Opcode() = GB_And;
  FrstOp.Type()  = O_REG;
  FrstOp.Reg()   = GB_RegA;

  // A, Reg8
  if (Opcode >= 0xA0 && Opcode <= 0xAF)
  {
    ScdOp.Type() = O_REG;

    // A, (HL)
    if (Opcode == 0xA6)
      ScdOp.Type() |= O_MEM;

    ScdOp.Reg() = GetRegisterByOpcode(Opcode);

    rInsn.Length() = 1;
    Res = true;
  }

  // A, n
  else if (Opcode == 0xE6)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    Res = true;
  }

  rInsn.SetUpdatedFlags(GB_FlZf);
  rInsn.SetClearedFlags(GB_FlNf | GB_FlCf);
  rInsn.SetFixedFlags(GB_FlHf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  if (Res == true)
  {
    auto pLeftOprd  = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
    auto pRightOprd = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

    if (pLeftOprd != nullptr && pRightOprd != nullptr)
    {
      auto pExpr = Expr::MakeAssign(
        pLeftOprd->Clone(),
          new OperationExpression(OperationExpression::OpAnd,
            pLeftOprd,
            pRightOprd
          ));
      rInsn.SetSemantic(pExpr);
    }
  }

  return Res;
}

bool GameBoyArchitecture::Insn_Or(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  bool Res = false;
  u8 Opcode;

  rBinStrm.Read(Offset, Opcode);

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  rInsn.SetName("or");
  rInsn.Opcode() = GB_Or;
  FrstOp.Type()  = O_REG;
  FrstOp.Reg()   = GB_RegA;

  // A, Reg8
  if (Opcode >= 0xB0 && Opcode <= 0xB7)
  {
    ScdOp.Type() = O_REG;

    // A, (HL)
    if (Opcode == 0xB6)
      ScdOp.Type() |= O_MEM;
    ScdOp.Reg() = GetRegisterByOpcode(Opcode);

    rInsn.Length() = 1;
    Res = true;
  }

  // A, n
  else if (Opcode == 0xF6)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    Res = true;
  }

  rInsn.SetUpdatedFlags(GB_FlZf);
  rInsn.SetClearedFlags(GB_FlNf | GB_FlHf | GB_FlCf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  if (Res == true)
  {
    auto pLeftOprd  = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
    auto pRightOprd = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

    if (pLeftOprd != nullptr && pRightOprd != nullptr)
    {
      auto pExpr = Expr::MakeAssign(
        pLeftOprd->Clone(),
          new OperationExpression(OperationExpression::OpOr,
            pLeftOprd,
            pRightOprd
          ));
      rInsn.SetSemantic(pExpr);
    }
  }

  return Res;
}

bool GameBoyArchitecture::Insn_Xor(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  bool Res = false;
  u8 Opcode;

  rBinStrm.Read(Offset, Opcode);

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp = rInsn.SecondOperand();

  rInsn.SetName("xor");
  rInsn.Opcode() = GB_Xor;
  FrstOp.Type()  = O_REG;
  FrstOp.Reg()   = GB_RegA;

  // A, Reg8
  if (Opcode >= 0xA8 && Opcode <= 0xAF)
  {
    ScdOp.Type() = O_REG;

    // A, (HL)
    if (Opcode == 0xAE)
      ScdOp.Type() |= O_MEM;
    ScdOp.Reg() = GetRegisterByOpcode(Opcode);

    rInsn.Length() = 1;
    Res = true;
  }

  // A, n
  else if (Opcode == 0xEE)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    Res = true;
  }

  rInsn.SetUpdatedFlags(GB_FlZf);
  rInsn.SetClearedFlags(GB_FlNf | GB_FlHf | GB_FlCf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  if (Res == true)
  {
    auto pLeftOprd  = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
    auto pRightOprd = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

    if (pLeftOprd != nullptr && pRightOprd != nullptr)
    {
      auto pExpr = Expr::MakeAssign(
        pLeftOprd->Clone(),
          new OperationExpression(OperationExpression::OpXor,
            pLeftOprd,
            pRightOprd
          ));
      rInsn.SetSemantic(pExpr);
    }
  }

  return Res;
}

bool GameBoyArchitecture::Insn_Bit(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("bit");
  rInsn.Opcode() = GB_Bit;
  rInsn.Length() = 2;

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  FrstOp.Type()  = (O_IMM8 | O_NO_REF);
  FrstOp.Value() = (Opcode >> 3) & 0x07;

  ScdOp.Type() = O_REG;
  if ((Opcode & 0x0F) == 0x06 || (Opcode & 0x0F) == 0x0E)
    ScdOp.Type() |= O_MEM;
  ScdOp.Reg() = GetRegisterByOpcode(Opcode);

  rInsn.SetClearedFlags(GB_FlNf);
  rInsn.SetFixedFlags(GB_FlHf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  auto pOprdExpr = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

  if (pOprdExpr != nullptr)
  {
    /* zf = ((op₀ >> op₁) & 1) */
    auto pExpr = Expr::MakeAssign(
      new IdentifierExpression(GB_FlZf, &m_CpuInfo),
      new OperationExpression(
        OperationExpression::OpAnd,
        new OperationExpression(
          OperationExpression::OpLls,
          pOprdExpr,
          new ConstantExpression(ConstantExpression::Const8Bit, FrstOp.GetValue())),
        new ConstantExpression(0, 1)));
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Set(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("set");
  rInsn.Opcode() = GB_Set;
  rInsn.Length() = 2;

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  FrstOp.Type()  = (O_IMM8 | O_NO_REF);
  FrstOp.Value() = (Opcode >> 3) & 0x07;
  ScdOp.Type()   = O_REG;

  if ((Opcode & 0x0F) == 0x06 || (Opcode & 0x0F) == 0x0E)
    ScdOp.Type() |= O_MEM;
  ScdOp.Reg() = GetRegisterByOpcode(Opcode);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  auto pOprdExpr = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr != nullptr)
  {
    /* op₀ = op₀ | (1 << op₁) */
    auto pExpr = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(
        OperationExpression::OpOr,
        pOprdExpr,
        new ConstantExpression(ConstantExpression::Const8Bit, 1 << FrstOp.GetValue())));
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Res(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("res");
  rInsn.Opcode() = GB_Res;
  rInsn.Length() = 2;

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  FrstOp.Type()  = (O_IMM8 | O_NO_REF);
  FrstOp.Value() = (Opcode >> 3) & 0x07;
  ScdOp.Type()   = O_REG;

  if ((Opcode & 0x0F) == 0x06 || (Opcode & 0x0F) == 0x0E)
    ScdOp.Type() |= O_MEM;
  ScdOp.Reg() = GetRegisterByOpcode(Opcode);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  auto pOprdExpr = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr != nullptr)
  {
    /* op₀ = op₀ & ((1 << op₁) ^ -1) */
    auto pExpr = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(
        OperationExpression::OpAnd,
        pOprdExpr,
        new ConstantExpression(ConstantExpression::Const8Bit, ~(1 << FrstOp.GetValue()))));
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Rl(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  rInsn.SetName("rl");
  rInsn.Opcode() = GB_Rl;
  rInsn.Length() = 2;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  rInsn.SetClearedFlags(GB_FlNf | GB_FlHf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  auto pOprdExpr = Op.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

  if (pOprdExpr != nullptr)
  {
    /* Op₀ = (Op₀ << 1) | cf */
    auto pExpr = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(
        OperationExpression::OpOr,
        new OperationExpression(
          OperationExpression::OpLls,
          pOprdExpr,
          new ConstantExpression(ConstantExpression::Const8Bit, 1)),
        new IdentifierExpression(GB_FlCf, &m_CpuInfo)));
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Rr(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  rInsn.SetName("rr");
  rInsn.Opcode() = GB_Rr;
  rInsn.Length() = 2;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  rInsn.SetClearedFlags(GB_FlNf | GB_FlHf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pOprdExpr = Op.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

  if (pOprdExpr != nullptr)
  {
    /* Op₀ = (Op₀ >> 1) | (cf << 7) */
    auto pExpr = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(
        OperationExpression::OpOr,
        new OperationExpression(
          OperationExpression::OpLrs,
          pOprdExpr,
          new ConstantExpression(ConstantExpression::Const8Bit, 1)),
        new OperationExpression(
          OperationExpression::OpLrs,
          new IdentifierExpression(GB_FlCf, &m_CpuInfo),
          new ConstantExpression(ConstantExpression::Const8Bit, 7))));
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Rlc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  rInsn.SetName("rlc");
  rInsn.Opcode() = GB_Rlc;
  rInsn.Length() = 2;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  rInsn.SetClearedFlags(GB_FlNf | GB_FlHf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pOprdExpr = Op.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

  if (pOprdExpr != nullptr)
  {
    /* cf = (Op₀ & 0x80) >> 7; Op₀ = (Op₀ << 1) | cf */
    auto pExprCarry = Expr::MakeAssign(
      new IdentifierExpression(GB_FlCf, &m_CpuInfo),
      new OperationExpression(
        OperationExpression::OpLrs,
        new OperationExpression(
          OperationExpression::OpAnd,
          pOprdExpr->Clone(),
          new ConstantExpression(ConstantExpression::Const8Bit, 0x80)),
        new ConstantExpression(ConstantExpression::Const8Bit, 7)));

    auto pExprShift = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(
        OperationExpression::OpOr,
        new OperationExpression(
          OperationExpression::OpLls,
          pOprdExpr,
          new ConstantExpression(ConstantExpression::Const8Bit, 1)),
        new IdentifierExpression(GB_FlCf, &m_CpuInfo)));

    Expression::List ExprList;
    ExprList.push_back(pExprCarry);
    ExprList.push_back(pExprShift);
    rInsn.SetSemantic(ExprList);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Rrc(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  rInsn.SetName("rrc");
  rInsn.Opcode() = GB_Rrc;
  rInsn.Length() = 2;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  rInsn.SetClearedFlags(GB_FlNf | GB_FlHf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pOprdExpr = Op.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

  if (pOprdExpr != nullptr)
  {
    /* cf = Op₀ & 0x01; Op₀ = (Op₀ >> 1) | (cf << 7) */
    auto pExprCarry = Expr::MakeAssign(
      new IdentifierExpression(GB_FlCf, &m_CpuInfo),
      new OperationExpression(
        OperationExpression::OpAnd,
        pOprdExpr->Clone(),
        new ConstantExpression(ConstantExpression::Const8Bit, 1)));

    auto pExprShift = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(
        OperationExpression::OpOr,
        new OperationExpression(
          OperationExpression::OpLrs,
          pOprdExpr,
          new ConstantExpression(ConstantExpression::Const8Bit, 1)),
        new OperationExpression(
          OperationExpression::OpLls,
          new IdentifierExpression(GB_FlCf, &m_CpuInfo),
          new ConstantExpression(ConstantExpression::Const8Bit, 7))));

    Expression::List ExprList;
    ExprList.push_back(pExprCarry);
    ExprList.push_back(pExprShift);
    rInsn.SetSemantic(ExprList);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Sla(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  rInsn.SetName("sla");
  rInsn.Opcode() = GB_Sla;
  rInsn.Length() = 2;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  rInsn.SetClearedFlags(GB_FlNf | GB_FlHf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pOprdExpr = Op.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr != nullptr)
  {
    auto pCarryExpr = Expr::MakeAssign(
      new IdentifierExpression(GB_FlCf, &m_CpuInfo),
      new OperationExpression(
        OperationExpression::OpAnd,
        new OperationExpression(
          OperationExpression::OpLrs,
          pOprdExpr->Clone(),
          new ConstantExpression(ConstantExpression::Const8Bit, 7)),
      new ConstantExpression(ConstantExpression::Const8Bit, 1)));
    auto pShiftExpr = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(
        OperationExpression::OpLls,
        pOprdExpr,
        new ConstantExpression(0, 1)));

    Expression::List ExprList;
    ExprList.push_back(pCarryExpr);
    ExprList.push_back(pShiftExpr);
    rInsn.SetSemantic(ExprList);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Sra(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  rInsn.SetName("sra");
  rInsn.Opcode() = GB_Sra;
  rInsn.Length() = 2;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  rInsn.SetClearedFlags(GB_FlHf | GB_FlNf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pOprdExpr = Op.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr != nullptr)
  {
    /* cf = Op₀ & 0x01; Op₀ = (Op₀ >> 1) | (Op₀ & 0x80) */
    auto pCarryExpr = Expr::MakeAssign(
      new IdentifierExpression(GB_FlCf, &m_CpuInfo),
      new OperationExpression(
        OperationExpression::OpAnd,
        pOprdExpr->Clone(),
        new ConstantExpression(ConstantExpression::Const8Bit, 1)));
    auto pShiftExpr = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(
        OperationExpression::OpOr,
          new OperationExpression(
            OperationExpression::OpLrs,
            pOprdExpr->Clone(),
            new ConstantExpression(ConstantExpression::Const8Bit, 1)),
          new OperationExpression(
          OperationExpression::OpAnd,
            pOprdExpr,
            new ConstantExpression(ConstantExpression::Const8Bit, 0x80))));
    
    Expression::List ExprList;
    ExprList.push_back(pCarryExpr);
    ExprList.push_back(pShiftExpr);
    rInsn.SetSemantic(ExprList);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Srl(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  rInsn.SetName("srl");
  rInsn.Opcode() = GB_Srl;
  rInsn.Length() = 2;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  rInsn.SetUpdatedFlags(GB_FlCf | GB_FlZf);
  rInsn.SetClearedFlags(GB_FlNf | GB_FlHf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pOprdExpr = Op.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr != nullptr)
  {
    auto pExprCf = Expr::MakeAssign(
      new IdentifierExpression(GB_FlCf, &m_CpuInfo),
      new OperationExpression(
      OperationExpression::OpAnd,
      pOprdExpr->Clone(),
      new ConstantExpression(0, 1)));

    auto pShiftExpr = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(
        OperationExpression::OpLrs,
        pOprdExpr->Clone(),
        new ConstantExpression(0, 1)));

    auto pExprZf = new IfElseConditionExpression(
      /* If */
      ConditionExpression::CondEq,
      pOprdExpr,
      new ConstantExpression(pOprdExpr->GetSizeInBit(), 0),
      /* Then */
      Expr::MakeAssign(
        new IdentifierExpression(GB_FlZf, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const1Bit, 1)),
      /* Else */
      Expr::MakeAssign(
        new IdentifierExpression(GB_FlZf, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const1Bit, 0)));

    Expression::List ExprList;
    ExprList.push_back(pExprCf);
    ExprList.push_back(pShiftExpr);
    ExprList.push_back(pExprZf);
    rInsn.SetSemantic(ExprList);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Swap(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  rInsn.SetName("swap");
  rInsn.Opcode() = GB_Swap;
  rInsn.Length() = 2;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  rInsn.SetUpdatedFlags(GB_FlZf);
  rInsn.SetClearedFlags(GB_FlNf | GB_FlHf | GB_FlCf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  // op = (op >> 4) | (op << 4)
  auto pOprdExpr = Op.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr != nullptr)
  {
    auto pExpr = Expr::MakeAssign(
      pOprdExpr->Clone(),
      new OperationExpression(
        OperationExpression::OpOr,
        new OperationExpression(
          OperationExpression::OpLls,
          pOprdExpr->Clone(),
          new ConstantExpression(ConstantExpression::Const8Bit, 4)),
        new OperationExpression(
          OperationExpression::OpLrs,
          pOprdExpr,
          new ConstantExpression(ConstantExpression::Const8Bit, 4))));
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Cpl(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("cpl");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Cpl;

  rInsn.SetFixedFlags(GB_FlNf | GB_FlHf);

  // ~A
  auto pExpr = Expr::MakeAssign(
    new IdentifierExpression(GB_RegA, &m_CpuInfo),
    new OperationExpression(
      OperationExpression::OpXor,
      new IdentifierExpression(GB_RegA, &m_CpuInfo),
      new ConstantExpression(ConstantExpression::Const8Bit, ~0)));
  rInsn.SetSemantic(pExpr);

  return true;
}

bool GameBoyArchitecture::Insn_Ccf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("ccf");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Ccf;

  rInsn.SetClearedFlags(GB_FlHf | GB_FlNf);
  rInsn.SetUpdatedFlags(GB_FlCf);

  /* cf ^= 1 */
  auto pExpr = Expr::MakeAssign(
    new IdentifierExpression(GB_FlCf, &m_CpuInfo),
    new OperationExpression(
      OperationExpression::OpXor,
      new IdentifierExpression(GB_FlCf, &m_CpuInfo),
      new ConstantExpression(ConstantExpression::Const1Bit, 1)));
  rInsn.SetSemantic(pExpr);
  return true;
}

bool GameBoyArchitecture::Insn_Scf(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("scf");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Scf;

  rInsn.SetFixedFlags(GB_FlCf);
  rInsn.SetClearedFlags(GB_FlHf | GB_FlNf);
  return true;
}

bool GameBoyArchitecture::Insn_Cp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  bool Res = false;
  u8 Opcode;

  rBinStrm.Read(Offset, Opcode);

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  rInsn.SetName("cp");
  rInsn.Opcode()  = GB_Cp;

  FrstOp.Type()   = O_REG;
  FrstOp.Reg()    = GB_RegA;

  // A, Reg8
  if (Opcode >= 0xB8 && Opcode <= 0xBF)
  {
    ScdOp.Type() = O_REG;

    // A, (HL)
    if (Opcode == 0xBE)
      ScdOp.Type() |= O_MEM;
    ScdOp.Reg() = GetRegisterByOpcode(Opcode);

    rInsn.Length() = 1;
    Res = true;
  }

  // A, n
  else if (Opcode == 0xFE)
  {
    FrstOp.Reg()   = GB_RegA;

    u8 Data;
    ScdOp.Type()   = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value()  = Data;

    rInsn.Length() = 2;
    Res = true;
  }

  rInsn.SetUpdatedFlags(GB_FlZf | GB_FlHf | GB_FlCf);
  rInsn.SetFixedFlags(GB_FlNf);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  auto pFstOprdExpr = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  auto pScdOprdExpr = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

  if (pFstOprdExpr != nullptr && pScdOprdExpr != nullptr)
  {
    auto pExpr = new IfElseConditionExpression(
      ConditionExpression::CondEq,
      pFstOprdExpr,
      pScdOprdExpr,
      Expr::MakeAssign(
      new IdentifierExpression(GB_FlCf, &m_CpuInfo),
      new ConstantExpression(ConstantExpression::Const1Bit, 1)), nullptr);
    rInsn.SetSemantic(pExpr);
  }

  return Res;
}

bool GameBoyArchitecture::Insn_Ld(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8  Opcode;
  u32 O1Type    = O_NONE,         O2Type      = O_NONE;
  u16 O1Reg     = GB_Invalid_Reg, O2Reg       = GB_Invalid_Reg;

  rInsn.SetName("ld");
  rInsn.Opcode() = GB_Ld;

  rBinStrm.Read(Offset, Opcode);

  switch (Opcode)
  {
  // Reg8, n
  case 0x06: case 0x0E:
  case 0x16: case 0x1E:
  case 0x26: case 0x2E:
  case 0x36: case 0x3E:
    O1Type = O_REG;
    O1Reg  = GetRegisterByOpcode((Opcode >> 3) & 0x07);
    O2Type = (O_IMM8 | O_NO_REF);
    break;

  // BC, nn
  case 0x01:
    O1Type = O_REG;
    O1Reg  = GB_RegBC;
    O2Type = O_IMM16;
    break;

  // (BC), A
  case 0x02:
    O1Type = O_REG | O_MEM;
    O1Reg  = GB_RegBC;
    O2Type = O_REG;
    O2Reg  = GB_RegA;
    break;

  // DE, nn
  case 0x11:
    O1Type = O_REG;
    O1Reg  = GB_RegDE;
    O2Type = O_IMM16;
    break;

  // (DE), A
  case 0x12:
    O1Type = O_REG | O_MEM;
    O1Reg  = GB_RegDE;
    O2Type = O_REG;
    O2Reg  = GB_RegA;
    break;

  // HL, nn
  case 0x21:
    O1Type = O_REG;
    O1Reg  = GB_RegHL;
    O2Type = O_IMM16;
    break;

  // SP, nn
  case 0x31:
    O1Type = O_REG;
    O1Reg  = GB_RegSp;
    O2Type = O_IMM16;
    break;

  // (nn), SP
  case 0x08:
    O1Type = O_IMM16 | O_MEM;
    O2Type = O_REG;
    O2Reg  = GB_RegSp;
    break;

  // A, (BC)
  case 0x0A:
    O1Type = O_REG;
    O1Reg  = GB_RegA;
    O2Type = O_REG | O_MEM;
    O2Reg  = GB_RegBC;
    break;

  // A, (DE)
  case 0x1A:
    O1Type = O_REG;
    O1Reg  = GB_RegA;
    O2Type = O_REG | O_MEM;
    O2Reg  = GB_RegDE;
    break;

  // Reg8, Reg8

  //   A           B           C           D           E           H           L           (HL)
  case 0x7F:  case 0x78:  case 0x79:  case 0x7A:  case 0x7B:  case 0x7C:  case 0x7D:  case 0x7E:  // A, Reg8
  case 0x47:  case 0x40:  case 0x41:  case 0x42:  case 0x43:  case 0x44:  case 0x45:  case 0x46:  // B, Reg8
  case 0x4F:  case 0x48:  case 0x49:  case 0x4A:  case 0x4B:  case 0x4C:  case 0x4D:  case 0x4E:  // C, Reg8
  case 0x57:  case 0x50:  case 0x51:  case 0x52:  case 0x53:  case 0x54:  case 0x55:  case 0x56:  // D, Reg8
  case 0x5F:  case 0x58:  case 0x59:  case 0x5A:  case 0x5B:  case 0x5C:  case 0x5D:  case 0x5E:  // E, Reg8
  case 0x67:  case 0x60:  case 0x61:  case 0x62:  case 0x63:  case 0x64:  case 0x65:  case 0x66:  // H, Reg8
  case 0x6F:  case 0x68:  case 0x69:  case 0x6A:  case 0x6B:  case 0x6C:  case 0x6D:  case 0x6E:  // L, Reg8
  case 0x77:  case 0x70:  case 0x71:  case 0x72:  case 0x73:  case 0x74:  case 0x75:              // (HL), Reg8
    O1Type = O_REG;
    O1Reg  = GetRegisterByOpcode(((Opcode >> 3) - 0x08) & 0x07);
    O2Type = O_REG;
    O2Reg  = GetRegisterByOpcode(Opcode & 0x0F);
    if (O1Reg == GB_RegHL)
      O1Type |= O_MEM;
    else if (O2Reg == GB_RegHL)
      O2Type |= O_MEM;
    break;

  // (nn), A
  case 0xEA:
    O1Type = O_IMM16 | O_MEM;
    O2Type = O_REG;
    O2Reg  = GB_RegA;
    break;

  case 0xF9:
    O1Type = O_REG16;
    O2Type = O_REG16;
    O1Reg  = GB_RegSp;
    O2Reg  = GB_RegHL;

  // A, (nn)
  case 0xFA:
    O1Type = O_REG;
    O1Reg  = GB_RegA;
    O2Type = O_IMM16 | O_MEM;
    break;

  default: return false;
  }

  Operand& FstOp = rInsn.FirstOperand();
  Operand& ScdOp = rInsn.SecondOperand();

  u8 InsnLen = 1;

  if ((O1Type & DS_MASK) == DS_8BIT)
  {
    u8 Data;
    rBinStrm.Read(Offset + 1, Data);
    FstOp.Value() = Data;
    InsnLen++;
  }
  else if ((O1Type & DS_MASK) == DS_16BIT)
  {
    u16 Data;
    rBinStrm.Read(Offset + 1, Data);
    FstOp.Value() = Data;
    InsnLen += 2;
  }

  if ((O2Type & DS_MASK) == DS_8BIT)
  {
    u8 Data;
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value() = Data;
    InsnLen++;
  }
  else if ((O2Type & DS_MASK) == DS_16BIT)
  {
    u16 Data;
    rBinStrm.Read(Offset + 1, Data);
    ScdOp.Value() = Data;
    InsnLen += 2;
  }

  rInsn.Length() = InsnLen;

  FstOp.Type()   = O1Type;
  FstOp.Reg()    = O1Reg;

  ScdOp.Type()   = O2Type;
  ScdOp.Reg()    = O2Reg;

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  auto pLeftOprd  = FstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  auto pRightOprd = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

  if (pLeftOprd != nullptr && pRightOprd != nullptr)
  {
    auto pExpr = Expr::MakeAssign(
      pLeftOprd,
      pRightOprd);
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Ldi(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("ldi");
  rInsn.Opcode()  = GB_Ldi;
  rInsn.Length()  = 1;

  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  FrstOp.Type()   = O_REG;
  ScdOp.Type()    = O_REG;

  // (HL), A
  if (Opcode == 0x22)
  {
    FrstOp.Type() |= O_MEM;
    FrstOp.Reg()   = GB_RegHL;
    ScdOp.Reg()    = GB_RegA;
  }

  // A, (HL)
  else if (Opcode == 0x2A)
  {
    FrstOp.Reg()  = GB_RegA;
    ScdOp.Type() |= O_MEM;
    ScdOp.Reg()   = GB_RegHL;
  }

  else return false;

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  auto pLeftOprd  = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  auto pRightOprd = dynamic_cast<MemoryExpression *>(ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo));

  if (pRightOprd != nullptr)
  {
    auto pRightOprdAddr = pRightOprd->GetAddressExpression();

    if (pLeftOprd != nullptr && pRightOprd != nullptr && pRightOprdAddr != nullptr)
    {
      auto pExpr = Expr::MakeAssign(
        pLeftOprd,
        pRightOprd);

      auto pIncExpr = Expr::MakeAssign(
        pRightOprdAddr->Clone(),
        new OperationExpression(
        OperationExpression::OpAdd,
        pRightOprdAddr->Clone(),
        new ConstantExpression(pRightOprdAddr->GetSizeInBit(), 1)));

      Expression::List ExprList;
      ExprList.push_back(pExpr);
      ExprList.push_back(pIncExpr);
      rInsn.SetSemantic(ExprList);
    }
  }

  return true;
}

bool GameBoyArchitecture::Insn_Ldd(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("ldd");
  rInsn.Opcode()  = GB_Ldd;
  rInsn.Length()  = 1;

  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  FrstOp.Type()   = O_REG;
  ScdOp.Type()    = O_REG;

  // (HL), A
  if (Opcode == 0x32)
  {
    FrstOp.Type() |= O_MEM;
    FrstOp.Reg()   = GB_RegHL;
    ScdOp.Reg()    = GB_RegA;
  }

  // A, (HL)
  else if (Opcode == 0x3A)
  {
    FrstOp.Reg()  = GB_RegA;
    ScdOp.Type() |= O_MEM;
    ScdOp.Reg()   = GB_RegHL;
  }

  else return false;

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  auto pLeftOprd  = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  auto pRightOprd = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  auto pRightOprdAddr = dynamic_cast<MemoryExpression const *>(pRightOprd);

  if (pLeftOprd != nullptr && pRightOprd != nullptr && pRightOprdAddr != nullptr)
  {
    auto pExpr = Expr::MakeAssign(
      pLeftOprd,
      pRightOprd);

    auto pIncExpr = new OperationExpression(
      OperationExpression::OpSub,
      pRightOprdAddr->Clone(),
      new ConstantExpression(pRightOprdAddr->GetSizeInBit(), 1));

    Expression::List ExprList;
    ExprList.push_back(pExpr);
    ExprList.push_back(pIncExpr);
    rInsn.SetSemantic(ExprList);
  }
  else
  {
    delete pLeftOprd;
    delete pRightOprd;
  }

  return true;
}

bool GameBoyArchitecture::Insn_Ldh(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("ldh");
  rInsn.Opcode()  = GB_Ldh;

  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  u8 Data;
  rBinStrm.Read(Offset + 1, Data);

  rInsn.Length()  = Opcode & 0x0f ? 1 : 2;
  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  switch (Opcode)
  {
  // (n), A
  case 0xE0:
    FrstOp.Type() |= O_MEM | O_IMM16;
    FrstOp.Value() = 0xFF00 + Data;
    ScdOp.Type()   = O_REG;
    ScdOp.Reg()    = GB_RegA;
    break;

  // A, (n)
  case 0xF0:
    FrstOp.Type() = O_REG;
    FrstOp.Reg()  = GB_RegA;
    ScdOp.Type() |= O_MEM | O_IMM16;
    ScdOp.Value() = 0xFF00 + Data;
    break;

  // (C), A
  case 0xE2:
    FrstOp.Type()  = O_MEM | O_REG;
    FrstOp.Reg()   = GB_RegC;
    ScdOp.Type()   = O_REG;
    ScdOp.Reg()    = GB_RegA;
    break;

  // A, (C)
  case 0xF2:
    FrstOp.Type() = O_REG;
    FrstOp.Reg()  = GB_RegA;
    ScdOp.Type()  = O_MEM | O_REG;
    ScdOp.Reg()   = GB_RegC;
    break;
  }

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  auto pLeftOprd  = FrstOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  auto pRightOprd = ScdOp.GetSemantic(rInsn.GetMode(), &m_CpuInfo);

  if (pLeftOprd != nullptr && pRightOprd != nullptr)
  {
    auto pExpr = Expr::MakeAssign(
      pLeftOprd,
      pRightOprd);
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Ldhl(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("ld");
  rInsn.Length()               = 2;
  rInsn.Opcode()               = GB_Ldhl;

  rInsn.FirstOperand().Type()  = O_REG;
  rInsn.FirstOperand().Reg()   = GB_RegHL;
  rInsn.SecondOperand().Type() = O_REG;
  rInsn.SecondOperand().Reg()  = GB_RegSp;
  s8 Immediate;
  rBinStrm.Read(Offset + 1, Immediate);
  rInsn.ThirdOperand().Type()  = O_IMM8;
  rInsn.ThirdOperand().Value() = Immediate;

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  //FIXME: FormatOperand(rInsn.SecondOperand(), Offset);
  //FIXME: FormatOperand(rInsn.ThirdOperand(),  Offset);
  auto pLeftOprd  = rInsn.FirstOperand().GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  auto pRightOprd = rInsn.SecondOperand().GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  auto pImmOprd   = rInsn.ThirdOperand().GetSemantic(rInsn.GetMode(), &m_CpuInfo);

  if (pLeftOprd != nullptr && pRightOprd != nullptr)
  {
    auto pExpr = Expr::MakeAssign(
      pLeftOprd,
      new OperationExpression(
        OperationExpression::OpAdd,
        pRightOprd,
        pImmOprd));
    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Push(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  u16 Reg;

  rBinStrm.Read(Offset, Opcode);

  rInsn.SetName("push");
  rInsn.Opcode() = GB_Push;
  rInsn.Length() = 1;

  switch (Opcode)
  {
  case 0xF5: Reg = GB_RegAF; break;
  case 0xC5: Reg = GB_RegBC; break;
  case 0xD5: Reg = GB_RegDE; break;
  case 0xE5: Reg = GB_RegHL; break;
  default:   Reg = GB_Invalid_Reg; break;
  }

  rInsn.FirstOperand().Reg()  = Reg;
  rInsn.FirstOperand().Type() = O_REG;

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pExprOprd = rInsn.FirstOperand().GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pExprOprd != nullptr)
  {
    auto pAllocStack = Expr::MakeAssign(
      new IdentifierExpression(GB_RegSp, &m_CpuInfo),
      new OperationExpression(
        OperationExpression::OpSub,
        new IdentifierExpression(GB_RegSp, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const16Bit, 2)));

    auto pStoreStack = Expr::MakeAssign(
      new MemoryExpression(16, nullptr, new IdentifierExpression(GB_RegSp, &m_CpuInfo)),
      pExprOprd);

    Expression::List ExprList;
    ExprList.push_back(pAllocStack);
    ExprList.push_back(pStoreStack);
    rInsn.SetSemantic(ExprList);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Pop(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  u16 Reg;

  rBinStrm.Read(Offset, Opcode);

  rInsn.SetName("pop");
  rInsn.Opcode() = GB_Pop;
  rInsn.Length() = 1;

  switch (Opcode)
  {
  case 0xF1: Reg = GB_RegAF; break;
  case 0xC1: Reg = GB_RegBC; break;
  case 0xD1: Reg = GB_RegDE; break;
  case 0xE1: Reg = GB_RegHL; break;
  default:   Reg = GB_Invalid_Reg; break;
  }

  rInsn.FirstOperand().Reg()  = Reg;
  rInsn.FirstOperand().Type() = O_REG;

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pOprdExpr = rInsn.FirstOperand().GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr != nullptr)
  {
    auto pStoreOprd = Expr::MakeAssign(
      pOprdExpr,
      new MemoryExpression(16, nullptr, new IdentifierExpression(GB_RegSp, &m_CpuInfo)));

    auto pFreeStack = Expr::MakeAssign(
      new IdentifierExpression(GB_RegSp, &m_CpuInfo),
      new OperationExpression(
        OperationExpression::OpAdd,
        new IdentifierExpression(GB_RegSp, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const16Bit, 2)));

    Expression::List ExprList;
    ExprList.push_back(pStoreOprd);
    ExprList.push_back(pFreeStack);
    rInsn.SetSemantic(ExprList);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Jr(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  s8 Relative;
  rBinStrm.Read(Offset, Opcode);
  rBinStrm.Read(Offset + 1, Relative);

  rInsn.Length()  = 2;
  rInsn.Opcode()  = GB_Jr;
  rInsn.SubType() = Instruction::JumpType;

  rInsn.FirstOperand().Type() = O_REL;
  rInsn.FirstOperand().Value() = Relative;

  u32 Flags = 0;
  bool Equal = true;

  switch (Opcode)
  {
  case 0x18:                                                                               rInsn.SetName("jr");    break;
  case 0x20: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlZf; Equal = false; rInsn.SetName("jr nz"); break;
  case 0x28: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlZf;                rInsn.SetName("jr z");  break;
  case 0x30: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlCf; Equal = false; rInsn.SetName("jr nc"); break;
  case 0x38: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlCf;                rInsn.SetName("jr c");  break;
  default: return false;
  }

  //FIXME: FormatOperand(rInsn.FirstOperand(), Offset);
  auto pOprdExpr = rInsn.FirstOperand().GetSemantic(rInsn.GetMode(), &m_CpuInfo, static_cast<u8>(rInsn.GetLength()));
  if (pOprdExpr != nullptr)
  {
    Expression *pExpr = Expr::MakeAssign(
      new IdentifierExpression(GB_RegPc, &m_CpuInfo),
      pOprdExpr);

    if (Flags != 0)
      pExpr = new IfElseConditionExpression(
        Equal == true ? ConditionExpression::CondEq : ConditionExpression::CondNe,
        new IdentifierExpression(Flags, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const1Bit, 1),
        pExpr, nullptr);

    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Jp(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  rInsn.Opcode()  = GB_Jp;
  rInsn.SubType() = Instruction::JumpType;

  // (HL)
  if (Opcode == 0xE9)
  {
    rInsn.SetName("jp");
    rInsn.Length() = 1;
    rInsn.FirstOperand().Type() = O_REG;
    rInsn.FirstOperand().Reg()  = GB_RegHL;
    return true;
  }

  u16 Absolute;
  rBinStrm.Read(Offset + 1, Absolute);

  rInsn.Length()               = 3;
  rInsn.FirstOperand().Type()  = O_ABS16;
  rInsn.FirstOperand().Value() = Absolute;

  u32 Flags = 0;
  bool Equal = true;

  switch (Opcode)
  {
  case 0xC3:                                                                                  rInsn.SetName("jp");    break;
  case 0xC2: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlZf; Equal = false; rInsn.SetName("jp nz"); break;
  case 0xCA: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlZf;                rInsn.SetName("jp z");  break;
  case 0xD2: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlCf; Equal = false; rInsn.SetName("jp nc"); break;
  case 0xDA: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlCf;                rInsn.SetName("jp c");  break;
  default: return false;
  }

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  auto pOprdExpr = rInsn.FirstOperand().GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr)
  {
    Expression *pExpr = new AssignmentExpression(
      new IdentifierExpression(GB_RegPc, &m_CpuInfo),
      pOprdExpr);

    if (Flags != 0)
      pExpr = new IfElseConditionExpression(
        Equal == true ? ConditionExpression::CondEq : ConditionExpression::CondNe,
        new IdentifierExpression(Flags, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const1Bit, 1),
        pExpr, nullptr);

    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Call(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  u16 Absolute;
  rBinStrm.Read(Offset, Opcode);
  rBinStrm.Read(Offset + 1, Absolute);

  rInsn.Length()  = 3;
  rInsn.Opcode()  = GB_Call;
  rInsn.SubType() = Instruction::CallType;

  rInsn.FirstOperand().Type()  = O_ABS16;
  rInsn.FirstOperand().Value() = Absolute;

  u32  Flags = 0;
  bool Equal = true;

  switch (Opcode)
  {
  case 0xCD:                                                                                  rInsn.SetName("call");    break;
  case 0xC4: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlZf; Equal = false; rInsn.SetName("call nz"); break;
  case 0xCC: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlZf;                rInsn.SetName("call z");  break;
  case 0xD4: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlCf; Equal = false; rInsn.SetName("call nc"); break;
  case 0xDC: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlCf;                rInsn.SetName("call c");  break;
  default: return false;
  }

  rInsn.SetUpdatedFlags(Flags);

  //FIXME: FormatOperand(rInsn.FirstOperand(),  Offset);
  /* CALL → sp -= 2 ; ld (sp), pc + insn_len ; pc = oprd */
  auto pOprdExpr = rInsn.FirstOperand().GetSemantic(rInsn.GetMode(), &m_CpuInfo);
  if (pOprdExpr != nullptr)
  {
    auto pExprAllocStack = Expr::MakeAssign(
      new IdentifierExpression(GB_RegSp, &m_CpuInfo),
      new OperationExpression(
        OperationExpression::OpSub,
        new IdentifierExpression(GB_RegSp, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const16Bit, 2)));

    auto pExprStoreStack = Expr::MakeAssign(
      new MemoryExpression(16, nullptr, new IdentifierExpression(GB_RegSp, &m_CpuInfo)),
      new OperationExpression(
        OperationExpression::OpAdd,
        new IdentifierExpression(GB_RegPc, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const16Bit, rInsn.GetLength())));

    auto pExprJmp = new AssignmentExpression(
      new IdentifierExpression(GB_RegPc, &m_CpuInfo),
      pOprdExpr);

    Expression::List ExprList;
    ExprList.push_back(pExprAllocStack);
    ExprList.push_back(pExprStoreStack);
    ExprList.push_back(pExprJmp);
    Expression *pExpr = new BindExpression(ExprList);

    if (Flags != 0)
      pExpr = new IfElseConditionExpression(
        Equal == true ? ConditionExpression::CondEq : ConditionExpression::CondNe,
        new IdentifierExpression(Flags, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const1Bit, 1),
        pExpr, nullptr);

    rInsn.SetSemantic(pExpr);
  }

  return true;
}

bool GameBoyArchitecture::Insn_Rst(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("rst");
  rInsn.Opcode() = GB_Rst;
  rInsn.Length() = 1;

  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  rInsn.FirstOperand().Type()  = (O_IMM8 | O_NO_REF);
  rInsn.FirstOperand().Value() = ((Opcode >> 3) & 0x07) * 0x08;

  auto pExprAllocStack = new AssignmentExpression(
      new IdentifierExpression(GB_RegSp, &m_CpuInfo),
      new OperationExpression(
        OperationExpression::OpSub,
        new IdentifierExpression(GB_RegSp, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const16Bit, 2)));

  auto pExprStoreStack = new AssignmentExpression(
      new MemoryExpression(16, nullptr, new IdentifierExpression(GB_RegSp, &m_CpuInfo)),
      new OperationExpression(
        OperationExpression::OpAdd,
        new IdentifierExpression(GB_RegPc, &m_CpuInfo),
        new ConstantExpression(ConstantExpression::Const16Bit, rInsn.GetLength())));

  auto pExprJmp = new AssignmentExpression(
      new IdentifierExpression(GB_RegPc, &m_CpuInfo),
      new ConstantExpression(ConstantExpression::Const16Bit, rInsn.FirstOperand().GetValue()));

    Expression::List ExprList;
    ExprList.push_back(pExprAllocStack);
    ExprList.push_back(pExprStoreStack);
    ExprList.push_back(pExprJmp);
    rInsn.SetSemantic(ExprList);

  return true;
}

bool GameBoyArchitecture::Insn_Ret(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.Opcode()        = GB_Ret;
  rInsn.Length()        = 1;
  rInsn.SubType() = Instruction::ReturnType;

  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);

  u32 Flags = 0;
  bool Equal = true;

  switch (Opcode)
  {
  case 0xC9:                                                                               rInsn.SetName("ret");    break;
  case 0xC0: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlZf; Equal = false; rInsn.SetName("ret nz"); break;
  case 0xC8: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlZf;                rInsn.SetName("ret z");  break;
  case 0xD0: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlCf; Equal = false; rInsn.SetName("ret nc"); break;
  case 0xD8: rInsn.SubType() |= Instruction::ConditionalType; Flags = GB_FlCf;                rInsn.SetName("ret c");  break;
  default: return false;
  }

  auto pLoadStack = new AssignmentExpression(
    new IdentifierExpression(GB_RegPc, &m_CpuInfo),
    new MemoryExpression(16, nullptr, new IdentifierExpression(GB_RegSp, &m_CpuInfo)));

  auto pFreeStack = new AssignmentExpression(
    new IdentifierExpression(GB_RegSp, &m_CpuInfo),
    new OperationExpression(
      OperationExpression::OpAdd,
      new IdentifierExpression(GB_RegSp, &m_CpuInfo),
      new ConstantExpression(ConstantExpression::Const16Bit, 2)));

  Expression::List ExprList;
  ExprList.push_back(pLoadStack);
  ExprList.push_back(pFreeStack);
  Expression *pExpr = new BindExpression(ExprList);

  if (Flags != 0)
    pExpr = new IfElseConditionExpression(
    Equal == true ? ConditionExpression::CondEq : ConditionExpression::CondNe,
    new IdentifierExpression(Flags, &m_CpuInfo),
    new ConstantExpression(ConstantExpression::Const1Bit, 1),
    pExpr, nullptr);

  rInsn.SetSemantic(pExpr);

  return true;
}

bool GameBoyArchitecture::Insn_Reti(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.SetName("reti");
  rInsn.Opcode()        = GB_Reti;
  rInsn.Length()        = 1;
  rInsn.SubType() = Instruction::ReturnType;

  auto pLoadStack = new AssignmentExpression(
    new IdentifierExpression(GB_RegPc, &m_CpuInfo),
    new MemoryExpression(16, nullptr, new IdentifierExpression(GB_RegSp, &m_CpuInfo)));

  auto pFreeStack = new AssignmentExpression(
    new IdentifierExpression(GB_RegSp, &m_CpuInfo),
    new OperationExpression(
      OperationExpression::OpAdd,
      new IdentifierExpression(GB_RegSp, &m_CpuInfo),
      new ConstantExpression(ConstantExpression::Const16Bit, 2)));

  Expression::List ExprList;
  ExprList.push_back(pLoadStack);
  ExprList.push_back(pFreeStack);
  rInsn.SetSemantic(ExprList);

  return true;
}

bool GameBoyArchitecture::Insn_Halt(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.Opcode() = GB_Halt;
  rInsn.Length() = 1;
  rInsn.SetName("halt");

  return true;
}

bool GameBoyArchitecture::Insn_Stop(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.Opcode() = GB_Stop;
  rInsn.Length() = 1;
  rInsn.SetName("stop");

  return true;
}

bool GameBoyArchitecture::Insn_Ei(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.Opcode() = GB_Ei;
  rInsn.Length() = 1;
  rInsn.SetName("ei");

  return true;
}

bool GameBoyArchitecture::Insn_Di(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  rInsn.Opcode() = GB_Di;
  rInsn.Length() = 1;
  rInsn.SetName("di");

  return true;
}
