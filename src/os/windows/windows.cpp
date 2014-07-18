#include "windows.hpp"
#include "stack_analyzer.hpp"
#include "medusa/module.hpp"

#include "sqlite3.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

WindowsOperatingSystem::WindowsOperatingSystem(void)
  : m_Database(nullptr)
{
}

WindowsOperatingSystem::~WindowsOperatingSystem(void)
{
  if (m_Database != nullptr)
    sqlite3_close(m_Database);
}

std::string WindowsOperatingSystem::GetName(void) const
{
  return "MS Windows";
}

bool WindowsOperatingSystem::InitializeCpuContext(Document const& rDoc, CpuContext& rCpuCtxt) const
{
  CpuInformation const& rCpuInfo = rCpuCtxt.GetCpuInformation();
  auto IdFs = rCpuInfo.ConvertNameToIdentifier("fs");
  if (IdFs == 0)
    return false;
  u16 Fs = 0x2b;
  if (rCpuCtxt.WriteRegister(IdFs, &Fs, sizeof(Fs)) == false)
    return false;
  if (rCpuCtxt.AddMapping(Address(Fs, 0x0), 0x7fdf0000) == false)
    return false;

  auto StartAddr = rDoc.GetAddressFromLabelName("start");
  u64 StartAddrVal = StartAddr.GetOffset();
  auto IdD = rCpuInfo.ConvertNameToIdentifier("rdx"); // it doesn't matter to use rdx instead of ecx or cx
  if (rCpuCtxt.WriteRegister(IdD, &StartAddrVal, sizeof(StartAddrVal)) == false)
    return false;

  return true;
}

bool WindowsOperatingSystem::InitializeMemoryContext(Document const& rDoc, MemoryContext& rMemCtxt) const
{
  // TODO: create a fake _TEB/_PEB
  if (rMemCtxt.AllocateMemory(0x7fdf0000, 0x1000, nullptr) == false)
    return false;
  return true;
}

bool WindowsOperatingSystem::IsSupported(Loader const& rLdr, Architecture const& rArch) const
{
  // At this time, only Windows on x86 is supported
  if ((rLdr.GetName() == "PE" || rLdr.GetName() == "PE+") && rArch.GetName() == "Intel x86")
    return true;

  return false;
}

bool WindowsOperatingSystem::AnalyzeFunction(Document& rDoc, Address const& rAddress)
{
  return true;
}

bool WindowsOperatingSystem::ProvideDetails(Document& rDoc) const
{
  return true;
}

bool WindowsOperatingSystem::GetValueDetail(Id ValueId, ValueDetail& rValDtl) const
{
  return false;
}

bool WindowsOperatingSystem::GetFunctionDetail(Id FunctionId, FunctionDetail& rFcnDtl) const
{
  if (!_OpenDatabaseIfNeeded())
    return false;

  static char const* pQuery = "SELECT * FROM TABLES Function WHERE id = ?";
  sqlite3_stmt *pStmt = nullptr;
  if (sqlite3_prepare(m_Database, pQuery, ::strlen(pQuery), &pStmt, nullptr) != SQLITE_OK)
  {
    Log::Write("os_windows") << "sqlite3_prepare: " << sqlite3_errmsg(m_Database) << LogEnd;
    return false;
  }

  std::string const IdStr = boost::lexical_cast<std::string>(FunctionId);

  if (sqlite3_bind_text(pStmt, 1, IdStr.c_str(), IdStr.length(), nullptr) != SQLITE_OK)
  {
    Log::Write("os_windows") << "sqlite3_bind_text: " << sqlite3_errmsg(m_Database) << LogEnd;
    return false;
  }

  if (sqlite3_step(pStmt) != SQLITE_OK)
  {
    Log::Write("os_windows") << "sqlite3_step: " << sqlite3_errmsg(m_Database) << LogEnd;
    return false;
  }

  std::string FuncName = reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 1));
  std::string LibName  = reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 2));
  std::string Parms    = reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 3));
  std::string Result   = reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 4));

  // TODO: give more info
  rFcnDtl = FunctionDetail(LibName + "!" + LibName, TypeDetail(), TypedValueDetail::List());

  if (sqlite3_finalize(pStmt) != SQLITE_OK)
  {
    Log::Write("os_windows") << "sqlite3_finalize: " << sqlite3_errmsg(m_Database) << LogEnd;
    return false;
  }

  return true;
}

bool WindowsOperatingSystem::GetStructureDetail(Id StructureId, StructureDetail& rStructDtl) const
{
  return false;
}

bool WindowsOperatingSystem::_OpenDatabaseIfNeeded(void) const
{
  if (m_Database != nullptr)
    return true;

  if (sqlite3_open_v2("windows.db", &m_Database, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK)
  {
    Log::Write("os_windows") << "sqlite3_open: " << sqlite3_errmsg(m_Database) << LogEnd;
    sqlite3_close(m_Database);
    m_Database = nullptr;
    return false;
  }

  return true;
}
