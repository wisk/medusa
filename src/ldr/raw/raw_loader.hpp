#ifndef LDR_RAW_HPP
#define LDR_RAW_HPP

#include <medusa/document.hpp>
#include <medusa/loader.hpp>

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_raw_EXPORTS
#  define LDR_RAW_EXPORT __declspec(dllexport)
#else
#  define LDR_RAW_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_RAW_EXPORT
#endif

MEDUSA_NAMESPACE_USE

class RawLoader : public Loader
{
public:
  virtual std::string GetName(void) const { return "raw"; }
  virtual u8          GetDepth(void) const { return 0; }

  virtual bool                     IsCompatible(BinaryStream const& rBinStrm) const { return true; }
  virtual std::string              GetDetailedName(BinaryStream const& rBinStrm) const { return GetName(); }
  virtual std::string              GetSystemName(BinaryStream const& rBinStrm) const { return ""; }
  virtual std::vector<std::string> GetUsedArchitectures(BinaryStream const& rBinStrm) const { return { "*" }; }

  virtual bool Map(Document& rDoc) const;
  virtual bool Map(Document& rDoc, Address const& rImgBase) const { return false; }
  virtual void Analyze(Document& rDoc) const {}
};

extern "C" LDR_RAW_EXPORT Loader* GetLoader(void);

#endif // !LDR_RAW_HPP
