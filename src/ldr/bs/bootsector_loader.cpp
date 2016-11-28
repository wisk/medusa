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

bool BootSectorLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs)
{
  auto itArchX86 = std::find_if(std::begin(rArchs), std::end(rArchs), [](Architecture::SPType spArch)
  {
    return spArch->GetName() == "x86";
  });
  if (itArchX86 == std::end(rArchs))
  {
    Log::Write("ldr_bs").Level(LogError) << "unable to find X86 architecture module" << LogEnd;
    return false;
  }

  if (!rDoc.AddMemoryArea(MemoryArea::CreateMapped(
    "mem", MemoryArea::Access::Read | MemoryArea::Access::Write | MemoryArea::Access::Execute,
    0x0, 0x200,
    Address(Address::LinearType, 0x0, AddressOffset, 16, 16), 0x200,
    (*itArchX86)->GetTag(), (*itArchX86)->GetModeByName("16-bit")
  )))
    return false;

  if (!rDoc.AddLabel(Address(Address::LinearType, 0x0, AddressOffset, 16, 16), Label("start", Label::Code | Label::Global)))
    return false;
  return true;
}

bool BootSectorLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs, Address const& rImgBase)
{
  return false;
}

void BootSectorLoader::FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const
{
  rArchs.erase(std::remove_if(std::begin(rArchs), std::end(rArchs), [](Architecture::SPType spArch)
  { return (spArch->GetName() != "x86"); }), std::end(rArchs));
}