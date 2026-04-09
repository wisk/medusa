#ifndef MEDUSA_MULTICELL_HPP
#define MEDUSA_MULTICELL_HPP

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/address.hpp"
#include "medusa/graph.hpp"


#include <string>
#include <map>

MEDUSA_NAMESPACE_BEGIN

//! MultiCell is a group of cell.
class MEDUSA_EXPORT MultiCell
{
public:
  using SPType = std::shared_ptr<MultiCell>;

  enum class Type : u8
  {
    UnknownType,
    FunctionType,
    StructType,
    ArrayType
  };

  // Backward-compatible aliases so MultiCell::FunctionType etc. still compile
  static constexpr Type UnknownType  = Type::UnknownType;
  static constexpr Type FunctionType = Type::FunctionType;
  static constexpr Type StructType   = Type::StructType;
  static constexpr Type ArrayType    = Type::ArrayType;

  MultiCell(Type McType = Type::UnknownType, u16 Size = 0x0)
    : m_Type(McType)
    , m_Size(Size)
  {}

  std::string Dump() const;

  //! This method returns the multicell type.
  Type GetType() const { return m_Type; }

  //! This method returns the size of multicell.
  u16 GetSize() const { return m_Size; }

  Graph::SPType GetGraph()                  { return m_spGraph;    }
  void          SetGraph(Graph::SPType spGraph) { m_spGraph = spGraph; }

protected:
  Type          m_Type;
  u16           m_Size;
  Graph::SPType m_spGraph;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_MULTICELL_HPP