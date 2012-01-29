#include "medusa/medusa.hpp"
#include "medusa/log.hpp"

#include "gameboy_loader.hpp"

#include <iomanip>
#include <sstream>
#include <algorithm>
#include <boost/foreach.hpp>

GameBoyLoader::GameBoyLoader(Database& rDatabase)
  : Loader(rDatabase)
  , m_rDatabase(rDatabase)
  , m_IsValid(false)
{
  if (m_rDatabase.GetFileBinaryStream().GetSize() < GAMEBOY_HEADER_OFFSET + sizeof m_GameBoyRom)
    return;

  m_rDatabase.GetFileBinaryStream().Read(GAMEBOY_HEADER_OFFSET, reinterpret_cast<u8*>(&m_GameBoyRom), sizeof m_GameBoyRom);

  // If the logo contained on a cartridge is not the genuine one
  // the game can't boot on real hardware
  if (memcmp(m_GameBoyRom.u8NintendoLogo, s_u8NintendoLogo, NINTENDO_LOGO_LEN))
    return;

  m_IsValid = true;
}

void GameBoyLoader::Map(void)
{
  m_rDatabase.AddMemoryArea(new MappedMemoryArea(
    m_rDatabase.GetFileBinaryStream(), "ROM",
    Address(Address::PhysicalType, 0x0),          BankSize,
    Address(Address::BankType, 0, 0x0000, 8, 16), BankSize,
    MA_EXEC | MA_READ | MA_WRITE
  ));

  m_rDatabase.AddMemoryArea(new VirtualMemoryArea(
    m_rDatabase.GetFileBinaryStream().GetEndianness(), "VRAM",
    Address(Address::BankType, 0, 0x8000, 8, 16), 0x2000,
    MA_READ | MA_WRITE
  ));

  m_rDatabase.AddMemoryArea(new VirtualMemoryArea(
    m_rDatabase.GetFileBinaryStream().GetEndianness(), "RAM#nn",
    Address(Address::BankType, 0, 0xA000, 8, 16), 0x2000,
    MA_READ | MA_WRITE
  ));

  m_rDatabase.AddMemoryArea(new VirtualMemoryArea(
    m_rDatabase.GetFileBinaryStream().GetEndianness(), "RAM/IOMap",
    Address(Address::BankType, 0, 0xC000, 8, 16), 0x4000,
    MA_READ | MA_WRITE
  ));

  // MBC5 allows to map the first bank
  m_rDatabase.AddMemoryArea(new MappedMemoryArea(
    m_rDatabase.GetFileBinaryStream(), "ROM#00",
    Address(Address::PhysicalType, 0x0),          BankSize,
    Address(Address::BankType, 0, 0x4000, 8, 16), BankSize,
    MA_EXEC | MA_READ | MA_WRITE
  ));

  m_rDatabase.AddMemoryArea(new MappedMemoryArea(
    m_rDatabase.GetFileBinaryStream(), "ROM#01",
    Address(Address::PhysicalType, 0x4000),       BankSize,
    Address(Address::BankType, 1, 0x4000, 8, 16), BankSize,
    MA_EXEC | MA_READ | MA_WRITE
  ));

  TBank BankNo = GetNumberOfBank();
  Log::Write("ldr_gb") << "Number of bank: " << BankNo << LogEnd;
  TOffset Offset = 0x8000;
  for (TBank Bank = 2; Bank < BankNo; ++Bank)
  {
    std::ostringstream oss;
    oss << "ROM#" << std::hex << std::setw(2) << std::setfill('0') << Bank;

    m_rDatabase.AddMemoryArea(new MappedMemoryArea(
      m_rDatabase.GetFileBinaryStream(), oss.str().c_str(),
      Address(Address::PhysicalType, Bank * BankSize), BankSize,
      Address(Address::BankType, Bank, 0x4000, 8, 16), BankSize,
      MA_EXEC | MA_READ | MA_WRITE
    ));
  }

  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0x40, 8, 16),   Label("VerticalBlankInterrupt", Label::LabelCode));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0x48, 8, 16),   Label("LcdStatusInterrupt",     Label::LabelCode));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0x50, 8, 16),   Label("TimerInterrupt",         Label::LabelCode));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0x58, 8, 16),   Label("SerialLinkInterrupt",    Label::LabelCode));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0x60, 8, 16),   Label("JoypadPressInterrupt",   Label::LabelCode));

  // Source: http://fms.komkon.org/GameBoy/Tech/Software.html
  // Source: http://nocash.emubase.de/pandocs.htm
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff00, 8, 16), Label("JOYPAD",                 Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff01, 8, 16), Label("SIODATA",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff02, 8, 16), Label("SIOCONT",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff04, 8, 16), Label("DIVIDER",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff05, 8, 16), Label("TIMECNT",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff06, 8, 16), Label("TIMEMOD",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff07, 8, 16), Label("TIMECONT",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff0f, 8, 16), Label("IFLAGS",                 Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff10, 8, 16), Label("SNDREG10",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff11, 8, 16), Label("SNDREG11",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff12, 8, 16), Label("SNDREG12",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff13, 8, 16), Label("SNDREG13",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff14, 8, 16), Label("SNDREG14",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff16, 8, 16), Label("SNDREG21",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff17, 8, 16), Label("SNDREG22",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff18, 8, 16), Label("SNDREG23",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff19, 8, 16), Label("SNDREG24",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff1a, 8, 16), Label("SNDREG25",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff1b, 8, 16), Label("SNDREG31",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff1c, 8, 16), Label("SNDREG32",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff1d, 8, 16), Label("SNDREG33",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff1e, 8, 16), Label("SNDREG34",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff20, 8, 16), Label("SNDREG41",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff21, 8, 16), Label("SNDREG42",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff22, 8, 16), Label("SNDREG43",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff23, 8, 16), Label("SNDREG44",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff24, 8, 16), Label("SNDREG50",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff25, 8, 16), Label("SNDREG51",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff26, 8, 16), Label("SNDREG52",               Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff40, 8, 16), Label("LCDCONT",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff41, 8, 16), Label("LCDSTAT",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff42, 8, 16), Label("SCROLLY",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff43, 8, 16), Label("SCROLLX",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff44, 8, 16), Label("CURLINE",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff45, 8, 16), Label("CMPLINE",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff46, 8, 16), Label("DMACONT",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff47, 8, 16), Label("BGRDPAL",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff48, 8, 16), Label("OBJ0PAL",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff49, 8, 16), Label("OBJ1PAL",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff4a, 8, 16), Label("WNDPOSY",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff4b, 8, 16), Label("WNDPOSX",                Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff4d, 8, 16), Label("KEY1",                   Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff51, 8, 16), Label("HDMA1",                  Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff52, 8, 16), Label("HDMA2",                  Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff53, 8, 16), Label("HDMA3",                  Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff54, 8, 16), Label("HDMA4",                  Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff55, 8, 16), Label("HDMA5",                  Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff56, 8, 16), Label("RP",                     Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff68, 8, 16), Label("BGPI",                   Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff69, 8, 16), Label("BGPD",                   Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff6a, 8, 16), Label("OBPI",                   Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff6b, 8, 16), Label("OBPD",                   Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xff70, 8, 16), Label("SVBK",                   Label::LabelData));
  m_rDatabase.AddLabel(Address(Address::BankType, 0x0, 0xffff, 8, 16), Label("ISWITCH",                Label::LabelData));
}

