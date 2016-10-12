#include "raw_loader.hpp"

std::string RawLoader::GetName(void) const
{
  return "Raw file";
}

bool RawLoader::IsCompatible(BinaryStream const& rBinStrm)
{
  return true;
}

bool RawLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs)
{
  return rDoc.AddMemoryArea(MemoryArea::CreateMapped(
    "raw", MemoryArea::Execute | MemoryArea::Read | MemoryArea::Write,
    0x0, rDoc.GetBinaryStream().GetSize(),
    Address(Address::VirtualType, 0x0), rDoc.GetBinaryStream().GetSize()
    ));
}

bool RawLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs, Address const& rImgBase)
{
  return false;
}

void RawLoader::FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const
{
}