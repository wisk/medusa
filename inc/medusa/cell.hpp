#ifndef _MEDUSA_CELL_
#define _MEDUSA_CELL_

#include <string>

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/serialize.hpp"

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

//! Cell is a base element of a memory entity.
class Medusa_EXPORT Cell : public SerializeAccess
{
public:
  enum Type
  {
    CellType,         //! Undefined cell.
    InstructionType,  //! Instruction cell.
    ValueType,        //! Value cell.
    CharacterType     //! Character cell.
  };

  /*! Cell constructor.
   * \param Type defines the type of cell @see Type
   * \param rComment is a the comment for the current cell.
   */
  Cell(
    Cell::Type Type = CellType,
    std::string const& rComment = ""
    )
    : m_Type(Type)
    , m_Comment(rComment)
  {}

  virtual ~Cell(void) {}

  //! This method returns the current comment.
  std::string&          Comment(void) { return m_Comment; }

  //! This method returns the current comment in read-only.
  std::string const&    GetComment(void) const { return m_Comment; }

  //! This method allows to change the current comment.
  void                  SetComment(std::string const& rComment) { m_Comment = rComment; }

  //! This method converts the current in string.
  virtual std::string   ToString(void) const  { return ""; }

  //! This method returns the size of this cell.
  virtual size_t        GetLength(void) const { return 1;  }

  //! This method returns the type of this cell.
  Type                  GetType(void) const { return m_Type; }

  virtual void                  Load(SerializeEntity::SPtr SrlzEtt);
  virtual SerializeEntity::SPtr Save(void);

protected:
  Type                  m_Type;
  std::string           m_Comment;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_CELL_
