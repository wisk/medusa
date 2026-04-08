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
  virtual bool        IsCompatible(BinaryStream const& rBinStrm) const;
  virtual std::string              GetDetailedName(BinaryStream const& rBinStrm) const { return GetName(); }
  virtual std::string              GetSystemName(BinaryStream const& rBinStrm) const { return ""; }
  virtual std::vector<std::string> GetUsedArchitectures(BinaryStream const& rBinStrm) const;
  virtual bool        Map(Document& rDoc) const;
  virtual bool        Map(Document& rDoc, Address const& rImgBase) const;
  virtual void        Analyze(Document& rDoc) const;

private:
  using LogicalType = u16;
  BaseType GetNumberOfBank(BinaryStream const& rBinStrm) const;
  enum     { BankSize = 0x4000 };
  static bool ReadHeader(BinaryStream const& rBinStrm, GameBoyRom& rRom);
};

extern "C" LDR_GB_EXPORT Loader* GetLoader(void);

#endif // LDR_GAMEBOY_HPP
