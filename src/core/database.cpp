#include "medusa/database.hpp"
#include "medusa/log.hpp"

MEDUSA_NAMESPACE_BEGIN

Database::Database()
{
}

Database::~Database()
{
}

std::string Database::GetName() const
{
  return "Invalid";
}

std::string Database::GetExtension() const
{
  return ".mdb"; // generic name
}

bool Database::IsCompatible(Path const& rDatabasePath) const
{
  return false;
}

bool Database::Open(Path const& rDatabasePath)
{
  return false;
}

bool Database::Create(Path const& rDatabasePath, bool Force)
{
  return false;
}

bool Database::Flush()
{
  return false;
}

bool Database::Close()
{
  return false;
}

Database& Database::SetBinaryStream(BinaryStream::SPType spBinStrm)
{
  m_spBinStrm = spBinStrm;
  return *this;
}

BinaryStream& Database::GetBinaryStream()
{
  // TODO(wisk): don't need a shared ptr here
  return *m_spBinStrm;
}

Database& Database::SetOperatingSystemName(std::string const& rOperatingSystemName)
{
  m_OsName = rOperatingSystemName;
  return *this;
}

std::string Database::GetOperatingSystemName() const
{
  return m_OsName;
}

MEDUSA_NAMESPACE_END
