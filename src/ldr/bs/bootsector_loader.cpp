#include "medusa/medusa.hpp"
#include <boost/foreach.hpp>

#include "bootsector_loader.hpp"

TOffset BootSectorLoader::AddressOffset = 0x7c00;

BootSectorLoader::BootSectorLoader(Document& rDoc)
  : Loader(rDoc)
  , m_rDoc(rDoc)
  , m_IsValid(false)
{
  if (rDoc.GetFileBinaryStream().GetSize() != 0x200)
    return;

  u16 Signature;
  rDoc.GetFileBinaryStream().Read(0x200 - 2, Signature);
  if (Signature != 0xAA55)
    return;

  m_IsValid = true;
}

void BootSectorLoader::Map(void)
{
  m_rDoc.AddMemoryArea(new MappedMemoryArea(
    "mem",
    0x0, 0x200,
    Address(Address::FlatType, 0x0, AddressOffset, 16, 16), 0x200,
    MemoryArea::Read | MemoryArea::Write | MemoryArea::Execute
  ));
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