#ifndef __MEDUSA_STRUCT_HPP__
#define __MEDUSA_STRUCT_HPP__

#include <list>

#include "medusa/namespace.hpp"
#include "medusa/multicell.hpp"

/*

//! Structure is a MultiCell which handles a group of value
class Structure : public MultiCell
{
public:
  class Field
  {
  public:
    typedef std::list<Field> List;

    enum Type
    {
      Integer,
      String,
    };

    Field(std::string const& rName, Type Type, u16 Size);

  protected:
    std::string m_Name;
    Type m_Type;
    u16 m_Size;
  };

  typedef std::list<Structure> List;

  Structure(std::string const& rName, Field::List const& rFields);

protected:
  std::string m_Name;
  Field::List m_Fields;
};

*/

#endif // !__MEDUSA_STRUCT_HPP__
