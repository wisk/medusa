#include "medusa/medusa.hpp"
#include "avr8_loader.hpp"

Avr8Loader::Avr8Loader(void)
{
}

bool Avr8Loader::IsCompatible(BinaryStream const& rBinStrm)
{
  return true;
}

void Avr8Loader::FilterAndConfigureArchitectures(Architecture::VectorSharedPtr& rArchs) const
{
   // rArchs.erase(std::remove_if(std::begin(rArchs), std::end(rArchs), [](Architecture::SharedPtr spArch)
 // { return spArch->GetName() != "Atmel AVR 8-bit"; }), std::end(rArchs));
}

void Avr8Loader::Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs)
{
    //TODO I I'll write loader,but i want to sleep now...
    rDoc.AddMemoryArea(new MappedMemoryArea(
    "avr8",
    0x0, rDoc.GetBinaryStream().GetSize(),
    Address(Address::FlatType, 0x0), rDoc.GetBinaryStream().GetSize(),
    MemoryArea::Execute | MemoryArea::Read | MemoryArea::Write
    ));
}

std::string Avr8Loader::GetName(void) const
{
  return "Avr8 file";
}