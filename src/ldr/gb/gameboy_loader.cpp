#include "medusa/medusa.hpp"
#include "medusa/log.hpp"

#include "gameboy_loader.hpp"

#include <iomanip>
#include <sstream>
#include <algorithm>
#include <boost/foreach.hpp>

std::string GameBoyLoader::GetName(void) const
{
  return "GameBoy (color)";
}

bool GameBoyLoader::IsCompatible(BinaryStream const& rBinStrm)
{
  if (rBinStrm.GetSize() < GAMEBOY_HEADER_OFFSET + sizeof m_GameBoyRom)
    return false;

  if (!rBinStrm.Read(GAMEBOY_HEADER_OFFSET, reinterpret_cast<u8*>(&m_GameBoyRom), sizeof m_GameBoyRom))
    return false;

  if (memcmp(m_GameBoyRom.NintendoLogo, s_NintendoLogo, NINTENDO_LOGO_LEN))
    return false;

  return true;
}

void GameBoyLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs)
{
  rDoc.AddMemoryArea(new VirtualMemoryArea(
    "VRAM",
    Address(Address::BankType, 0, 0x8000, 8, 16), 0x2000,
    MemoryArea::Read | MemoryArea::Write
  ));

  rDoc.AddMemoryArea(new VirtualMemoryArea(
    "RAM#nn",
    Address(Address::BankType, 0, 0xA000, 8, 16), 0x2000,
    MemoryArea::Read | MemoryArea::Write
  ));

  rDoc.AddMemoryArea(new VirtualMemoryArea(
    "RAM/IOMap",
    Address(Address::BankType, 0, 0xC000, 8, 16), 0x4000,
    MemoryArea::Read | MemoryArea::Write
  ));

  rDoc.AddMemoryArea(new MappedMemoryArea(
    "ROM#00",
    0x0, BankSize,
    Address(Address::BankType, 0, 0x0000, 8, 16), BankSize,
    MemoryArea::Execute | MemoryArea::Read | MemoryArea::Write
  ));

  // TODO: This memory area is not really located at 0:4000, but it's the default value
  rDoc.AddMemoryArea(new MappedMemoryArea(
    "ROM#01",
    0x4000, BankSize,
    Address(Address::BankType, 0, 0x4000, 8, 16), BankSize,
    MemoryArea::Execute | MemoryArea::Read | MemoryArea::Write
  ));

  BankType BankNo = GetNumberOfBank();
  Log::Write("ldr_gb") << "Number of bank: " << BankNo << LogEnd;
  TOffset Offset = 0x8000;
  for (TBank Bank = 2; Bank < BankNo; ++Bank)
  {
    std::ostringstream oss;
    oss << "ROM#" << std::hex << std::setw(2) << std::setfill('0') << Bank;

    rDoc.AddMemoryArea(new MappedMemoryArea(
      oss.str().c_str(),
      Bank * BankSize, BankSize,
      Address(Address::BankType, Bank, 0x4000, 8, 16), BankSize,
      MemoryArea::Execute | MemoryArea::Read | MemoryArea::Write
    ));
  }

  rDoc.AddLabel(Address(Address::BankType, 0x0, 0x100, 8, 16),   Label("start",                  Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0x40,  8, 16),   Label("VerticalBlankInterrupt", Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0x48,  8, 16),   Label("LcdStatusInterrupt",     Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0x50,  8, 16),   Label("TimerInterrupt",         Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0x58,  8, 16),   Label("SerialLinkInterrupt",    Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0x60,  8, 16),   Label("JoypadPressInterrupt",   Label::Code | Label::Global));

  // Source: http://fms.komkon.org/GameBoy/Tech/Software.html
  // Source: http://nocash.emubase.de/pandocs.htm
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff00, 8, 16), Label("JOYPAD",                 Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff01, 8, 16), Label("SIODATA",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff02, 8, 16), Label("SIOCONT",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff04, 8, 16), Label("DIVIDER",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff05, 8, 16), Label("TIMECNT",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff06, 8, 16), Label("TIMEMOD",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff07, 8, 16), Label("TIMECONT",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff0f, 8, 16), Label("IFLAGS",                 Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff10, 8, 16), Label("SNDREG10",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff11, 8, 16), Label("SNDREG11",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff12, 8, 16), Label("SNDREG12",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff13, 8, 16), Label("SNDREG13",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff14, 8, 16), Label("SNDREG14",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff16, 8, 16), Label("SNDREG21",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff17, 8, 16), Label("SNDREG22",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff18, 8, 16), Label("SNDREG23",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff19, 8, 16), Label("SNDREG24",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff1a, 8, 16), Label("SNDREG25",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff1b, 8, 16), Label("SNDREG31",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff1c, 8, 16), Label("SNDREG32",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff1d, 8, 16), Label("SNDREG33",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff1e, 8, 16), Label("SNDREG34",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff20, 8, 16), Label("SNDREG41",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff21, 8, 16), Label("SNDREG42",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff22, 8, 16), Label("SNDREG43",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff23, 8, 16), Label("SNDREG44",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff24, 8, 16), Label("SNDREG50",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff25, 8, 16), Label("SNDREG51",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff26, 8, 16), Label("SNDREG52",               Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff40, 8, 16), Label("LCDCONT",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff41, 8, 16), Label("LCDSTAT",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff42, 8, 16), Label("SCROLLY",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff43, 8, 16), Label("SCROLLX",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff44, 8, 16), Label("CURLINE",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff45, 8, 16), Label("CMPLINE",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff46, 8, 16), Label("DMACONT",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff47, 8, 16), Label("BGRDPAL",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff48, 8, 16), Label("OBJ0PAL",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff49, 8, 16), Label("OBJ1PAL",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff4a, 8, 16), Label("WNDPOSY",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff4b, 8, 16), Label("WNDPOSX",                Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff4d, 8, 16), Label("KEY1",                   Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff51, 8, 16), Label("HDMA1",                  Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff52, 8, 16), Label("HDMA2",                  Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff53, 8, 16), Label("HDMA3",                  Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff54, 8, 16), Label("HDMA4",                  Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff55, 8, 16), Label("HDMA5",                  Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff56, 8, 16), Label("RP",                     Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff68, 8, 16), Label("BGPI",                   Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff69, 8, 16), Label("BGPD",                   Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff6a, 8, 16), Label("OBPI",                   Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff6b, 8, 16), Label("OBPD",                   Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xff70, 8, 16), Label("SVBK",                   Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x0, 0xffff, 8, 16), Label("ISWITCH",                Label::Data | Label::Global));
}

TBank GameBoyLoader::GetNumberOfBank(void) const
{
  switch (m_GameBoyRom.RomSize)
  {
  case 0x00: return 0x02; // 32Kbyte
  case 0x01: return 0x04; // 64Kbyte
  case 0x02: return 0x08; // 128Kbyte
  case 0x03: return 0x10; // 256Kbyte
  case 0x04: return 0x20; // 512Kbyte
  case 0x05: return 0x40; // 1Mbyte
  case 0x06: return 0x80; // 2Mbyte
  case 0x52: return 0x48; // 1.1Mbyte
  case 0x53: return 0x50; // 1.2Mbyte
  case 0x54: return 0x60; // 1.5Mbyte
  default:   return 0x00; // Unknown
  }
}

void GameBoyLoader::FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const
{
  rArchs.erase(std::remove_if(std::begin(rArchs), std::end(rArchs), [](Architecture::SPType spArch)
  { return spArch->GetName() != "Nintendo GameBoy Z80"; }), std::end(rArchs));
}
