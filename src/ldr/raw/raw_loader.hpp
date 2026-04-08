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
  std::string GetName(void) const override { return "raw"; }
  u8          GetDepth(void) const override { return 0; }

  bool                     IsCompatible(BinaryStream const& rBinStrm) const override { return true; }
  std::string              GetDetailedName(BinaryStream const& rBinStrm) const override { return GetName(); }
  std::string              GetSystemName(BinaryStream const& rBinStrm) const override { return ""; }
  std::vector<std::string> GetUsedArchitectures(BinaryStream const& rBinStrm) const override { return { "*" }; }

  bool Map(Document& rDoc) const override;
  bool Map(Document& rDoc, Address const& rImgBase) const override { return false; }
  void Analyze(Document& rDoc) const override {}
};

extern "C" LDR_RAW_EXPORT Loader* GetLoader(void);

#endif // !LDR_RAW_HPP
