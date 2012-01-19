#ifndef _GAMEBOY_LOADER_
#define _GAMEBOY_LOADER_

#include "medusa/database.hpp"
#include "medusa/loader.hpp"
#include "gameboy.h"

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_gb_EXPORTS
#  define LDR_GB_EXPORT __declspec(dllexport)
#else
#  define LDR_GB_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_GB_EXPORT
#endif

MEDUSA_NAMESPACE_USE

class                       GameBoyLoader : public Loader
{
public:
                            GameBoyLoader(Database& rDatabase);
  virtual                  ~GameBoyLoader(void)                             {}

  virtual char  const*      GetName(void)                                     { return "GameBoy (color)"; }
  virtual bool              IsSupported(void)                                 { return m_IsValid; }
  virtual void              Map(void);
  virtual void              Translate(Address const& rVirtAddr, TOffset& rOffset) { }
  virtual Address           GetEntryPoint(void);
  virtual EEndianness       GetEndianness(void)                               { return LittleEndian; }
  virtual Architecture::Ptr GetMainArchitecture(Architecture::VectorPtr const& Architectures);
  virtual Address::SPtr     MakeAddress(TBase Base, TOffset Offset)
  {
    Address::SPtr NewAddr(new Address(Address::BankType, Base, Offset, 16, 16));
    return NewAddr;
  }

private:
  TBank                        GetNumberOfBank(void) const;
  enum                         { BankSize = 0x4000 };

  Database&                    m_rDatabase;
  SGameBoyRom                  m_GameBoyRom;
  bool                         m_IsValid;
};

extern "C" LDR_GB_EXPORT Loader* GetLoader(Database& rDatabase);

#endif // _GAMEBOY_LOADER_
