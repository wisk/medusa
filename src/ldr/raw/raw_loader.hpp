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

  virtual std::string GetName(void) const { return "Raw file";  }
  virtual bool        IsCompatible(BinaryStream const&) { return true; }
  virtual void        Map(Document& rDoc)
  {
    rDoc.AddMemoryArea(new MappedMemoryArea(
      "file",
      0x0, rDoc.GetFileBinaryStream().GetSize(),
      Address(Address::FlatType, 0x0), rDoc.GetFileBinaryStream().GetSize(),
      MemoryArea::Execute | MemoryArea::Read | MemoryArea::Write
    ));
  }

  virtual Architecture::SharedPtr GetMainArchitecture(Architecture::VectorSharedPtr const& rArchitectures)
  { return Architecture::SharedPtr(); }

};

extern "C" LDR_RAW_EXPORT Loader* GetLoader(void);

#endif // _RAW_LOADER_