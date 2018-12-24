#ifndef LDR_PE_HPP
#define LDR_PE_HPP

#include <medusa/document.hpp>
#include <medusa/loader.hpp>
#include <medusa/log.hpp>

#include "pe.hpp"

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_pe_EXPORTS
#  define LDR_PE_EXPORT __declspec(dllexport)
#else
#  define LDR_PE_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_PE_EXPORT
#endif

MEDUSA_NAMESPACE_USE

class PeLoader : public Loader
{
public:
  // Generic methods
  virtual std::string GetName(void) const { return "PE"; }
  virtual u8          GetDepth(void) const { return 2; }

  // Only binary stream is needed
  virtual bool                     IsCompatible(BinaryStream const& rBinStrm) const;
  virtual std::string              GetDetailedName(BinaryStream const& rBinStrm) const;
  virtual std::string              GetSystemName(BinaryStream const& rBinStrm) const;
  virtual std::vector<std::string> GetUsedArchitectures(BinaryStream const& rBinStrm) const;

  // Document is required
  virtual bool Map(Document& rDoc) const;
  virtual bool Map(Document& rDoc, Address const& rImgBase) const;
  virtual void Analyze(Document& rDoc) const {}

private:
  static bool _GetInformation(BinaryStream const& rBinStrm, u16& rMachine, u16& rMagic, u64& rImageBase);

  template<int bit> bool _Map(Document& rDoc, u64 ImageBase) const;
  template<int bit> bool _MapSections(Document& rDoc, u64 ImageBase, u64 SectionHeadersOffset, u16 NumberOfSection, u32 SectionAlignment) const;
  template<int bit> bool _ResolveImports(Document& rDoc, u64 ImageBase, u64 ImportDirectoryRva, u64 ImportAddressTableRva) const;
  template<int bit> bool _ResolveExports(Document& rDoc, u64 ImageBase, u64 ExportDirectoryRva) const;
};

extern "C" LDR_PE_EXPORT Loader* GetLoader(void);

#endif // !LDR_PE_HPP
