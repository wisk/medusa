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

bool GameBoyArchitecture::Translate(Address const& rVirtAddr, TAddress& rPhysAddr)
{
  return true;
}

bool GameBoyArchitecture::Disassemble(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Address, Opcode);
  bool Result;

  if (Opcode == 0xCB)
  {
    rBinStrm.Read(Address + 1, Opcode);
    Result = (this->*m_CbPrefix[Opcode])(rBinStrm, Address + 1, rInsn);
  }
  else
    Result = (this->*m_OpcodeMap[Opcode])(rBinStrm, Address, rInsn);

  if (Result == true)
  {
    FormatOperand(rInsn.FirstOperand(),  Address);
    FormatOperand(rInsn.SecondOperand(), Address);
    FormatOperand(rInsn.ThirdOperand(),  Address);
    FormatOperand(rInsn.FourthOperand(), Address);
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

void GameBoyArchitecture::FillConfigurationModel(ConfigurationModel&)
{
}

void GameBoyArchitecture::FormatOperand(Operand& Op, TAddress Address)
{
  if (Op.Type() & O_REG)
  {
    switch (Op.Reg())
    {
    case GB_RegA: case GB_RegB: case GB_RegC: case GB_RegD:
    case GB_RegE: case GB_RegF: case GB_RegH: case GB_RegL:
      Op.Type() |= O_REG8; break;
    case GB_RegAF: case GB_RegBC: case GB_RegDE: case GB_RegHL:
    case GB_RegPc: case GB_RegSp:
      Op.Type() |= O_REG16; break;
    default: break;
    }
  }
  std::ostringstream oss;

  if (Op.Type() & O_MEM)
    oss << "[";

  if (Op.Type() & O_REG)
  {
    for (GameBoyArchitecture::TRegName const* pRegName = m_RegName;
      pRegName->m_Value != GB_Invalid_Reg; ++pRegName)
    {
      if (pRegName->m_Value == Op.Reg())
      {
        oss << pRegName->m_Name;
        break;
      }
    }
  }

  if (Op.Type() & O_REL)
    oss << std::hex << std::showpos << static_cast<u16>((Address + Op.Value()) & 0xffff);

  if (Op.Type() & O_ABS)
    oss << std::hex << std::showpos << static_cast<u16>(Op.Value() & 0xfffff);

  if (Op.Type() & O_IMM)
    oss << std::hex << Op.GetValue();

  if (Op.Type() & O_MEM)
    oss << "]";

  Op.SetName(oss.str().c_str());
}

bool GameBoyArchitecture::Insn_Invalid(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("invalid");
  rInsn.Opcode() = GB_Invalid_Insn;
  return false;
}

bool GameBoyArchitecture::Insn_Nop(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.Opcode() = GB_Nop;
  rInsn.Length() = 1;
  rInsn.SetName("nop");

  return true;
}

bool GameBoyArchitecture::Insn_Inc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u16 Reg;
  u32 Type = O_REG;
  u8  Opcode;

  rInsn.SetName("inc");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Inc;

  rBinStrm.Read(Address, Opcode);
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

  Operand& FrstOperand =  rInsn.FirstOperand();
  FrstOperand.Type()   = Type;
  FrstOperand.Reg()    = Reg;

  return true;
}

bool GameBoyArchitecture::Insn_Dec(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u16 Reg;
  u32 Type = O_REG;
  u8  Opcode;

  rInsn.SetName("dec");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Dec;

  rBinStrm.Read(Address, Opcode);
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

  return true;
}

bool GameBoyArchitecture::Insn_Add(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  rInsn.SetName("add");
  rInsn.Opcode() = GB_Add;

  FrstOp.Type() = O_REG;
  FrstOp.Reg()  = GB_RegA;
  ScdOp.Type()  = O_REG;

  rBinStrm.Read(Address, Opcode);

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
    return true;
  }

  else if (Opcode >= 0x80 && Opcode <= 0x87)
  {
    if (Opcode == 0x86)
      ScdOp.Type() |= O_MEM;
    ScdOp.Reg() = GetRegisterByOpcode(Opcode);

    rInsn.Length() = 1;
    return true;
  }

  // A, n
  else if (Opcode == 0xC6)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Address + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    return true;
  }

  return false;
}

