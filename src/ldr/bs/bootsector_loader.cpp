#include "medusa/medusa.hpp"
#include <boost/foreach.hpp>

#include "bootsector_loader.hpp"

std::string BootSectorLoader::GetName(void) const
{
  return "Boot sector";
}

bool BootSectorLoader::IsCompatible(BinaryStream const& rBinStrm)
{
  if (rBinStrm.GetSize() != 0x200)
    return false;

  u16 Signature;
  if (!rBinStrm.Read(0x200 - 2, Signature))
    return false;

  if (Signature != 0xAA55)
    return false;

  return true;
}

void BootSectorLoader::Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs)
{
  auto spArchX86 = *std::find_if(std::begin(rArchs), std::end(rArchs), [](Architecture::SharedPtr spArch)
  {
    return spArch->GetName() == "Intel X86";
  });
  rDoc.AddMemoryArea(new MappedMemoryArea(
    "mem",
    0x0, 0x200,
    Address(Address::FlatType, 0x0, AddressOffset, 16, 16), 0x200,
    MemoryArea::Read | MemoryArea::Write | MemoryArea::Execute,
    spArchX86->GetTag(), spArchX86->GetModeByName("16-bit")
  ));

  rDoc.AddLabel(Address(Address::FlatType, 0x0, AddressOffset, 16, 16), Label("start", Label::Code | Label::Global));
}

void BootSectorLoader::FilterAndConfigureArchitectures(Architecture::VectorSharedPtr& rArchs) const
{
  rArchs.erase(std::remove_if(std::begin(rArchs), std::end(rArchs), [](Architecture::SharedPtr spArch)
  { return (spArch->GetName() != "Intel x86"); }), std::end(rArchs));
}