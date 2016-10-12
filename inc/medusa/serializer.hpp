#ifndef SERIALIZER_HPP
# define SERIALIZER_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

MEDUSA_NAMESPACE_BEGIN

class Serializer
{
public:
  virtual ~Serializer(){}
  virtual bool LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::MemoryArea const& rMemArea);
  virtual bool LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::Address const& rAddr);
  virtual bool LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::Label const& rLabel);
  virtual bool LoadField(char const* pFieldName, medusa::Address const& rAddress, medusa::CellData const& rCellData);

  virtual bool SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::MemoryArea const& rMemArea);
  virtual bool SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::Address const& rAddr);
  virtual bool SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::Label const& rLabel);
  virtual bool SaveField(char const* pFieldName, medusa::Address const& rAddress, medusa::CellData const& rCellData);
};

MEDUSA_NAMESPACE_END

#endif // !SERIALIZER_HPP
