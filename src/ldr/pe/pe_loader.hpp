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
  PeLoader(void);

  virtual std::string GetName(void) const;
  virtual u8          GetDepth(void) const { return 2; /* IMAGE_DOS_HEADER is depth 1 */ }
  virtual bool        IsCompatible(BinaryStream const& rBinStrm);
  virtual void        Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs);
  virtual void        FilterAndConfigureArchitectures(Architecture::VectorSharedPtr& rArchs) const;

private:
  u16 m_Machine;
  u16 m_Magic;

  bool _FindArchitectureTagAndModeByMachine(
      Architecture::VectorSharedPtr const& rArchs,
      Tag& rArchTag, u8& rArchMode
      ) const;

  template<int bit> void _Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs);
  template<int bit> void _MapSections(Document& rDoc, Architecture::VectorSharedPtr const& rArchs, u64 ImageBase, u64 SectionHeadersOffset, u16 NumberOfSection);
  template<int bit> void _ResolveImports(Document& rDoc, u64 ImageBase, u64 ImportDirectoryRva, u64 ImportAddressTableRva);
  template<int bit> void _ResolveExports(Document& rDoc, u64 ImageBase, u64 ExportDirectoryRva);
};

extern "C" LDR_PE_EXPORT Loader* GetLoader(void);

#endif // !LDR_PE_HPP
