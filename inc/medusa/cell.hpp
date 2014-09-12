#ifndef MEDUSA_CELL_HPP
#define MEDUSA_CELL_HPP

#include <list>
#include <string>
#include <forward_list>
#include <memory>

#include <medusa/cell_data.hpp>

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

//! Cell is a base element of a memory entity.
class Medusa_EXPORT Cell
{
public:
  enum Type
  {
    CellType,         //! Undefined cell.
    InstructionType,  //! Instruction cell.
    ValueType,        //! Value cell.
    CharacterType,    //! Character cell.
    StringType        //! String cell.
  };

  typedef std::shared_ptr<Cell> SPType;

  /*! Cell constructor.
   * \param Type defines the type of cell @see Type.
   * \param rBuffer is the text representation of the current cell.
   * \param rComment is a the comment for the current cell.
   */
  Cell(
    u8 Type, u8 SubType,
    u16 Size = 0x0
    )
  {
    m_spDna = std::make_shared<CellData>(Type, SubType, Size);
  }

  Cell(CellData::SPType spDna) : m_spDna(spDna) {}

  virtual ~Cell(void) { }

  //! This method returns the size of this cell.
  virtual size_t        GetLength(void) const { return m_spDna->GetLength(); }

  //! This method returns the type of this cell.
  u8 GetType(void) const { return m_spDna->GetType(); }
  u8 GetSubType(void) const { return m_spDna->GetSubType(); }
  u8 GetMode(void) const { return m_spDna->GetMode(); }
  u8& SubType(void) { return m_spDna->SubType(); }
  u8& Mode(void) { return m_spDna->Mode(); }

  //! This method returns the used architecture tag.
  Tag GetArchitectureTag(void) const { return m_spDna->GetArchitectureTag(); }

  CellData::SPType GetData(void) { return m_spDna; }

  //! This method returns the internal data

protected:
  CellData::SPType m_spDna;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CELL_HPP