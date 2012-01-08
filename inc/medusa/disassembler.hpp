#ifndef _MEDUSA_DISASSEMBLER_
#define _MEDUSA_DISASSEMBLER_

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/database.hpp"
#include "medusa/architecture.hpp"
#include "medusa/serialize.hpp"

MEDUSA_NAMESPACE_BEGIN

//! Disassembler handles all analysis operations.
class Medusa_EXPORT Disassembler : public SerializeAccess
{
public:
  Disassembler(void)
  : m_FunctionPrefix("func_")
  , m_LabelPrefix("lbl_")
  , m_DataPrefix("dat_")
  , m_StringPrefix("str_")
  {}

  ~Disassembler(void) {}

  virtual void                  Load(SerializeEntity::SPtr spSrlzEtt);
  virtual SerializeEntity::SPtr Save(void);

  //! This method disassembles code by following the execution path.
  void FollowExecutionPath(Database& rDatabase, Address const& rEntrypoint, Architecture &rArch) const;

  //! This method finds and adds cross-references.
  void CreateXRefs(Database& rDatabase) const;

  //! This method updates string in an instruction.
  void FormatInstructions(Database& rDatabase, Architecture &rArch) const;

  //! This method finds string using specific patterns.
  void FindStrings(Database& rDatabase) const;

  /*! This method computes the size of a function.
   * \param rDatabase contains all cells.
   * \param rFunctionAddress is the address of the function.
   * \param EndAddress is set by this method and contains the end of the function.
   * \param rFunctionLength is set by this method and contains the size of the function.
   * \param rInstructionCounter is set by this method and contains the number of instruction in the function.
   * \param LengthThreshold is the maximum size of this function, it this value is reached, this method returns false.
   * \return Returns true if the size of the function can be computed, otherwise it returns false.
   */
  bool ComputeFunctionLength(
    Database const& rDatabase,
    Address const& rFunctionAddress,
    Address& EndAddress,
    u16& rFunctionLength,
    u16& rInstructionCounter,
    u32 LengthThreshold) const;

private:
  std::string m_FunctionPrefix; //! Function prefix
  std::string m_LabelPrefix;    //! Label prefix
  std::string m_DataPrefix;     //! Data prefix
  std::string m_StringPrefix;   //! String prefix
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_DISASSEMBLER_