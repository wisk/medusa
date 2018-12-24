#include "raw_loader.hpp"

bool RawLoader::Map(Document& rDoc) const
{
  return rDoc.AddMemoryArea(MemoryArea::CreateMapped(
    "raw", MemoryArea::Access::Execute | MemoryArea::Access::Read | MemoryArea::Access::Write,
    0x0, rDoc.GetBinaryStream().GetSize(),
    Address(Address::LinearType, 0x0), rDoc.GetBinaryStream().GetSize()
    ));
}