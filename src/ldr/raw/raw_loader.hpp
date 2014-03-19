#ifndef _RAW_LOADER_
#define _RAW_LOADER_

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
  virtual void        Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs);
  virtual void        FilterAndConfigureArchitectures(Architecture::VectorSharedPtr& rArchs) const;
};

extern "C" LDR_RAW_EXPORT Loader* GetLoader(void);

#endif // _RAW_LOADER_
