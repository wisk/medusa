#ifndef LDR_ST62_HPP
#define LDR_ST62_HPP

#include <medusa/document.hpp>
#include <medusa/loader.hpp>

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_st62_EXPORTS
#  define LDR_ST62_EXPORT __declspec(dllexport)
#else
#  define LDR_ST62_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_ST62_EXPORT
#endif

MEDUSA_NAMESPACE_USE

class ST62Loader : public Loader
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
};

extern "C" LDR_ST62_EXPORT Loader* GetLoader(void);

#endif // LDR_ST62_HPP
