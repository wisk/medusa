#ifndef SOCI_SERIALIZER_HPP
# define SOCI_SERIALIZER_HPP

#include <medusa/serializer.hpp>
#include "soci_db.hpp"

class SociSerializer : public Serializer
{
public:
  SociSerializer(soci::session& Session);
  virtual ~SociSerializer(){}
  virtual void print(){ std::cout << "Hello, Soci serializer" << std::endl; }
  virtual bool LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::MemoryArea const& rMemArea);
  virtual bool LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::Address const& rAddr);
  virtual bool LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::Label const& rLabel);
  virtual bool LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::CellData const& rCellData);
  virtual bool LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::MappedMemoryArea const& rMappedMemArea);
  virtual bool LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::VirtualMemoryArea const& rVirtualMemArea);

  virtual bool SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::MemoryArea const& rMemArea);
  virtual bool SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::Address const& rAddr);
  virtual bool SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::Label const& rLabel);
  virtual bool SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::CellData const& rCellData);
  virtual bool SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::MappedMemoryArea const& rMappedMemArea);
  virtual bool SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::VirtualMemoryArea const& rVirtualMemArea);

private:
  soci::session& m_Session;
};

#endif // !SOCI_SERIALIZER_HPP
