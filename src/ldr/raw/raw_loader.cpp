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
    "raw", MemoryArea::Access::Execute | MemoryArea::Access::Read | MemoryArea::Access::Write,
    0x0, rDoc.GetBinaryStream().GetSize(),
    Address(Address::LinearType, 0x0), rDoc.GetBinaryStream().GetSize()
    ));
}

bool RawLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs, Address const& rImgBase)
{
  return false;
}

void RawLoader::FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const
{
}