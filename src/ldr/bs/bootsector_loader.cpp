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

void BootSectorLoader::Map(Document& rDoc)
{
  rDoc.AddMemoryArea(new MappedMemoryArea(
    "mem",
    0x0, 0x200,
    Address(Address::FlatType, 0x0, AddressOffset, 16, 16), 0x200,
    MemoryArea::Read | MemoryArea::Write | MemoryArea::Execute
  ));

  rDoc.AddLabel(Address(Address::FlatType, 0x0, AddressOffset, 16, 16), Label("start", Label::Code | Label::Global));
}

Architecture::SharedPtr BootSectorLoader::GetMainArchitecture(Architecture::VectorSharedPtr const& rArchitectures)
{
  if (rArchitectures.size() > 0)
    BOOST_FOREACH(Architecture::SharedPtr pArchitecture, rArchitectures)
    {
      if (pArchitecture->GetName() == "Intel x86")
        return pArchitecture;
    }
  return Architecture::SharedPtr();
}

void BootSectorLoader::Configure(Configuration& rCfg)
{
  rCfg.Set("Bit", 16);
}