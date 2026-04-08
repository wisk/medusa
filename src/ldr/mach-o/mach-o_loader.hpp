#ifndef LDR_MACH_O_HPP
#define LDR_MACH_O_HPP

#include <medusa/namespace.hpp>
#include <medusa/document.hpp>
#include <medusa/loader.hpp>
#include <medusa/log.hpp>
#include <medusa/module.hpp>

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

enum EntryPoBitVector {
    ENTRYPOINT_NONE,
    ENTRYPOINT_VMADDR,
    ENTRYPOINT_OFFSET,
};

class MachOLoader : public Loader
{
public:
  std::string GetName(void) const override;
  u8          GetDepth(void) const override;
  bool        IsCompatible(BinaryStream const& rBinStrm) const override;
  std::vector<std::string> GetUsedArchitectures(BinaryStream const& rBinStrm) const override;
  bool        Map(Document& rDoc) const override;
  bool        Map(Document& rDoc, Address const& rImgBase) const override;
  void        Analyze(Document& rDoc) const override {}

private:
    template<int bit> bool MapImpl(Document& rDoc) const;
    template<int bit> void MapSegment(Document& rDoc, int LoadCmdOff, Tag ArchTag, u8 ArchMode,
                                      EEndianness Endian, u32 HeaderOffset,
                                      u64& rTextSectionVMAddr, u64& rImportsAddress, u32& rSymbolsIndex) const;
                      void GetEntryPointV1(Document& rDoc, int LoadCmdOff, u32 Machine, EEndianness Endian,
                                           u64& rEntryPoint, EntryPoBitVector& rEntryPoBitVector) const;
                      void GetEntryPointV2(Document& rDoc, int LoadCmdOff, EEndianness Endian,
                                           u64& rEntryPoint, EntryPoBitVector& rEntryPoBitVector) const;
    template<int bit> void GetSymbols(Document& rDoc, int LoadCmdOff, EEndianness Endian,
                                      u32& rSymbolsOffset, u32& rStringsOffset) const;
    template<int bit> void GetDynamicSymbols(Document& rDoc, int LoadCmdOff, EEndianness Endian,
                                             u32 SymbolsOffset, u32 StringsOffset,
                                             u64 ImportsAddress, u32 SymbolsIndex) const;

    static bool _GetMachineAndEndian(BinaryStream const& rBinStrm, u32& rMachine, bool& rArch64, EEndianness& rEndian, u32& rHeaderOffset);

    static std::string _GetArchitectureName(u32 Machine, bool Arch64);
};

extern "C" LDR_MACH_O_EXPORT Loader* GetLoader(void);

#endif // LDR_MACH_O_HPP
