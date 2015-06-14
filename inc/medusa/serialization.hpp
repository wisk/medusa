#ifndef MEDUSA_SERIALIZATION_HPP
#define MEDUSA_SERIALIZATION_HPP

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT Serializer
{
public:
  virtual bool LoadField(char const* pFieldName, u32 Index, void const* pRawData, u32 RawSize) = 0;
  virtual bool LoadField(char const* pFieldName, u32 Index, IntType const& rInt) = 0;
  virtual bool LoadField(char const* pFieldName, u32 Index, Address const& rAddr) = 0;
  virtual bool LoadField(char const* pFieldName, u32 Index, std::string const& rStr) = 0;

  virtual bool SaveField(char const* pFieldName, u32 Index, void const* pRawData, u32 RawSize) = 0;
  virtual bool SaveField(char const* pFieldName, u32 Index, IntType const& rInt) = 0;
  virtual bool SaveField(char const* pFieldName, u32 Index, Address const& rAddr) = 0;
  virtual bool SaveField(char const* pFieldName, u32 Index, std::string const& rStr) = 0;
};

class Medusa_EXPORT IsSerializable
{
public:
  virtual bool Load(Serializer& rSrlz) = 0;
  virtual bool Save(Serializer& rSrlz) = 0;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_SERIALIZATION_HPP
