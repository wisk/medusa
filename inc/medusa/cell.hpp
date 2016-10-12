#ifndef MEDUSA_CELL_HPP
#define MEDUSA_CELL_HPP

#include <list>
#include <string>
#include <forward_list>
#include <memory>

#include <medusa/cell_data.hpp>

MEDUSA_NAMESPACE_BEGIN

//! Cell is a base element of a memory entity.
class MEDUSA_EXPORT Cell
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

  size_t GetSize(void)    const { return m_spDna->GetSize(); }
  u8     GetType(void)    const { return m_spDna->GetType(); }
  u8     GetSubType(void) const { return m_spDna->GetSubType(); }
  u8     GetMode(void)    const { return m_spDna->GetMode(); }

  void SetArchitectureTag(Tag ArchTag) { m_spDna->SetArchitectureTag(ArchTag); }
  void SetMode(u8 Mode)                { m_spDna->SetMode(Mode);               }

  u8& SubType(void) { return m_spDna->SubType(); }

  //! This method returns the used architecture tag.
  Tag GetArchitectureTag(void) const { return m_spDna->GetArchitectureTag(); }

  CellData::SPType GetData(void) { return m_spDna; }

  //! This method returns the internal data

protected:
  CellData::SPType m_spDna;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CELL_HPP