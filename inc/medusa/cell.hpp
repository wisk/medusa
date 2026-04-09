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
  enum class Type : u8
  {
    CellType,         //! Undefined cell.
    InstructionType,  //! Instruction cell.
    ValueType,        //! Value cell.
    CharacterType,    //! Character cell.
    StringType        //! String cell.
  };

  // Backward-compatible aliases so Cell::InstructionType etc. still compile
  static constexpr Type CellType        = Type::CellType;
  static constexpr Type InstructionType = Type::InstructionType;
  static constexpr Type ValueType       = Type::ValueType;
  static constexpr Type CharacterType   = Type::CharacterType;
  static constexpr Type StringType      = Type::StringType;

  using SPType = std::shared_ptr<Cell>;

  /*! Cell constructor.
   * \param Type defines the type of cell @see Type.
   * \param rBuffer is the text representation of the current cell.
   * \param rComment is a the comment for the current cell.
   */
  Cell(
    Type CellType, u8 SubType,
    u16 Size = 0x0
    )
  {
    m_spDna = std::make_shared<CellData>(static_cast<u8>(CellType), SubType, Size);
  }

  explicit Cell(CellData::SPType spDna) : m_spDna(spDna) {}

  virtual ~Cell() = default;

  size_t GetSize()    const { return m_spDna->GetSize(); }
  Type   GetType()    const { return static_cast<Type>(m_spDna->GetType()); }
  u8     GetSubType() const { return m_spDna->GetSubType(); }
  u8     GetMode()    const { return m_spDna->GetMode(); }

  void SetArchitectureTag(Tag ArchTag) { m_spDna->SetArchitectureTag(ArchTag); }
  void SetMode(u8 Mode)                { m_spDna->SetMode(Mode);               }

  u8& SubType() { return m_spDna->SubType(); }

  //! This method returns the used architecture tag.
  Tag GetArchitectureTag() const { return m_spDna->GetArchitectureTag(); }

  CellData::SPType GetData() { return m_spDna; }

  //! This method returns the internal data

protected:
  CellData::SPType m_spDna;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CELL_HPP