#include "st62_architecture.hpp"
#include <medusa/extend.hpp>


Expression::SPType St62Architecture::__Decode_ext(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  u8 High;
  u8 Low;
  u16 Address;

  rInsn.Length()++;
  rBinStrm.Read(Offset, High);
  rBinStrm.Read(Offset+1, Low);

  Address= (u16)((High & 0xF0) >> 4) | ((u16)Low<<4);

  return Expr::MakeConst(16, Address);
}

Expression::SPType St62Architecture::__Decode_pcr(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  u8 Byte;
  u16 Address;
  u16 Offs;

  rBinStrm.Read(Offset, Byte);
  Offs = (Byte>>3) & 0xF;

  if (Byte & 0x80) {
      Offs = -Offs;
  }
  Address= Offset + Offs + 1;

  return Expr::MakeConst(16, Address);
}

Expression::SPType St62Architecture::__Decode_ee(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  s8 Byte;
  u16 Address;

  Offset++;
  rInsn.Length()++;
  rBinStrm.Read(Offset, Byte);

  Address= Offset + Byte + 1;

  return Expr::MakeConst(16, Address);
}

Expression::SPType St62Architecture::__Decode_direct(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  u8 Value;

  Offset++;
  rInsn.Length()++;
  rBinStrm.Read(Offset, Value);

  switch(Value) {
  case 0x80:
    return Expr::MakeId(ST62_Reg_X, &m_CpuInfo);
  case 0x81:
    return Expr::MakeId(ST62_Reg_Y, &m_CpuInfo);
  case 0x82:
    return Expr::MakeId(ST62_Reg_V, &m_CpuInfo);
  case 0x83:
    return Expr::MakeId(ST62_Reg_W, &m_CpuInfo);
   default:
    break;
  }

  return Expr::MakeMem(8, Expr::MakeConst(16, 0x1000), Expr::MakeConst(8, Value), true);
}

Expression::SPType St62Architecture::__Decode_imm(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  u8 Value;

  Offset++;
  rInsn.Length()++;
  rBinStrm.Read(Offset, Value);

  return Expr::MakeConst(8, Value);
}

Expression::SPType St62Architecture::__Decode_bitdirect(BinaryStream const& rBinStrm, TOffset& Offset, Instruction& rInsn, u8 Mode)
{
  static u8 map_bit[] = { 0,
                            4,
                            2,
                            6,
                            1,
                            5,
                            3,
                            7
                          };

  u8 Value;

  rBinStrm.Read(Offset, Value);
  Value = Value >> 5;

  return Expr::MakeConst(8, map_bit[Value]);
}
