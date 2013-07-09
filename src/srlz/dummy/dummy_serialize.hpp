#include <medusa/serialize.hpp>
#include <boost/variant.hpp>
#include <fstream>

#if defined(_WIN32) || defined(WIN32)
#ifdef srlz_dummy_EXPORTS
#  define SRLZ_DUMMY_EXPORT __declspec(dllexport)
#else
#  define SRLZ_DUMMY_EXPORT __declspec(dllimport)
#endif
#else
#define SRLZ_DUMMY_EXPORT
#endif

class DummySerialize : public medusa::Serialize
{
public:
  DummySerialize(void) : Serialize() {}
  virtual ~DummySerialize(void) {}

  virtual void Open(std::string const& rFilename);
  virtual void Load(void);
  virtual void Save(void);

  void Visitor(medusa::SerializeEntity::SPtr spSrlzEtt, std::ofstream& rOfs);

private:
  class OStreamVisitor : public boost::static_visitor<>
  {
  public:
    explicit OStreamVisitor(std::ostream& rOstrm)
      : m_rOstrm(rOstrm) {}

  public:
    void operator()(medusa::Address const& rAddr) const
    {
      m_rOstrm << rAddr.ToString();
    }

    void operator()(medusa::BinaryStream const& rBinStrm) const
    {
      m_rOstrm
        << "bs(" << std::hex << std::setw(8) << std::setfill('0') << rBinStrm.GetSize()
        << ", \"";

      medusa::u8* pBuffer = new medusa::u8[rBinStrm.GetSize() * 2];
      medusa::u8* pData   = new medusa::u8[rBinStrm.GetSize()];

      rBinStrm.Read(0x0, pData, rBinStrm.GetSize());

      for (medusa::u32 i = 0; i < rBinStrm.GetSize(); ++i)
      {
        static const char *pHexCharset = "0123456789abcedf";
        pBuffer[i + 0] = pHexCharset[pData[i] >> 4 ];
        pBuffer[i + 1] = pHexCharset[pData[i] & 0xf];
      }
      m_rOstrm.write((char*)pBuffer, rBinStrm.GetSize() * 2);
      m_rOstrm << "\")";
    }

    void operator()(medusa::s8 Data) const
    {
      m_rOstrm << static_cast<int>(Data);
    }

    void operator()(medusa::u8 Data) const
    {
      m_rOstrm << static_cast<unsigned int>(Data);
    }

    template <typename T>
    void operator()(const T& rEtt) const
    {
      m_rOstrm << rEtt;
    }

  private:
    OStreamVisitor& operator=(const OStreamVisitor&);
    std::ostream& m_rOstrm;
  };

  std::ofstream m_Ofstrm;
};

extern "C" SRLZ_DUMMY_EXPORT medusa::Serialize* GetSerialize(void);
