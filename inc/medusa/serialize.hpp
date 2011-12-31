#ifndef _MEDUSA_SERIALIZE_
#define _MEDUSA_SERIALIZE_

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/export.hpp"
#include "medusa/exception.hpp"
#include "medusa/binary_stream.hpp"

#include <string>
#include <list>
#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

#include <iostream>

/*
Medusa serialization is organized as tree.
At this time, scheme is fixed as follow:
Root (1)
\_    Architecture (*)
\_    Loader (1)
\_    Database (1)
\_        MemoryArea (+)
\_          Cell (*)
\_            Operand(*)
*/

MEDUSA_NAMESPACE_BEGIN

//! SerializeEntity contains the description of a medusa object to the serialized.
class Medusa_EXPORT SerializeEntity
{
public:
  typedef boost::shared_ptr<SerializeEntity>  SPtr;
  typedef std::list<SPtr>                     SPtrList;

  typedef boost::variant<
    s8, u8, /**/ s16, u16, /**/ s32, u32, /**/ s64, u64,
    std::string,
    Address,
    BinaryStream
  > SerializeDataTypes;
  typedef std::map<std::string, SerializeDataTypes> SerializeFields;

  typedef enum
  {
    SqliteSerializeLoader = 0,
    SqliteSerializeArchitecture = 1,
    SqliteSerializeDatabase = 2,
    SqliteSerializeMemoryArea = 3,
    SqliteSerializeCell = 4
  } SqliteSerializeType;

  SerializeEntity(std::string const& rEntityName)
    : m_Name(rEntityName)
  {}

  virtual ~SerializeEntity(void) {}

  SPtr CreateSubEntity(std::string const& rSubEntityName)
  {
    SPtr spNewEntity(new SerializeEntity(rSubEntityName));
    m_SubEntities.push_back(spNewEntity);
    return spNewEntity;
  }

  void AddSubEntity(SPtr spSubEntity)
  {
    m_SubEntities.push_back(spSubEntity);
  }

  void AddField(std::string const& rFieldName, SerializeDataTypes Data)
  {
    m_Fields[rFieldName] = Data;
  }

  template<typename T>
  void GetField(std::string const& rFieldName, T& Data)
  {
    SerializeFields::iterator It = m_Fields.find(rFieldName);
    if (It == m_Fields.end())
    {
      Data = T();
      return;
    }

    Data =  boost::get<T>(It->second);
  }

  std::string const&        GetName(void) const               { return m_Name;  }
  void                      SetName(std::string const& rName) { m_Name = rName; }

  SerializeFields::const_iterator BeginField(void) const { return m_Fields.begin(); }
  SerializeFields::const_iterator EndField(void)   const { return m_Fields.end();   }

  SPtrList::const_iterator  BeginSubEntities(void)   const { return m_SubEntities.begin(); }
  SPtrList::const_iterator  EndSubEntities(void)     const { return m_SubEntities.end();   }

private:
  std::string         m_Name;
  SerializeFields     m_Fields;
  SPtrList            m_SubEntities;
};

//! SerializeAccess allows a class to expose attribut for serialization.
class SerializeAccess
{
public:
  virtual void                  Load(SerializeEntity::SPtr spSrlzEtt) = 0;
  virtual SerializeEntity::SPtr Save(void)                            = 0;
};

// Dont forget to export a extern "C" medusa::Serialize* GetSerialize(void) function
/*! Serialize is the serialize interface.
 */
class Medusa_EXPORT Serialize
{
public:
  typedef Serialize*  Ptr;
  typedef boost::shared_ptr<Serialize> SPtr;
  typedef std::vector<SPtr> VectorSPtr;

  Serialize(void)
    : m_spRootEntity(new SerializeEntity("root"))
  {}

  virtual  ~Serialize(void) {}

  SerializeEntity::SPtr GetRoot(void) { return m_spRootEntity; }

  virtual void Open(std::string const& rFilename) { throw Exception_NotImplemented(L"Open"); }
  virtual void Load(void)                         { throw Exception_NotImplemented(L"Load"); }
  virtual void Save(void)                         { throw Exception_NotImplemented(L"Save"); }

protected:
  SerializeEntity::SPtr m_spRootEntity;
};

typedef Serialize* (*TGetSerialize)(void);

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_SERIALIZE_
