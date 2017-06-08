#ifndef MEDUSA_MULTICELL_HPP
#define MEDUSA_MULTICELL_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/address.hpp"
#include "medusa/graph.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>

#include <string>
#include <map>

MEDUSA_NAMESPACE_BEGIN

//! MultiCell is a group of cell.
class MEDUSA_EXPORT MultiCell
{
public:
  typedef std::shared_ptr<MultiCell> SPType;

  enum Type
  {
    UnknownType,
    FunctionType,
    StructType,
    ArrayType
  };

  MultiCell(u8 Type = UnknownType, u16 Size = 0x0)
    : m_Type(Type)
    , m_Size(Size)
  {}

  std::string Dump(void) const;

  //! This method returns the multicell type.
  u8 GetType(void) const { return m_Type; }

  //! This method returns the size of multicell.
  u16 GetSize(void) const { return m_Size; }

  Graph::SPType GetGraph(void)                  { return m_spGraph;    }
  void          SetGraph(Graph::SPType spGraph) { m_spGraph = spGraph; }

protected:
  u8            m_Type;
  u16           m_Size;
  Graph::SPType m_spGraph;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_MULTICELL_HPP