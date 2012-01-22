#include "x86_architecture.hpp"

bool X86Architecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)
{
  u8 Opcode;
  rBinStrm.Read(Offset, Opcode);
  bool Res = (this->*m_Table1[Opcode])(rBinStrm, Offset + 1, rInsn);
  ApplySegmentOverridePrefix(rInsn);
  return Res;
}

void X86Architecture::FormatCell(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddr, Cell& rCell)
{
  if (rCell.GetType() == Cell::InstructionType)
  {
    X86Architecture::FormatInstruction(rDatabase, rAddr, static_cast<Instruction&>(rCell));
    return;
  }

  Architecture::FormatCell(rDatabase, rBinStrm, rAddr, rCell);
}

void X86Architecture::FillConfigurationModel(ConfigurationModel& rCfgMdl)
{
  ConfigurationModel::Enum Bit;
  Bit.push_back(std::make_pair("16-bit", X86_Bit_16));
  Bit.push_back(std::make_pair("32-bit", X86_Bit_32));
  Bit.push_back(std::make_pair("64-bit", X86_Bit_64));
  rCfgMdl.Set("Bit", Bit, X86_Bit_32);

  /* XXX: We don't have enough information about instructions origin to offer this option. */
  //ConfigurationModel::Enum FeatInt;
  //FeatInt.push_back(std::make_pair("8086",  X86_FeatInt_8086 ));
  //FeatInt.push_back(std::make_pair("80186", X86_FeatInt_80186));
  //FeatInt.push_back(std::make_pair("80286", X86_FeatInt_80286));
  //FeatInt.push_back(std::make_pair("80386", X86_FeatInt_80386));
  //FeatInt.push_back(std::make_pair("80486", X86_FeatInt_80486));
  //FeatInt.push_back(std::make_pair("80586", X86_FeatInt_80586));
  //FeatInt.push_back(std::make_pair("80686", X86_FeatInt_80686));
  //FeatInt.push_back(std::make_pair("80786", X86_FeatInt_80786));
  //FeatInt.push_back(std::make_pair("sse",   X86_FeatInt_Sse  ));
  //FeatInt.push_back(std::make_pair("sse2",  X86_FeatInt_Sse2 ));
  //FeatInt.push_back(std::make_pair("sse3",  X86_FeatInt_Sse3 ));
  //FeatInt.push_back(std::make_pair("svm",   X86_FeatInt_Svm  ));
  //FeatInt.push_back(std::make_pair("vmx",   X86_FeatInt_Vmx  ));
  //FeatInt.push_back(std::make_pair("sse4a", X86_FeatInt_Sse4a));
  //rCfgMdl.Set("Integer feature", FeatInt, X86_FeatInt_Sse4a);

  //ConfigurationModel::Enum FeatFp;
  //FeatFp.push_back(std::make_pair("8087",  X86_FeatFp_8087 ));
  //FeatFp.push_back(std::make_pair("80287", X86_FeatFp_80287));
  //FeatFp.push_back(std::make_pair("80387", X86_FeatFp_80387));
  //FeatFp.push_back(std::make_pair("80487", X86_FeatFp_80487));
  //FeatFp.push_back(std::make_pair("80587", X86_FeatFp_80587));
  //rCfgMdl.Set("Floating-point feature", FeatFp, X86_FeatFp_80587);

  //ConfigurationModel::Enum FeatSimd;
  //FeatSimd.push_back(std::make_pair("mmx",     X86_FeatSimd_Mmx    ));
  //FeatSimd.push_back(std::make_pair("mmx+",    X86_FeatSimd_MmxPlus));
  //FeatSimd.push_back(std::make_pair("emmx",    X86_FeatSimd_Emmx   ));
  //FeatSimd.push_back(std::make_pair("3dnow!",  X86_FeatSimd_3dNow  ));
  //FeatSimd.push_back(std::make_pair("sse",     X86_FeatSimd_Sse    ));
  //FeatSimd.push_back(std::make_pair("sse 2",   X86_FeatSimd_Sse2   ));
  //FeatSimd.push_back(std::make_pair("sse 3",   X86_FeatSimd_Sse3   ));
  //FeatSimd.push_back(std::make_pair("ssse 3",  X86_FeatSimd_Ssse3  ));
  //FeatSimd.push_back(std::make_pair("sse 4.1", X86_FeatSimd_Sse41  ));
  //FeatSimd.push_back(std::make_pair("sse 4a",  X86_FeatSimd_Sse4a  ));
  //FeatSimd.push_back(std::make_pair("sse 4.2", X86_FeatSimd_Sse42  ));
  //FeatSimd.push_back(std::make_pair("aes",     X86_FeatSimd_Aes    ));
  //FeatSimd.push_back(std::make_pair("avx",     X86_FeatSimd_Avx    ));
  //rCfgMdl.Set("SIMD feature", FeatSimd, X86_FeatSimd_Avx);

  //rCfgMdl.Set("undocumented instruction", true);
}