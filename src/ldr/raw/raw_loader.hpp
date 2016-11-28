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
  virtual ~RawLoader(void) {}

  virtual std::string GetName(void) const;
  virtual u8          GetDepth(void) const { return 0; }
  virtual bool        IsCompatible(BinaryStream const& rBinStrm);
  virtual bool        Map(Document& rDoc, Architecture::VSPType const& rArchs);
  virtual bool        Map(Document& rDoc, Architecture::VSPType const& rArchs, Address const& rImgBase);
  virtual void        FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const;
};

extern "C" LDR_RAW_EXPORT Loader* GetLoader(void);

#endif // !LDR_RAW_HPP
