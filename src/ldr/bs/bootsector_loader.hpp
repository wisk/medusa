#ifndef LDR_BOOTSECTOR_HPP
#define LDR_BOOTSECTOR_HPP

#include "medusa/namespace.hpp"
#include "medusa/document.hpp"
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

class BootSectorLoader : public Loader
{
public:
  virtual std::string GetName(void) const { return "boot sector"; }
  virtual u8          GetDepth(void) const { return 1; }

  virtual bool IsCompatible(BinaryStream const& rBinStrm) const;
  virtual std::vector<std::string> GetUsedArchitectures(BinaryStream const& rBinStrm) const { return { "x86/16-bit" }; }

  virtual bool Map(Document& rDoc) const;
  virtual bool Map(Document& rDoc, Address const& rImgBase) const { return false; }
  virtual void Analyze(Document& rDoc) const {}

private:
  enum { AddressOffset = 0x7c00 };
};

extern "C" LDR_BS_EXPORT Loader* GetLoader(void);

#endif // !LDR_BOOTSECTOR_HPP
