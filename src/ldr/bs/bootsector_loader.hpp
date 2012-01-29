#ifndef BOOTSECTOR_LOADER
#define BOOTSECTOR_LOADER

#include "medusa/namespace.hpp"
#include "medusa/database.hpp"
#include "medusa/loader.hpp"

MEDUSA_NAMESPACE_USE

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_bs_EXPORTS
#  define LDR_BS_EXPORT __declspec(dllexport)
#else
#  define LDR_BS_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_BS_EXPORT
#endif

class                       BootSectorLoader : public Loader
{
public:
                            BootSectorLoader(Database& rDatabase);
  virtual                  ~BootSectorLoader(void)                           {}

  virtual char  const*      GetName(void)                                     { return "Boot sector"; }
  virtual bool              IsSupported(void)                                 { return m_IsValid; }
  virtual void              Map(void);
  virtual void              Translate(Address const& rVirtAddr, TOffset& rOffset) { }
  virtual Address           GetEntryPoint(void)                               { return Address(0x0, AddressOffset); }
  virtual Architecture::SPtr GetMainArchitecture(Architecture::VectorSPtr& rArchitectures);
  virtual void              Configure(Configuration& rCfg);

private:
  static TOffset            AddressOffset;
  Database&                 m_rDatabase;
  bool                      m_IsValid;
};

extern "C" LDR_BS_EXPORT Loader* GetLoader(Database& rDatabase);

#endif // !BOOTSECTOR_LOADER