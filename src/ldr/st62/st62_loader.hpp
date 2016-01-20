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
  virtual bool        IsCompatible(BinaryStream const& rBinStrm);
  virtual bool        Map(Document& rDoc, Architecture::VSPType const& rArchs);
  virtual bool        Map(Document& rDoc, Architecture::VSPType const& rArchs, Address const& rImgBase);
  virtual void        FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const;

private:
};

extern "C" LDR_ST62_EXPORT Loader* GetLoader(void);

#endif // LDR_ST62_HPP
