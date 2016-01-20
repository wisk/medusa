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
  rDoc.AddMemoryArea(new MappedMemoryArea(
    "raw",
    0x0, rDoc.GetBinaryStream().GetSize(),
    Address(Address::FlatType, 0x0), rDoc.GetBinaryStream().GetSize(),
    MemoryArea::Execute | MemoryArea::Read | MemoryArea::Write
    ));

  return true;
}

bool RawLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs, Address const& rImgBase)
{
  return false;
}

void RawLoader::FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const
{
}