bool GameBoyArchitecture::Insn_Sub(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  rInsn.SetName("sub");
  rInsn.Opcode() = GB_Sub;
  FrstOp.Type()  = O_REG;
  FrstOp.Reg()   = GB_RegA;

  rBinStrm.Read(Address, Opcode);

  // A, Reg8
  if (Opcode >= 0x90 && Opcode <= 0x97)
  {
    ScdOp.Type() = O_REG;

    // A, (HL)
    if (Opcode == 0x86)
      ScdOp.Type() |= O_MEM;
    ScdOp.Reg() = GetRegisterByOpcode(Opcode);

    rInsn.Length() = 1;
    return true;
  }

  // A, n
  else if (Opcode == 0xD6)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Address + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    return true;
  }

  return false;
}

bool GameBoyArchitecture::Insn_Adc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;

  rBinStrm.Read(Address, Opcode);

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
    return true;
  }

  // A, n
  else if (Opcode == 0xCE)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Address + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    return true;
  }

  return false;
}

bool GameBoyArchitecture::Insn_Sbc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;

  rBinStrm.Read(Address, Opcode);

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
    return true;
  }

  // A, n
  else if (Opcode == 0xDE)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Address + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    return true;
  }

  return false;
}

bool GameBoyArchitecture::Insn_Daa(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("daa");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Daa;
  return true;
}

bool GameBoyArchitecture::Insn_And(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;

  rBinStrm.Read(Address, Opcode);

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
    return true;
  }

  // A, n
  else if (Opcode == 0xE6)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Address + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    return true;
  }

  return false;
}

bool GameBoyArchitecture::Insn_Or(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;

  rBinStrm.Read(Address, Opcode);

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
    return true;
  }

  // A, n
  else if (Opcode == 0xF6)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Address + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    return true;
  }

  return false;
}

bool GameBoyArchitecture::Insn_Xor(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;

  rBinStrm.Read(Address, Opcode);

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
    return true;
  }

  // A, n
  else if (Opcode == 0xEE)
  {
    u8 Data;
    ScdOp.Type() = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Address + 1, Data);
    ScdOp.Value() = Data;

    rInsn.Length() = 2;
    return true;
  }

  return false;
}

