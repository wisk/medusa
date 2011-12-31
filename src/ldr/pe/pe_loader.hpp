#ifndef _PE_LOADER_
#define _PE_LOADER_

#include <medusa/database.hpp>
#include <medusa/loader.hpp>

#include "pe_interpreter.hpp"
#include "pe.hpp"

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_pe_EXPORTS
#  define LDR_PE_EXPORT __declspec(dllexport)
#else
#  define LDR_PE_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_PE_EXPORT
#endif

MEDUSA_NAMESPACE_USE

class                       PeLoader : public Loader
{
public:
                            PeLoader(Database& rDatabase);
  virtual                  ~PeLoader(void) {}

  virtual char  const*      GetName(void);
  virtual bool              IsSupported(void) { return m_IsValid; }
  virtual void              Map(void);
  virtual void              Translate(Address const& rVirtAddr, TOffset& rOffset);
  virtual Address           GetEntryPoint(void);
  virtual Architecture::Ptr GetMainArchitecture(Architecture::VectorPtr const& Architectures)
  { return Architecture::Ptr(); }

private:
  u8 GetWordSize(void)      { return m_WordSize; }

  Database&                 m_rDatabase;
  bool                      m_IsValid;
  u16                       m_Machine;
  u8                        m_WordSize;

  union
  {
    PeInterpreter<u32>* _32;
    PeInterpreter<u64>* _64;
  }                         m_Pe;
};

extern "C" LDR_PE_EXPORT Loader* GetLoader(Database& rDatabase);

#endif
