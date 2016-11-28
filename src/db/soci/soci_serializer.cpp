#include "soci_serializer.hpp"


SociSerializer::SociSerializer(soci::session& Session)
  : m_Session(Session)
{
}

bool SociSerializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::MemoryArea const& rMemArea){}
bool SociSerializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::Address const& rAddr){}
bool SociSerializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::Label const& rLabel){}
bool SociSerializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::CellData const& rCellData){}
bool SociSerializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::MappedMemoryArea const& rMappedMemArea){}
bool SociSerializer::LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::VirtualMemoryArea const& rVirtualMemArea){}

bool SociSerializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::MemoryArea const& rMemArea){}
bool SociSerializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::Address const& rAddr){}
bool SociSerializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::Label const& rLabel){}
bool SociSerializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::CellData const& rCellData){}
bool SociSerializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::MappedMemoryArea const& rMappedMemArea){}
bool SociSerializer::SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::VirtualMemoryArea const& rVirtualMemArea){}
