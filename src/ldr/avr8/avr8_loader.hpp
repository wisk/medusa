#ifndef _AVR8_LOADER_
#define _AVR8_LOADER_ 

#include <medusa/document.hpp>
#include <medusa/loader.hpp>
#include <medusa/log.hpp>

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_avr8_EXPORTS
#  define LDR_AVR8_EXPORT __declspec(dllexport)
#else
#  define LDR_AVR8_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_AVR8_EXPORT
#endif

MEDUSA_NAMESPACE_USE

class Avr8Loader : public Loader
{
public:
  Avr8Loader(void);
  virtual ~Avr8Loader(void) {}
  virtual std::string GetName(void) const;
  virtual u8          GetDepth(void) const { return 0; }
  virtual bool        IsCompatible(BinaryStream const& rBinStrm);
  virtual void        Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs);
  virtual void        FilterAndConfigureArchitectures(Architecture::VectorSharedPtr& rArchs) const;
};

extern "C" LDR_AVR8_EXPORT Loader* GetLoader(void);

#endif
