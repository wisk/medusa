#ifndef _ELF_LOADER_
#define _ELF_LOADER_

#include "medusa/namespace.hpp"
#include "medusa/database.hpp"
#include "medusa/loader.hpp"
#include "elf.h"
#include "elf_traits.hpp"
#include "elf_interpreter.hpp"

MEDUSA_NAMESPACE_USE

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_elf_EXPORTS
#  define LDR_ELF_EXPORT __declspec(dllexport)
#else
#  define LDR_ELF_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_ELF_EXPORT
#endif

class                             ElfLoader : public Loader
{
public:
                                  ElfLoader(Database& rDatabase);
  virtual                         ~ElfLoader(void) {}

  virtual std::string             GetName(void) const;
  virtual bool                    IsSupported(void) { return m_IsValid; }
  virtual void                    Map(void);
  virtual void                    Translate(Address const& rVirtAddr, TOffset& rRawOff);
  virtual Address                 GetEntryPoint(void);
  virtual Architecture::SharedPtr GetMainArchitecture(Architecture::VectorSharedPtr const& rArchitectures);
  virtual void                    Configure(Configuration& rCfg);

private:
  EEndianness                     GetEndianness(void);
  u8                              GetWordSize(void) const
  {
    switch (m_Ident[EI_CLASS])
    {
      case ELFCLASS32: return 32;
      case ELFCLASS64: return 64;
      default:         return  0;
    }
  }

  Database&                    m_rDatabase;
  bool                         m_IsValid;
  u8                           m_Ident[EI_NIDENT];
  u8                           m_Machine;

  union
  {
    ElfInterpreter<32>* _32;
    ElfInterpreter<64>* _64;
  }                            m_Elf;
};

extern "C" LDR_ELF_EXPORT Loader* GetLoader(Database& rDatabase);

#endif // _ELF_LOADER_