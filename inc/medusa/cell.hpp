#ifndef _MEDUSA_CELL_
#define _MEDUSA_CELL_

#include <string>
#include <forward_list>

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/serialize.hpp"

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

//! Cell is a base element of a memory entity.
class Medusa_EXPORT Cell// : public SerializeAccess
{
public:
  enum Type : u8
  {
    CellType,         //! Undefined cell.
    InstructionType,  //! Instruction cell.
    ValueType,        //! Value cell.
    CharacterType,    //! Character cell.
    StringType        //! String cell.
  };

  class Mark
  {
  public:
    typedef std::list<Mark> List;

    enum Type : u8
    {
      UnknownType,
      MnemonicType,
      RegisterType,
      ImmediateType,
      LabelType,
      KeywordType,
      OperatorType,
      CharacterType,
      StringType
    };

    Mark(Type Type = UnknownType, u16 Length = 0)
      : m_Type(Type), m_Length(Length)
    {}

    Type GetType(void)   const { return m_Type;   }
    u16  GetLength(void) const { return m_Length; }

  private:
    Type m_Type;
    u16  m_Length;
  };

  /*! Cell constructor.
   * \param Type defines the type of cell @see Type.
   * \param rBuffer is the text representation of the current cell.
   * \param rComment is a the comment for the current cell.
   */
  Cell(
    Cell::Type Type = CellType,
    std::string const& rBuffer = "",
    std::string const& rComment = ""
    )
    : m_Type(Type)
    , m_Buffer(rBuffer)
    , m_Comment(rComment)
  {}

  virtual ~Cell(void) {}

  //! This method returns the current comment.
  std::string&          Comment(void) { return m_Comment; }

  //! This method returns the current comment in read-only.
  std::string const&    GetComment(void) const { return m_Comment; }

  //! This method allows to change the current comment.
  void                  SetComment(std::string const& rComment) { m_Comment = rComment; }

  //! This method returns the string of the current cell.
  std::string const& ToString(void) const { return m_Buffer; }

  //! This method update the string of the current cell. This method requires to notify the current database.
  void UpdateString(std::string const& rString) { m_Buffer = rString; }

  //! This method returns the size of this cell.
  virtual size_t        GetLength(void) const { return 1; }

  //! This method returns the type of this cell.
  Type                  GetType(void) const { return m_Type; }

  Mark::List const&     GetMarks(void) const { return m_Marks; }
  void                  ResetMarks(void) { m_Marks = Mark::List(); }
  void                  AddMark(Mark const& rMark) { m_Marks.push_back(rMark); }
  void                  AddMark(Mark::Type Type, size_t Offset) { m_Marks.push_back(Mark(Type, static_cast<u16>(Offset))); }

  //virtual void                  Load(SerializeEntity::SPtr SrlzEtt);
  //virtual SerializeEntity::SPtr Save(void);

protected:
  Type                  m_Type;
  std::string           m_Buffer;
  std::string           m_Comment;
  Mark::List            m_Marks;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_CELL_