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

bool Database::IsCompatible(std::wstring const& rDatabasePath) const
{
  return false;
}

bool Database::Open(std::wstring const& rDatabasePath)
{
  return false;
}

bool Database::Create(std::wstring const& rDatabasePath, bool Force)
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

Database& Database::SetBinaryStream(BinaryStream::SharedPtr spBinStrm)
{
  m_spBinStrm = spBinStrm;
  return *this;
}

BinaryStream::SharedPtr const Database::GetBinaryStream(void) const
{
  return m_spBinStrm;
}

Database& Database::SetConfigurationManager(ConfigurationManager const& rConfigurationManager)
{
  m_CfgMgr = rConfigurationManager;
  return *this;
}

Configuration* Database::GetConfiguration(ConfigurationManager::ConfigurationType Type)
{
  return m_CfgMgr.GetConfiguration(Type);
}

Configuration const* GetConfiguration(ConfigurationManager::ConfigurationType Type) const
{
  return m_CfgMgr.GetConfiguration(Type);
}

MEDUSA_NAMESPACE_END
