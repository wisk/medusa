#ifndef LDR_MACH_O_HPP
#define LDR_MACH_O_HPP

#include <medusa/namespace.hpp>
#include <medusa/document.hpp>
#include <medusa/loader.hpp>
#include <medusa/log.hpp>

#include <list>

MEDUSA_NAMESPACE_USE

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_mach_o_EXPORTS
#  define LDR_MACH_O_EXPORT __declspec(dllexport)
#else
#  define LDR_MACH_O_EXPORT __declspec(dllimport)
#endif
#else
#  define LDR_MACH_O_EXPORT
#endif

typedef enum {
    ENTRYPOINT_NONE,
    ENTRYPOINT_VMADDR,
    ENTRYPOINT_OFFSET,
} EntryPointType;

class MachOLoader : public Loader
{
public:
                           MachOLoader(void);
  virtual std::string      GetName(void) const;
  virtual u8               GetDepth(void) const;
  virtual bool             IsCompatible(BinaryStream const& rBinStrm);
  virtual void             Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs);
  virtual void             FilterAndConfigureArchitectures(Architecture::VectorSharedPtr& rArchs) const;

private:
    template<int bit> void Map(Document& rDoc, Architecture::VectorSharedPtr const& rArchs);
    template<int bit> void MapSegment(Document& rDoc, int LoadCmdOff, Tag ArchTag, u8 ArchMode);
                      void GetEntryPointV1(Document& rDoc, int LoadCmdOff);
                      void GetEntryPointV2(Document& rDoc, int LoadCmdOff);
    template<int bit> void GetSymbols(Document& rDoc, int LoadCmdOff);
    template<int bit> void GetDynamicSymbols(Document& rDoc, int LoadCmdOff);

    bool                   _FindArchitectureTagAndModeByMachine(
        Architecture::VectorSharedPtr const& rArchs,
        Tag& rArchTag, u8& rArchMode) const;

    u32            m_Machine;
    bool           m_Arch64;
    EEndianness    m_Endian;
    u64            m_EntryPoint;
    EntryPointType m_EntryPointType;
    u64            m_TextSectionVMAddr;
    u32            m_SymbolsOffset;
    u32            m_StringsOffset;
    u64            m_ImportsAddress;
    u32            m_SymbolsIndex;
};

extern "C" LDR_MACH_O_EXPORT Loader* GetLoader(void);

#endif // LDR_MACH_O_HPP