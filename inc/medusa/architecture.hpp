#ifndef _MEDUSA_ARCHITECTURE_
#define _MEDUSA_ARCHITECTURE_

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/endian.hpp"
#include "medusa/configuration.hpp"
#include "medusa/address.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/instruction.hpp"
#include "medusa/database.hpp"

#include <map>
#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

MEDUSA_NAMESPACE_BEGIN

typedef std::map<u32, std::string> TInfoMap;

/*! This class defines what an architecture plugin must implement.
 * Dont forget to export a extern "C" Architecture* GetArchitecture(void) function
 */
class Medusa_EXPORT   Architecture
{
public:
  typedef boost::shared_ptr<Architecture> Ptr;
  typedef std::vector<Ptr> VectorPtr;

  //! This method returns the name of the current architecture.
  virtual std::string GetName(void) = 0;

  //! This method converts an virtual address to a physical one.
  virtual bool        Translate(Address const& rVirtAddr, TAddress& rPhysAddr) = 0;

  //! This method disassembles one instruction.
  virtual bool        Disassemble(BinaryStream const& rBinStrm, TAddress Address, Instruction& rInsn) = 0;

  //! This method fills a configuration object.
  virtual void        FillConfigurationModel(ConfigurationModel& rCfgMdl) = 0;

  //! This method returns the architecture endianness.
  virtual EEndianness GetEndianness(void) = 0;

  //! This method allows to configure the current architecture.
  void                UseConfiguration(Configuration const& rCfg) { m_Cfg = rCfg; }

  //! This method converts an Instruction object to a string and stores the result on it.
  virtual void        FormatInstruction(Database const& rDatabase, Address const& rAddr, Instruction& rInsn);

protected:
  Configuration m_Cfg;
};

typedef Architecture* (*TGetArchitecture)(void);

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_ARCHITECTURE_
