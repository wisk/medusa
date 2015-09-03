#include "medusa/medusa.hpp"
#include "medusa/log.hpp"

#include "st62_loader.hpp"

#include <iomanip>
#include <sstream>
#include <algorithm>
#include <boost/foreach.hpp>

std::string ST62Loader::GetName(void) const
{
  return "ST62 ROM";
}

bool ST62Loader::IsCompatible(BinaryStream const& rBinStrm)
{
  u8 vector[0x10];

  if (rBinStrm.GetSize() != 0x1000)
    return false;

  if (!rBinStrm.Read(0xFF0, vector, sizeof vector))
    return false;

  // Reset vector should have a JP instruction
  if ( (vector[0xE] & 0xF) != 0x09)
    return false;

  // NMI vector should have a JP instruction
  if ((vector[0xC] & 0x0F) != 0x09)
    return false;

  // ADC vector should have a JP instruction
  if ((vector[0x0] & 0x0F) != 0x09)
    return false;

  // TIMER vector should have a JP instruction
  if ((vector[0x2] & 0x0F) != 0x09)
    return false;

  // PORTB vector should have a JP instruction
  if ((vector[0x4] & 0x0F) != 0x09)
    return false;

  // PORTA vector should have a JP instruction
  if ((vector[0x6] & 0x0F) != 0x09)
    return false;

  return true;
}

void ST62Loader::Map(Document& rDoc, Architecture::VSPType const& rArchs)
{
  auto itArch = std::find_if(std::begin(rArchs), std::end(rArchs), [](Architecture::SPType spArch)
  {
    if (spArch->GetName() == "ST62")
      return true;
    return false;
  });

  if (itArch == std::end(rArchs))
  {
    Log::Write("ldr_st62") << "unable to find ST62 architecture" << LogEnd;
    return;
  }

  Tag ArchTag = (*itArch)->GetTag();
  u8 ArchMode = (*itArch)->GetModeByName("ST62x25");
  if (ArchMode == 0)
  {
    Log::Write("ldr_st62") << "unable to set ST62x25 mode" << LogEnd;
    return;
  }

  rDoc.AddMemoryArea(new MappedMemoryArea(
    "ROM",
    0x0, rDoc.GetBinaryStream().GetSize(),
    Address(Address::BankType, 0x0, 0x0, 8, 16), rDoc.GetBinaryStream().GetSize(),
    MemoryArea::Execute,
    ArchTag, ArchMode
    ));

  rDoc.AddMemoryArea(new VirtualMemoryArea(
    "DATA",
    Address(Address::BankType, 0x1000, 0x00, 16, 8), 0x100,
    MemoryArea::Read | MemoryArea::Write,
    ArchTag, ArchMode
    ));

  // Try and decode jump instruction @ reset vector to create start label
  Instruction insn;

  if ((*itArch)->Disassemble(rDoc.GetBinaryStream(), 0xFFE, insn, ArchMode) &&
      insn.GetSubType() & Instruction::JumpType) {
    Address CurAddr;
    Address DstAddr;
    rDoc.ConvertPositionToAddress(0xFFE, CurAddr);
    if (insn.GetOperandReference(rDoc, 0, CurAddr, DstAddr, false)) {
      rDoc.AddLabel(DstAddr, Label("start", Label::Code | Label::Global));
    }
  }

  rDoc.AddLabel(Address(Address::FlatType, 0xFF0), Label("ADC_vec", Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::FlatType, 0xFF2), Label("TIMER_vec", Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::FlatType, 0xFF4), Label("PORTB_vec", Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::FlatType, 0xFF6), Label("PORTA_vec", Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::FlatType, 0xFFC), Label("NMI_vec", Label::Code | Label::Global));
  rDoc.AddLabel(Address(Address::FlatType, 0xFFE), Label("RESET_vec", Label::Code | Label::Global));

  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0x80, 16, 8), Label("X", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0x81, 16, 8), Label("Y", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0x82, 16, 8), Label("V", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0x83, 16, 8), Label("W", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xC0, 16, 8), Label("DRA", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xC1, 16, 8), Label("DRB", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xC2, 16, 8), Label("DRC", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xC4, 16, 8), Label("DDRA", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xC5, 16, 8), Label("DDRB", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xC6, 16, 8), Label("DDRC", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xCC, 16, 8), Label("ORA", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xCD, 16, 8), Label("ORB", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xCE, 16, 8), Label("ORC", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xC8, 16, 8), Label("IOR", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xC9, 16, 8), Label("DRWR", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xD0, 16, 8), Label("ADR", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xD1, 16, 8), Label("ACR", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xD2, 16, 8), Label("TIMR", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xD3, 16, 8), Label("TDR", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xD4, 16, 8), Label("TSCR", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xD8, 16, 8), Label("WDT", Label::Data | Label::Global));
  rDoc.AddLabel(Address(Address::BankType, 0x1000, 0xFF, 16, 8), Label("A", Label::Data | Label::Global));
}


void ST62Loader::FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const
{
  rArchs.erase(std::remove_if(std::begin(rArchs), std::end(rArchs), [](Architecture::SPType spArch)
  { return spArch->GetName() != "ST62"; }), std::end(rArchs));
}
