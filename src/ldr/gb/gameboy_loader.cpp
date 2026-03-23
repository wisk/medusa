#include "medusa/medusa.hpp"
#include "medusa/log.hpp"
#include "medusa/module.hpp"

#include "gameboy_loader.hpp"

#include <iomanip>
#include <sstream>
#include <algorithm>

std::string GameBoyLoader::GetName(void) const
{
  return "GameBoy (color)";
}

bool GameBoyLoader::ReadHeader(BinaryStream const& rBinStrm, GameBoyRom& rRom)
{
  if (rBinStrm.GetSize() < GAMEBOY_HEADER_OFFSET + sizeof(GameBoyRom))
    return false;
  if (!rBinStrm.Read(GAMEBOY_HEADER_OFFSET, reinterpret_cast<u8*>(&rRom), sizeof(GameBoyRom)))
    return false;
  if (memcmp(rRom.NintendoLogo, s_NintendoLogo, NINTENDO_LOGO_LEN))
    return false;
  return true;
}

bool GameBoyLoader::IsCompatible(BinaryStream const& rBinStrm) const
{
  GameBoyRom Rom;
  return ReadHeader(rBinStrm, Rom);
}

std::vector<std::string> GameBoyLoader::GetUsedArchitectures(BinaryStream const& rBinStrm) const
{
  // GameBoy uses the Sharp LR35902 (SM83), a Z80-derivative
  return { "Zilog 80/LR35902" };
}

bool GameBoyLoader::Map(Document& rDoc) const
{
  auto const& rBinStrm = rDoc.GetBinaryStream();
  GameBoyRom Rom;
  if (!ReadHeader(rBinStrm, Rom))
    return false;

  auto const& rModMgr = ModuleManager::Instance();
  TagType ArchTag;
  u8 ArchMode;
  if (!rModMgr.ConvertArchitectureNameToTagAndMode("Zilog 80/LR35902", ArchTag, ArchMode))
  {
    Log::Write("ldr_gb") << "unable to find Z80/LR35902 architecture" << LogEnd;
    return false;
  }

  rDoc.AddMemoryArea(MemoryArea::CreateVirtual(
    "VRAM", MemoryArea::Access::Read | MemoryArea::Access::Write,
    Address(Address::LogicalType, 0, 0x8000, 8, 16), 0x2000,
    ArchTag, ArchMode
  ));

  rDoc.AddMemoryArea(MemoryArea::CreateVirtual(
    "RAM#nn", MemoryArea::Access::Read | MemoryArea::Access::Write,
    Address(Address::LogicalType, 0, 0xA000, 8, 16), 0x2000,
    ArchTag, ArchMode
  ));

  rDoc.AddMemoryArea(MemoryArea::CreateVirtual(
    "RAM/IOMap", MemoryArea::Access::Read | MemoryArea::Access::Write,
    Address(Address::LogicalType, 0, 0xC000, 8, 16), 0x4000,
    ArchTag, ArchMode
  ));

  rDoc.AddMemoryArea(MemoryArea::CreateMapped(
    "ROM#00", MemoryArea::Access::Execute | MemoryArea::Access::Read,
    0x0, BankSize,
    Address(Address::LogicalType, 0, 0x0000, 8, 16), BankSize,
    ArchTag, ArchMode
  ));

  rDoc.AddMemoryArea(MemoryArea::CreateMapped(
    "ROM#01", MemoryArea::Access::Execute | MemoryArea::Access::Read,
    0x4000, BankSize,
    Address(Address::LogicalType, 0, 0x4000, 8, 16), BankSize,
    ArchTag, ArchMode
  ));

  LogicalType BankNo = GetNumberOfBank(rBinStrm);
  Log::Write("ldr_gb") << "Number of bank: " << BankNo << LogEnd;
  for (BaseType Bank = 2; Bank < BankNo; ++Bank)
  {
    std::ostringstream oss;
    oss << "ROM#" << std::hex << std::setw(2) << std::setfill('0') << Bank;

    rDoc.AddMemoryArea(MemoryArea::CreateMapped(
      oss.str().c_str(), MemoryArea::Access::Execute | MemoryArea::Access::Read,
      Bank * BankSize, BankSize,
      Address(Address::LogicalType, Bank, 0x4000, 8, 16), BankSize,
      ArchTag, ArchMode
    ));
  }

  return true;
}

bool GameBoyLoader::Map(Document& rDoc, Address const& rImgBase) const
{
  return false;
}

void GameBoyLoader::Analyze(Document& rDoc) const
{
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0x100, 8, 16),   Label("start",                  Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0x40,  8, 16),   Label("VerticalBlankInterrupt", Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0x48,  8, 16),   Label("LcdStatusInterrupt",     Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0x50,  8, 16),   Label("TimerInterrupt",         Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0x58,  8, 16),   Label("SerialLinkInterrupt",    Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0x60,  8, 16),   Label("JoypadPressInterrupt",   Label::Code | Label::Global));

  // I/O registers
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff00, 8, 16), Label("JOYPAD",  Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff01, 8, 16), Label("SIODATA", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff02, 8, 16), Label("SIOCONT", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff04, 8, 16), Label("DIVIDER", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff05, 8, 16), Label("TIMECNT", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff06, 8, 16), Label("TIMEMOD", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff07, 8, 16), Label("TIMECONT",Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff0f, 8, 16), Label("IFLAGS", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff40, 8, 16), Label("LCDCONT",Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff41, 8, 16), Label("LCDSTAT",Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff42, 8, 16), Label("SCROLLY",Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff43, 8, 16), Label("SCROLLX",Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff44, 8, 16), Label("CURLINE",Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff46, 8, 16), Label("DMACONT",Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xff47, 8, 16), Label("BGRDPAL",Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::LogicalType, 0x0, 0xffff, 8, 16), Label("ISWITCH",Label::Data | Label::Global));
}

BaseType GameBoyLoader::GetNumberOfBank(BinaryStream const& rBinStrm) const
{
  GameBoyRom Rom;
  if (!ReadHeader(rBinStrm, Rom))
    return 0;
  switch (Rom.RomSize)
  {
  case 0x00: return 0x02;
  case 0x01: return 0x04;
  case 0x02: return 0x08;
  case 0x03: return 0x10;
  case 0x04: return 0x20;
  case 0x05: return 0x40;
  case 0x06: return 0x80;
  case 0x52: return 0x48;
  case 0x53: return 0x50;
  case 0x54: return 0x60;
  default:   return 0x00;
  }
}
