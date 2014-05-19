#ifndef __MEDUSA_STRUCT_HPP__
#define __MEDUSA_STRUCT_HPP__

#include <list>

#include "medusa/namespace.hpp"
#include "medusa/multicell.hpp"

#include <boost/uuid/uuid.hpp>

MEDUSA_NAMESPACE_BEGIN

/*!
 **
 ** Field type:
 **  - {s,u}{8,16,32,64}
 **  - float not implemented
 **  - composite → include an another element
 **  - reference → pointer to an another element
 **  - relative  → the another element is located at the beginning of struct +
 **  field value
 **  - intrusive → the another element is located at field value - offsetof field
 **
 */
class StructureDefinition
{
public:
  typedef boost::uuids::uuid Id;
protected:
};

//! Structure is a MultiCell which handles a group of value
class Structure : public MultiCell
{
public:

protected:
  StructureDefinition::Id m_Id;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_STRUCT_HPP__
