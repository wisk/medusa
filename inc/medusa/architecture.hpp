#ifndef _MEDUSA_ARCHITECTURE_
#define _MEDUSA_ARCHITECTURE_

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/exception.hpp"
#include "medusa/types.hpp"
#include "medusa/endian.hpp"
#include "medusa/configuration.hpp"
#include "medusa/address.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/context.hpp"
#include "medusa/expression.hpp"

#include "medusa/character.hpp"
#include "medusa/value.hpp"
#include "medusa/instruction.hpp"
#include "medusa/string.hpp"

#include "medusa/function.hpp"
#include "medusa/structure.hpp"

#include "medusa/cell_text.hpp"

#include <map>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

MEDUSA_NAMESPACE_BEGIN

/*! This class defines what an architecture plug-in must implement.
 * Don't forget to export a extern "C" Architecture* GetArchitecture(void) function
 */
class Medusa_EXPORT Architecture : public IsConfigurable
{
public:
  typedef boost::shared_ptr<Architecture> SharedPtr;
  typedef std::vector<SharedPtr>          VectorSharedPtr;
  typedef std::list<SharedPtr>            ListSharedPtr;
  typedef std::map<Tag, SharedPtr>        TagMap;
  typedef std::tuple<const char*, u8>     NamedMode;
  typedef std::vector<NamedMode>          NamedModeVector;

  Architecture(Tag ArchTag);

  //! This method returns the name of the current architecture.
  virtual std::string GetName(void) const = 0;

  //! This method converts an virtual address to a physical one.
  virtual bool Translate(Address const& rVirtAddr, TOffset& rPhysOff) = 0;

  //! This method disassembles one instruction.
  virtual bool Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode) = 0;

  //! This method returns all available mode
  virtual NamedModeVector GetModes(void) const = 0;
  u8 GetModeByName(std::string const &rModeName) const;

  //! This method helps the analyzer to guess the correct mode
  virtual u8 GetDefaultMode(Address const& rAddress) const { return 0; }

  //! This method returns the architecture endianness.
  virtual EEndianness GetEndianness(void) = 0;

  virtual CpuInformation const* GetCpuInformation(void) const = 0;
  virtual CpuContext*           MakeCpuContext(void)    const = 0;
  virtual MemoryContext*        MakeMemoryContext(void) const = 0;

  virtual bool HandleSystemExpression(Expression::List& rExprs, std::string const& rName, Instruction& rInsn) { return true; }

  void UpdateId(u8 Id) { m_Tag |= Id; }
  Tag  GetTag(void) const { return m_Tag; }

  void AnalyzeFunction(Function const& rFunc);

  bool                      DisassembleBasicBlockOnly(void) const { return m_CfgMdl.GetBoolean("Disassembly only basic block"); }

  bool FormatTypeDetail(
    TypeDetail const& rTypeDtl,
    PrintData       & rPrintData) const;

  bool FormatValueDetail(
    Document    const& rDoc,
    Address     const& rAddr,
    u8                 ValueBits,
    ValueDetail const& rValDtl,
    PrintData        & rPrintData) const;

  //! This method allows architecture to format cell as it wants.
  //\param rDoc is needed if rCell contains a reference.
  //\param rAddr is the address of rCell.
  //\param rCell is the cell object.
  bool FormatCell(
    Document      const& rDoc,
    Address       const& rAddress,
    Cell          const& rCell,
    PrintData          & rPrintData) const;

  //! This method converts an Operand object to a string
  //\param rDoc is needed if an operand contains a reference.
  //\param rAddr is the address of rInsn.
  //\param rInsn is the cell object.
  virtual bool FormatOperand(
    Document      const& rDoc,
    Address       const& rAddress,
    Instruction   const& rInstruction,
    Operand       const& rOperand,
    u8                   OperandNo,
    PrintData          & rPrintData) const;

  //! This method converts an Instruction object to a string.
  //\param rDoc is needed if an operand contains a reference.
  //\param rAddr is the address of rInsn.
  //\param rInsn is the cell object.
  virtual bool FormatInstruction(
    Document      const& rDoc,
    Address       const& rAddr,
    Instruction   const& rInsn,
    PrintData          & rPrintData) const;

  //! This method reads and convert a character.
  //\param rDoc is reserved for future use.
  //\param rAddr is the address of rChar.
  //\param rChar is the cell object.
  virtual bool FormatCharacter(
    Document      const& rDoc,
    Address       const& rAddr,
    Character     const& rChar,
    PrintData          & rPrintData) const;

  //! This method reads and convert a numeric value.
  //\param rDoc is needed if rVal contains a reference.
  //\param rAddr is the address of rVal.
  //\param rVal is the cell object.
  virtual bool FormatValue(
    Document      const& rDoc,
    Address       const& rAddr,
    Value         const& rVal,
    PrintData          & rPrintData) const;

  virtual bool FormatString(
    Document      const& rDoc,
    Address       const& rAddr,
    String        const& rStr,
    PrintData          & rPrintData) const;

  bool FormatMultiCell(
    Document      const& rDoc,
    Address       const& rAddress,
    MultiCell     const& rMultiCell,
    PrintData          & rPrintData) const;

  virtual bool FormatFunction(
    Document      const& rDoc,
    Address       const& rAddr,
    Function      const& rFunc,
    PrintData          & rPrintData) const;

  virtual bool FormatStructure(
    Document      const& rDoc,
    Address       const& rAddr,
    Structure     const& rStruct,
    PrintData          & rPrintData) const;

protected:
  Tag                m_Tag;
};

typedef Architecture* (*TGetArchitecture)(void);

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_ARCHITECTURE_
