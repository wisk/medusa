#ifndef LDR_GAMEBOY_HPP
#define LDR_GAMEBOY_HPP

#include <medusa/document.hpp>
#include <medusa/loader.hpp>
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

class GameBoyLoader : public Loader
{
public:
  virtual std::string GetName(void) const;
  virtual u8          GetDepth(void) const { return 1; }
  virtual bool        IsCompatible(BinaryStream const& rBinStrm);
  virtual bool        Map(Document& rDoc, Architecture::VSPType const& rArchs);
  virtual bool        Map(Document& rDoc, Architecture::VSPType const& rArchs, Address const& rImgBase);
  virtual void        FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const;

private:
  typedef u16 BankType;
  BankType GetNumberOfBank(void) const;
  enum     { BankSize = 0x4000 };

  GameBoyRom m_GameBoyRom;
};

extern "C" LDR_GB_EXPORT Loader* GetLoader(void);

#endif // LDR_GAMEBOY_HPP
