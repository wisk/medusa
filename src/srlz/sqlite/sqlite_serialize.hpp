#include <medusa/serialize.hpp>
#include <boost/variant.hpp>
#include "sqlite3.h"

#if defined(_WIN32) || defined(WIN32)
#ifdef srlz_sqlite_EXPORTS
#  define SRLZ_SQLITE_EXPORT __declspec(dllexport)
#else
#  define SRLZ_SQLITE_EXPORT __declspec(dllimport)
#endif
#else
#define SRLZ_SQLITE_EXPORT
#endif

class SqliteSerialize : public medusa::Serialize
{
public:

  SqliteSerialize(void) : Serialize() {}
  virtual ~SqliteSerialize(void) {}

  virtual void Open(std::string const& rFilename);
  virtual void Load(void);
  virtual void Save(void);

  void Visitor(medusa::SerializeEntity::SPtr spSrlzEtt, sqlite3* rSqlite, sqlite3_int64 iOwner);
  void InsertIntoDB();

  private:
  class SqliteVisitor : public boost::static_visitor<>
  {
  public:
    explicit SqliteVisitor(sqlite3& rSqlite3)
      : m_rOstrm(rSqlite3) {}

  public:
    void operator()(medusa::Address const& rAddr) const
    {
      //m_rOstrm << rAddr.ToString();
    }

    void operator()(medusa::BinaryStream const& rBinStrm) const
    {
      //m_rOstrm
      //  << "bs(" << std::hex << std::setw(8) << std::setfill('0') << rBinStrm.GetSize()
     //   << ", \"";

      //for (medusa::u32 i = 0; i < rBinStrm.GetSize(); ++i)
      //{
      //  medusa::u8 Byte;
      //  rBinStrm.Read(i, Byte);
      //  m_rOstrm
      //    << std::hex << std::setw(2) << std::setfill('0') << Byte;
      //}

     // m_rOstrm << "\")";
    }

    void operator()(medusa::s8 Data) const
    {
      //m_rOstrm << static_cast<int>(Data);
    }

    void operator()(medusa::u8 Data) const
    {
     // m_rOstrm << static_cast<unsigned int>(Data);
    }

    template <typename T>
    void operator()(const T& rEtt) const
    {
      //m_rOstrm << rEtt;
    }

  private:
    SqliteVisitor& operator=(const SqliteVisitor&);
    sqlite3& m_rOstrm;
  };

private:
  sqlite3               *m_pSqlite;
  std::string           m_sDbName;
  sqlite3_int64         m_iId;
};

extern "C" SRLZ_SQLITE_EXPORT medusa::Serialize* GetSerialize(void);
