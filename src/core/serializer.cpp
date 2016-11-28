#include "medusa/serializer.hpp"

MEDUSA_NAMESPACE_BEGIN

bool Serializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::MemoryArea const& rMemArea){}
bool Serializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::Address const& rAddr){}
bool Serializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::Label const& rLabel){}
bool Serializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::CellData const& rCellData){}
bool Serializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::MappedMemoryArea const& rMappedMemArea){}
bool Serializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::VirtualMemoryArea const& rVirtualMemArea){}

bool Serializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::MemoryArea const& rMemArea){}
bool Serializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::Address const& rAddr){}
bool Serializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::Label const& rLabel){}
bool Serializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::CellData const& rCellData){}
bool Serializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::MappedMemoryArea const& rMappedMemArea){}
bool Serializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::VirtualMemoryArea const& rVirtualMemArea){}

MEDUSA_NAMESPACE_END
