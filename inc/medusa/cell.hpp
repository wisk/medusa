#ifndef _MEDUSA_CELL_
#define _MEDUSA_CELL_

#include <list>
#include <string>
#include <forward_list>

#include <medusa/cell_data.hpp>

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

//! Cell is a base element of a memory entity.
class Medusa_EXPORT Cell
{
public:
  // TODO: Move this class outside Cell scope
  class Mark
  {
  public:
    typedef std::list<Mark> List;

    enum Type
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

    Mark(Type Type = UnknownType, size_t Length = 0)
      : m_Type(Type), m_Length(static_cast<u16>(Length))
    {}

    u16 GetType(void)   const { return m_Type;   }
    u16 GetLength(void) const { return m_Length; }

  private:
    u16 m_Type;
    u16  m_Length;
  };

  /*! Cell constructor.
   * \param Type defines the type of cell @see Type.
   * \param rBuffer is the text representation of the current cell.
   * \param rComment is a the comment for the current cell.
   */
  Cell(
    CellData::Type Type = CellData::CellType,
    std::string const& rBuffer = "",
    std::string const& rComment = ""
    )
    : m_Comment(rComment)
  {
    m_pDna = new CellData(Type, 1, 0x0, MEDUSA_ARCH_UNK);
  }

  virtual ~Cell(void) { delete m_pDna; }

  //! This method returns the current comment.
  std::string&          Comment(void) { return m_Comment; }

  //! This method returns the current comment in read-only.
  std::string const&    GetComment(void) const { return m_Comment; }

  //! This method allows to change the current comment.
  void                  SetComment(std::string const& rComment) { m_Comment = rComment; }

  //! This method returns the size of this cell.
  virtual size_t        GetLength(void) const { return m_pDna->GetLength(); }

  //! This method returns the type of this cell.
  CellData::Type GetType(void) const { return m_pDna->GetType(); }

  //! This method returns the used architecture tag.
  Tag GetArchitectureTag(void) const { return m_pDna->GetArchitectureTag(); }

  //! This method returns the internal data

protected:
  CellData*   m_pDna;
  std::string m_Comment;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_CELL_