Address GameBoyLoader::GetEntryPoint(void)
{
  return Address(Address::BankType, 0x0, GAMEBOY_HEADER_OFFSET, 8, 16);
}

TBank GameBoyLoader::GetNumberOfBank(void) const
{
  switch (m_GameBoyRom.u8RomSize)
  {
  case 0x00: return 0x02; // 32Kbyte
  case 0x01: return 0x04; // 64Kbyte
  case 0x02: return 0x08; // 128Kbyte
  case 0x03: return 0x10; // 256Kbyte
  case 0x04: return 0x20; // 512Kbyte
  case 0x05: return 0x40; // 1Mbyte
  case 0x06: return 0x80; // 2Mbyte
  case 0x34: return 0x48; // 1.1Mbyte
  case 0x35: return 0x50; // 1.2Mbyte
  case 0x36: return 0x60; // 1.5Mbyte
  default:   return 0x00; // Unknown
  }
}

Architecture::SPtr GameBoyLoader::GetMainArchitecture(Architecture::VectorSPtr& rArchitectures)
{
  if (rArchitectures.size() > 0)
    BOOST_FOREACH(Architecture::SPtr pArchitecture, rArchitectures)
    {
      if (pArchitecture->GetName() == "Nintendo GameBoy Z80")
        return pArchitecture;
    }
  return Architecture::SPtr();
}