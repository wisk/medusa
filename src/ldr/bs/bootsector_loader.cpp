#include "bootsector_loader.hpp"

#include "medusa/medusa.hpp"
#include "medusa/module.hpp"

bool BootSectorLoader::IsCompatible(BinaryStream const& rBinStrm) const
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

bool BootSectorLoader::Map(Document& rDoc) const
{
  auto const& rModMngr = ModuleManager::Instance();
  TagType X86;
  u8 X86_16;
  if (!rModMngr.ConvertArchitectureNameToTagAndMode("x86/16-bit", X86, X86_16))
    return false;
  if (!rDoc.AddMemoryArea(MemoryArea::CreateMapped(
    "boot_sector", MemoryArea::Access::Read | MemoryArea::Access::Write | MemoryArea::Access::Execute,
    0x0, 0x200,
    Address(Address::LinearType, 0x0, AddressOffset, 16, 16), 0x200,
    X86, X86_16
  )))
    return false;

  if (!rDoc.AddLabel(Address(Address::LinearType, 0x0, AddressOffset, 16, 16), Label("start", Label::Code | Label::Global)))
    return false;
  return true;
}