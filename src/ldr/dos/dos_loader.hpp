#ifndef LDR_DOS_HPP
#define LDR_DOS_HPP

#include "medusa/namespace.hpp"
#include "medusa/document.hpp"
#include "medusa/loader.hpp"
#include "dos.hpp"

MEDUSA_NAMESPACE_USE

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_dos_EXPORTS
#  define LDR_DOS_EXPORT __declspec(dllexport)
#else
#  define LDR_DOS_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_DOS_EXPORT
#endif

class DosLoader : public Loader
{
public:
  virtual std::string GetName(void) const;
  virtual u8          GetDepth(void) const { return 0; }
  virtual bool        IsCompatible(BinaryStream const& rBinStrm);
  virtual void        Map(Document& rDoc, Architecture::VSPType const& rArchs);
  virtual void        FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const;

private:
  
};

extern "C" LDR_DOS_EXPORT Loader* GetLoader(void);

#endif // !LDR_DOS_HPP
