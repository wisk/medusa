#include "medusa/medusa.hpp"
#include <boost/foreach.hpp>

#include "bootsector_loader.hpp"

TOffset BootSectorLoader::AddressOffset = 0x7c00;

BootSectorLoader::BootSectorLoader(Database& rDatabase)
  : Loader(rDatabase)
  , m_rDatabase(rDatabase)
  , m_IsValid(false)
{
  if (rDatabase.GetFileBinaryStream().GetSize() != 0x200)
    return;

  u16 Signature;
  rDatabase.GetFileBinaryStream().Read(0x200 - 2, Signature);
  if (Signature != 0xAA55)
    return;

  m_IsValid = true;
}

void BootSectorLoader::Map(void)
{
  m_rDatabase.AddMemoryArea(new MappedMemoryArea(
    m_rDatabase.GetFileBinaryStream(), "mem",
    Address(Address::PhysicalType, 0x0),                    0x200,
    Address(Address::FlatType, 0x0, AddressOffset, 16, 16), 0x200,
    MA_READ | MA_WRITE | MA_EXEC
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