bool GameBoyArchitecture::Insn_Bit(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("bit");
  rInsn.Opcode() = GB_Bit;
  rInsn.Length() = 2;

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  FrstOp.Type()  = (O_IMM | O_NO_REF);
  FrstOp.Value() = (Opcode >> 3) & 0x03;

  ScdOp.Type() = O_REG;
  if ((Opcode & 0x0F) == 0x06 || (Opcode & 0x0F) == 0x0E)
    ScdOp.Type() |= O_MEM;
  ScdOp.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Set(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("set");
  rInsn.Opcode() = GB_Set;
  rInsn.Length() = 2;

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  FrstOp.Type()  = (O_IMM | O_NO_REF);
  FrstOp.Value() = (Opcode >> 3) & 0x03;
  ScdOp.Type()   = O_REG;

  if ((Opcode & 0x0F) == 0x06 || (Opcode & 0x0F) == 0x0E)
    ScdOp.Type() |= O_MEM;
  ScdOp.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Res(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("res");
  rInsn.Opcode() = GB_Res;
  rInsn.Length() = 2;

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();

  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  FrstOp.Type()  = (O_IMM | O_NO_REF);
  FrstOp.Value() = (Opcode >> 3) & 0x03;
  ScdOp.Type()   = O_REG;

  if ((Opcode & 0x0F) == 0x06 || (Opcode & 0x0F) == 0x0E)
    ScdOp.Type() |= O_MEM;
  ScdOp.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Rl(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  rInsn.SetName("rl");
  rInsn.Opcode() = GB_Rl;
  rInsn.Length() = 1;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Rr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  rInsn.SetName("rr");
  rInsn.Opcode() = GB_Rr;
  rInsn.Length() = 1;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Rlc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  rInsn.SetName("rlc");
  rInsn.Opcode() = GB_Rlc;
  rInsn.Length() = 1;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Rrc(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  rInsn.SetName("rrc");
  rInsn.Opcode() = GB_Rrc;
  rInsn.Length() = 1;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Sla(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  rInsn.SetName("sla");
  rInsn.Opcode() = GB_Sla;
  rInsn.Length() = 1;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Sra(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  rInsn.SetName("sra");
  rInsn.Opcode() = GB_Sra;
  rInsn.Length() = 1;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Srl(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  rInsn.SetName("srl");
  rInsn.Opcode() = GB_Srl;
  rInsn.Length() = 1;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Swap(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  rInsn.SetName("swap");
  rInsn.Opcode() = GB_Swap;
  rInsn.Length() = 1;

  Operand& Op    = rInsn.FirstOperand();

  // Reg8
  Op.Type()      = O_REG;

  // (HL)
  if ((Opcode & 0x07) == 0x06)
    Op.Type() |= O_MEM;

  Op.Reg() = GetRegisterByOpcode(Opcode);

  return true;
}

bool GameBoyArchitecture::Insn_Cpl(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("cpl");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Cpl;

  return true;
}

bool GameBoyArchitecture::Insn_Ccf(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("ccf");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Ccf;
  return true;
}

bool GameBoyArchitecture::Insn_Scf(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("scf");
  rInsn.Length() = 1;
  rInsn.Opcode() = GB_Scf;
  return true;
}

bool GameBoyArchitecture::Insn_Cp(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;

  rBinStrm.Read(Address, Opcode);

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
    return true;
  }

  // A, n
  else if (Opcode == 0xFE)
  {
    FrstOp.Reg()   = GB_RegA;

    u8 Data;
    ScdOp.Type()   = (O_IMM8 | O_NO_REF);
    rBinStrm.Read(Address + 1, Data);
    ScdOp.Value()  = Data;

    rInsn.Length() = 2;
    return true;
  }

  return false;
}

bool GameBoyArchitecture::Insn_Ld(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8  Opcode;
  u32 O1Type    = O_NONE,         O2Type      = O_NONE;
  u16 O1Reg     = GB_Invalid_Reg, O2Reg       = GB_Invalid_Reg;

  rInsn.SetName("ld");
  rInsn.Opcode() = GB_Ld;

  rBinStrm.Read(Address, Opcode);

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
    rBinStrm.Read(Address + 1, Data);
    FstOp.Value() = Data;
    InsnLen++;
  }
  else if ((O1Type & DS_MASK) == DS_16BIT)
  {
    u16 Data;
    rBinStrm.Read(Address + 1, Data);
    FstOp.Value() = Data;
    InsnLen += 2;
  }

  if ((O2Type & DS_MASK) == DS_8BIT)
  {
    u8 Data;
    rBinStrm.Read(Address + 1, Data);
    ScdOp.Value() = Data;
    InsnLen++;
  }
  else if ((O2Type & DS_MASK) == DS_16BIT)
  {
    u16 Data;
    rBinStrm.Read(Address + 1, Data);
    ScdOp.Value() = Data;
    InsnLen += 2;
  }

  rInsn.Length() = InsnLen;

  FstOp.Type()   = O1Type;
  FstOp.Reg()    = O1Reg;

  ScdOp.Type()   = O2Type;
  ScdOp.Reg()    = O2Reg;

  return true;
}

bool GameBoyArchitecture::Insn_Ldi(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("ldi");
  rInsn.Opcode()  = GB_Ldi;
  rInsn.Length()  = 1;

  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

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

  return true;
}

bool GameBoyArchitecture::Insn_Ldd(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("ldd");
  rInsn.Opcode()  = GB_Ldd;
  rInsn.Length()  = 1;

  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

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

  return true;
}

bool GameBoyArchitecture::Insn_Ldh(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("ldh");
  rInsn.Opcode()  = GB_Ldi;
  rInsn.Length()  = 2;

  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  u8 Data;
  rBinStrm.Read(Address + 1, Data);

  Operand& FrstOp = rInsn.FirstOperand();
  Operand& ScdOp  = rInsn.SecondOperand();


  // (n), A
  if (Opcode == 0xE0)
  {
    FrstOp.Type() |= O_MEM | O_IMM16;
    FrstOp.Value() = 0xFF00 + Data;
    ScdOp.Type()   = O_REG;
    ScdOp.Reg()    = GB_RegA;
  }

  // A, (n)
  else if (Opcode == 0xF0)
  {
    FrstOp.Type() = O_REG;
    FrstOp.Reg()  = GB_RegA;
    ScdOp.Type() |= O_MEM | O_IMM16;
    ScdOp.Value() = 0xFF00 + Data;
  }

  else return false;

  return true;
}

bool GameBoyArchitecture::Insn_Ldhl(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("ld");
  rInsn.Length()               = 2;
  rInsn.Opcode()               = GB_Ldhl;

  rInsn.FirstOperand().Type()  = O_REG;
  rInsn.FirstOperand().Reg()   = GB_RegHL;
  rInsn.SecondOperand().Type() = O_REG;
  rInsn.SecondOperand().Reg()  = GB_RegSp;
  s8 Offset;
  rBinStrm.Read(Address + 1, Offset);
  rInsn.ThirdOperand().Type()  = O_IMM8;
  rInsn.ThirdOperand().Value() = Offset;

  return true;
}

bool GameBoyArchitecture::Insn_Push(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  u16 Reg;

  rBinStrm.Read(Address, Opcode);

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

  return true;
}

bool GameBoyArchitecture::Insn_Pop(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  u16 Reg;

  rBinStrm.Read(Address, Opcode);

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

  return true;
}

bool GameBoyArchitecture::Insn_Jr(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  s8 Offset;
  rBinStrm.Read(Address, Opcode);
  rBinStrm.Read(Address + 1, Offset);

  rInsn.Length()        = 2;
  rInsn.Opcode()        = GB_Jr;
  rInsn.OperationType() = Instruction::OpJump;

  rInsn.FirstOperand().Type() = O_REL;
  rInsn.FirstOperand().Value() = Offset;

  switch (Opcode)
  {
  case 0x18: rInsn.Cond() = GB_Cond_None;     rInsn.SetName("jr");    break;
  case 0x20: rInsn.Cond() = GB_Cond_NotZero;  rInsn.SetName("jr nz"); break;
  case 0x28: rInsn.Cond() = GB_Cond_Zero;     rInsn.SetName("jr z");  break;
  case 0x30: rInsn.Cond() = GB_Cond_NotCarry; rInsn.SetName("jr nc"); break;
  case 0x38: rInsn.Cond() = GB_Cond_Carry;    rInsn.SetName("jr c");  break;
  default: return false;
  }

  return true;
}

bool GameBoyArchitecture::Insn_Jp(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  rInsn.Opcode()        = GB_Jp;
  rInsn.OperationType() = Instruction::OpJump;

  // (HL)
  if (Opcode == 0xE9)
  {
    rInsn.SetName("jp");
    rInsn.Length() = 1;
    rInsn.FirstOperand().Type() = (O_REG | O_MEM);
    rInsn.FirstOperand().Reg()  = GB_RegHL;
    return true;
  }

  u16 Offset;
  rBinStrm.Read(Address + 1, Offset);

  rInsn.Length()               = 3;
  rInsn.FirstOperand().Type()  = O_ABS16;
  rInsn.FirstOperand().Value() = Offset;

  switch (Opcode)
  {
  case 0xC3: rInsn.Cond() = GB_Cond_None;     rInsn.SetName("jp");    break;
  case 0xC2: rInsn.Cond() = GB_Cond_NotZero;  rInsn.SetName("jp nz"); break;
  case 0xCA: rInsn.Cond() = GB_Cond_Zero;     rInsn.SetName("jp z");  break;
  case 0xD2: rInsn.Cond() = GB_Cond_NotCarry; rInsn.SetName("jp nc"); break;
  case 0xDA: rInsn.Cond() = GB_Cond_Carry;    rInsn.SetName("jp c");  break;
  default: return false;
  }

  return true;
}

bool GameBoyArchitecture::Insn_Call(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  u8 Opcode;
  u16 Offset;
  rBinStrm.Read(Address, Opcode);
  rBinStrm.Read(Address + 1, Offset);

  rInsn.Length()               = 3;
  rInsn.Opcode()               = GB_Call;
  rInsn.OperationType()        = Instruction::OpCall;

  rInsn.FirstOperand().Type()  = O_ABS16;
  rInsn.FirstOperand().Value() = Offset;

  switch (Opcode)
  {
  case 0xCD: rInsn.Cond() = GB_Cond_None;      rInsn.SetName("call");    break;
  case 0xC4: rInsn.Cond() = GB_Cond_NotZero;   rInsn.SetName("call nz"); break;
  case 0xCC: rInsn.Cond() = GB_Cond_Zero;      rInsn.SetName("call z");  break;
  case 0xD4: rInsn.Cond() = GB_Cond_NotCarry;  rInsn.SetName("call nc"); break;
  case 0xDD: rInsn.Cond() = GB_Cond_Carry;     rInsn.SetName("call c");  break;
  default: return false;
  }

  return true;
}

bool GameBoyArchitecture::Insn_Rst(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("rst");
  rInsn.Opcode() = GB_Rst;
  rInsn.Length() = 1;

  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  rInsn.FirstOperand().Type()  = (O_IMM8 | O_NO_REF);
  rInsn.FirstOperand().Value() = ((Opcode >> 3) & 0x07) * 0x08;

  return true;
}

bool GameBoyArchitecture::Insn_Ret(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.Opcode()        = GB_Ret;
  rInsn.Length()        = 1;
  rInsn.OperationType() = Instruction::OpRet;

  u8 Opcode;
  rBinStrm.Read(Address, Opcode);

  switch (Opcode)
  {
  case 0xC9: rInsn.Cond() = GB_Cond_None;     rInsn.SetName("ret");    break;
  case 0xC0: rInsn.Cond() = GB_Cond_NotZero;  rInsn.SetName("ret nz"); break;
  case 0xC8: rInsn.Cond() = GB_Cond_Zero;     rInsn.SetName("ret z");  break;
  case 0xD0: rInsn.Cond() = GB_Cond_NotCarry; rInsn.SetName("ret nc"); break;
  case 0xD8: rInsn.Cond() = GB_Cond_Carry;    rInsn.SetName("ret c");  break;
  default: return false;
  }
  return true;
}

bool GameBoyArchitecture::Insn_Reti(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.SetName("reti");
  rInsn.Opcode()        = GB_Reti;
  rInsn.Length()        = 1;
  rInsn.OperationType() = Instruction::OpRet;
  return true;
}

bool GameBoyArchitecture::Insn_Halt(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.Opcode() = GB_Halt;
  rInsn.Length() = 1;
  rInsn.SetName("halt");

  return true;
}

bool GameBoyArchitecture::Insn_Stop(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.Opcode() = GB_Stop;
  rInsn.Length() = 1;
  rInsn.SetName("stop");

  return true;
}

bool GameBoyArchitecture::Insn_Ei(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.Opcode() = GB_Ei;
  rInsn.Length() = 1;
  rInsn.SetName("ei");

  return true;
}

bool GameBoyArchitecture::Insn_Di(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn)
{
  rInsn.Opcode() = GB_Di;
  rInsn.Length() = 1;
  rInsn.SetName("di");

  return true;
}
