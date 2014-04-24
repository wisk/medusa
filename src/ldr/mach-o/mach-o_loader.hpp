#ifndef _MACH_O_LOADER_
#define _MACH_O_LOADER_

#include <medusa/namespace.hpp>
#include <medusa/document.hpp>
#include <medusa/loader.hpp>
#include <medusa/log.hpp>

#include <boost/foreach.hpp> // TODO: Use c++11 foreach instead of BOOST_FOREACH

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
  virtual std::string             GetName(void) const;
  virtual bool                    IsCompatible(BinaryStream const& rBinStrm);
  virtual void                    Map(Document& rDoc);
  virtual Architecture::SharedPtr GetMainArchitecture(Architecture::VectorSharedPtr const& rArchitectures);
  virtual void                    Configure(Configuration& rCfg);

private:
    template<int bit> void Map(Document& rDoc);
    template<int bit> void MapSegment(Document& rDoc, int LoadCmdOff);
                      void GetEntryPointV1(Document& rDoc, int LoadCmdOff);
                      void GetEntryPointV2(Document& rDoc, int LoadCmdOff);
    template<int bit> void GetSymbols(Document& rDoc, int LoadCmdOff);

    u32            m_Machine;
    bool           m_Arch64;
    EEndianness    m_Endian;
    u64            m_EntryPoint;
    EntryPointType m_EntryPointType;
    u64            m_TextSectionVMAddr;
};

extern "C" LDR_MACH_O_EXPORT Loader* GetLoader(void);

#endif // _MACH_O_LOADER_
