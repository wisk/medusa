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
}

void Avr8Loader::Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs)
{
    
}

std::string Avr8Loader::GetName(void) const
{
  return "Avr8 file";
}