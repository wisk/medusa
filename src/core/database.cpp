#include "medusa/database.hpp"
#include "medusa/log.hpp"

MEDUSA_NAMESPACE_BEGIN

Database::Database(void)
{
}

Database::~Database(void)
{
}

std::string Database::GetName(void) const
{
  return "Invalid";
}

std::string Database::GetExtension(void) const
{
  return ".mdb"; // generic name
}

bool Database::IsCompatible(boost::filesystem::path const& rDatabasePath) const
{
  return false;
}

bool Database::Open(boost::filesystem::path const& rDatabasePath)
{
  return false;
}

bool Database::Create(boost::filesystem::path const& rDatabasePath, bool Force)
{
  return false;
}

bool Database::Flush(void)
{
  return false;
}

bool Database::Close(void)
{
  return false;
}

Database& Database::SetBinaryStream(BinaryStream::SPType spBinStrm)
{
  m_spBinStrm = spBinStrm;
  return *this;
}

BinaryStream& Database::GetBinaryStream(void)
{
  // TODO(wisk): don't need a shared ptr here
  return *m_spBinStrm;
}

Database& Database::SetOperatingSystemName(std::string const& rOperatingSystemName)
{
  m_OsName = rOperatingSystemName;
  return *this;
}

std::string Database::GetOperatingSystemName(void) const
{
  return m_OsName;
}

MEDUSA_NAMESPACE_END
