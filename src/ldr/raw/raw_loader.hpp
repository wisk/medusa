#ifndef _RAW_LOADER_
#define _RAW_LOADER_

#include <medusa/database.hpp>
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
  RawLoader(Database& rDatabase) : Loader(rDatabase), m_rDatabase(rDatabase) {}
  virtual ~RawLoader(void) {}

  virtual char  const*    GetName(void)
  { return "Raw file";  }

  virtual bool            IsSupported(void)
  { return true;        }

  virtual void            Map(void)
  {
    m_rDatabase.AddMemoryArea(new MappedMemoryArea(
      m_rDatabase.GetFileBinaryStream(), "file",
      Address(Address::PhysicalType, 0x0), m_rDatabase.GetFileBinaryStream().GetSize(),
      Address(Address::FlatType, 0x0),     m_rDatabase.GetFileBinaryStream().GetSize(),
      MA_EXEC | MA_READ | MA_WRITE
    ));
  }

  virtual void            Translate(Address const& rVirtlAddr, TOffset& rOffset)
  { rOffset = rVirtlAddr.GetOffset(); }

  virtual Address GetEntryPoint(void)
  { return Address(0x0); }

  virtual Architecture::SharedPtr GetMainArchitecture(Architecture::VectorSharedPtr const& rArchitectures)
  { return Architecture::SharedPtr(); }

private:
  Database&  m_rDatabase;
};

extern "C" LDR_RAW_EXPORT Loader* GetLoader(Database& rDatabase);

#endif // _RAW_LOADER_