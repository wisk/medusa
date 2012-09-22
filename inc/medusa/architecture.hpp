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
#include "medusa/database.hpp"

#include "medusa/character.hpp"
#include "medusa/value.hpp"
#include "medusa/instruction.hpp"

#include "medusa/function.hpp"
#include "medusa/string.hpp"

#include <map>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

MEDUSA_NAMESPACE_BEGIN

/*! This class defines what an architecture plug-in must implement.
 * Don't forget to export a extern "C" Architecture* GetArchitecture(void) function
 */
class Medusa_EXPORT   Architecture
{
public:
  typedef boost::shared_ptr<Architecture> SharedPtr;
  typedef std::vector<SharedPtr>          VectorSharedPtr;
  typedef std::list<SharedPtr>            ListSharedPtr;
  typedef std::map<Tag, SharedPtr>        TagMap;

  Architecture(Tag ArchTag) : m_Tag(ArchTag) {}

  //! This method returns the name of the current architecture.
  virtual std::string GetName(void) = 0;

  //! This method converts an virtual address to a physical one.
  virtual bool        Translate(Address const& rVirtAddr, TOffset& rPhysOff) = 0;

  //! This method disassembles one instruction.
  virtual bool        Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn) = 0;

  //! This method fills a configuration object.
  virtual void        FillConfigurationModel(ConfigurationModel& rCfgMdl) = 0;

  //! This method returns the architecture endianness.
  virtual EEndianness GetEndianness(void) = 0;

  void                UpdateId(u8 Id) { m_Tag |= Id; }

  Tag                 GetTag(void) const { return m_Tag; }

  //! This method allows to configure the current architecture.
  void                UseConfiguration(Configuration const& rCfg) { m_Cfg = rCfg; }

  //! This method allows architecture to format cell as it wants.
  //\param rDatabase is needed if rCell contains a reference.
  //\param rBinStrm must be the binary stream of the memory area where rCell is located.
  //\param rAddr is the address of rCell.
  //\param rCell is the cell object.
  virtual void        FormatCell( Database      const& rDatabase,
                                  BinaryStream  const& rBinStrm,
                                  Address       const& rAddress,
                                  Cell               & rCell);

  //! This method allows architecture to generate an instruction LLVM ir instructions
  virtual void        ConvertInstructionToLlvmIr( Database     const& rDatabase,
                                                  BinaryStream const& rBinStrm,
                                                  Address      const& rAddress,
                                                  Cell         const& rCell)
  {
    throw Exception_NotImplemented(L"LLVM IR");
  }

  //! This method converts an Instruction object to a string and stores the result on it.
  //\param rDatabase is needed if an operand contains a reference.
  //\param rAddr is the address of rInsn.
  //\param rInsn is the cell object.
  void                DefaultFormatInstruction( Database const& rDatabase,
                                                BinaryStream const& rBinStrm,
                                                Address const& rAddr,
                                                Instruction& rInsn);

  //! This method reads and convert a character.
  //\param rDatabase is reserved for future use.
  //\param rBinStrm must be the binary stream of the memory area where rChar is located.
  //\param rAddr is the address of rChar.
  //\param rChar is the cell object.
  void                DefaultFormatCharacter(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddr, Character& rChar);

  //! This method reads and convert a numeric value.
  //\param rDatabase is needed if rVal contains a reference.
  //\param rBinStrm must be the binary stream of the memory area where rVal is located.
  //\param rAddr is the address of rVal.
  //\param rVal is the cell object.
  void                DefaultFormatValue   (Database      const& rDatabase,
                                            BinaryStream  const& rBinStrm,
                                            Address       const& rAddr,
                                            Value              & rVal);

  virtual void        FormatMultiCell      (Database     const& rDatabase,
                                            BinaryStream const& rBinStrm,
                                            Address      const& rAddress,
                                            MultiCell         & rMultiCell);

  void                DefaultFormatString  (Database     const& rDatabase,
                                            BinaryStream const& rBinStrm,
                                            Address      const& rAddr,
                                            String            & rStr);

  void                DefaultFormatFunction(Database     const& rDatabase,
                                            BinaryStream const& rBinStrm,
                                            Address      const& rAddr,
                                            Function          & rFunc);

protected:
  Configuration m_Cfg;
  Tag           m_Tag;
};

typedef Architecture* (*TGetArchitecture)(void);

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_ARCHITECTURE_