#include "windows.hpp"
#include "medusa/module.hpp"

#include "sqlite3.h"

#include <iomanip>
#include <sstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

// FIXME: the code is x86 specific

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

bool WindowsOperatingSystem::IsSupported(Loader const& rLdr, Architecture const& rArch) const
{
  // At this time, only Windows on x86 is supported
  if ((rLdr.GetName() == "PE" || rLdr.GetName() == "PE+") && rArch.GetName() == "x86")
    return true;

  return false;
}

bool WindowsOperatingSystem::InitializeContext(
  Document const& rDoc,
  CpuContext& rCpuCtxt, MemoryContext& rMemCtxt,
  std::vector<std::string> const& rArgs, std::vector<std::string> const& rEnv, std::string const& rCurWrkDir) const
{
  CpuInformation const& rCpuInfo = rCpuCtxt.GetCpuInformation();

  auto SetSeg = [&](char const* pSegRegName, u16 SegRegVal, u64 LinAddr) -> bool
  {
    auto IdSeg = rCpuInfo.ConvertNameToIdentifier(pSegRegName);
    if (IdSeg == 0)
      return false;
    if (!rCpuCtxt.WriteRegister(IdSeg, SegRegVal))
      return false;
    if (!rCpuCtxt.AddMapping(Address(SegRegVal, 0x0), LinAddr))
      return false;
    return true;
  };

  // ref: https://code.google.com/p/corkami/wiki/InitialValues
  u64 Teb32LinAddr = 0x7ffde000;
  u64 Peb32LinAddr = 0x7ffdf000;
  u64 Teb64LinAddr = 0x77ffde000000;
  u64 Peb64LinAddr = 0x77ffdf000000;

  if (!SetSeg("cs", 0x23, 0x0))
    return false;
  if (!SetSeg("ds", 0x2b, 0x0))
    return false;
  if (!SetSeg("es", 0x2b, 0x0))
    return false;
  if (!SetSeg("fs", 0x63, Teb32LinAddr))
    return false;
  if (!SetSeg("gs", 0x6b, Teb64LinAddr))
    return false;

  auto StartAddr = rDoc.GetAddressFromLabelName("start");
  u64 StartAddrVal = StartAddr.GetOffset();
  auto IdD = rCpuInfo.ConvertNameToIdentifier("rdx"); // it doesn't matter to use rdx instead of ecx or cx
  if (!rCpuCtxt.WriteRegister(IdD, StartAddrVal))
    return false;

  auto const ReadWrite = MemoryArea::Access::Read | MemoryArea::Access::Write;

  // TODO: create a fake _TEB/_PEB
  if (!rMemCtxt.AllocateMemory(Teb32LinAddr, 0x1000, ReadWrite, nullptr))
    return false;
  // Init TIB::Self
  if (!rMemCtxt.WriteMemory(Teb32LinAddr + 0x18, static_cast<u32>(Teb32LinAddr)))
    return false;
  // Allocate fake _PEB
  if (!rMemCtxt.AllocateMemory(Peb32LinAddr, 0x1000, ReadWrite, nullptr))
    return false;
  // Write _TEB::Peb
  if (!rMemCtxt.WriteMemory(Teb32LinAddr + 0x30, static_cast<u32>(Peb32LinAddr)))
    return false;
  // Set _PEB::BeingDebugged to false
  if (!rMemCtxt.WriteMemory(Peb32LinAddr + 0x2, static_cast<u8>(0x00)))
    return false;

  u32 StkReg = rCpuInfo.GetRegisterByType(CpuInformation::StackPointerRegister, rCpuCtxt.GetMode());
  if (StkReg == 0)
    return false;
  u32 StkRegBitSize = rCpuInfo.GetSizeOfRegisterInBit(StkReg);
  u64 StkAddr = 0x200000;
  u32 StkSize = 0x10000;
  if (!rMemCtxt.AllocateMemory(StkAddr, StkSize, ReadWrite, nullptr))
    return false;
  StkAddr += StkSize;
  StkAddr -= 0x8 * 4; // home space http://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64/
  if (!rCpuCtxt.WriteRegister(StkReg, &StkAddr, StkRegBitSize)) // FIXME: should not be endian safe...
    return false;

  // Set default flags
  u32 ZF = rCpuInfo.ConvertNameToIdentifier("zf");
  u32 PF = rCpuInfo.ConvertNameToIdentifier("pf");
  u32 IF = rCpuInfo.ConvertNameToIdentifier("if");
  if (!rCpuCtxt.WriteRegister(ZF, true))
    return false;
  if (!rCpuCtxt.WriteRegister(PF, true))
    return false;
  if (!rCpuCtxt.WriteRegister(IF, true))
    return false;

  return true;
}

bool WindowsOperatingSystem::AnalyzeFunction(Document& rDoc, Address const& rAddress)
{
  return true;
}

Expression::VSPType WindowsOperatingSystem::ExecuteSymbol(Document& rDoc, Address const& rSymAddr)
{
  return {};
}

bool WindowsOperatingSystem::ProvideDetails(Document& rDoc) const
{
  if (!_OpenDatabaseIfNeeded())
    return false;

  rDoc.ForEachLabel([&](Address const& rAddress, Label const& rLabel)
  {
    bool IsCode = (rLabel.GetType() & Label::CellMask) == Label::Code;
    bool IsFunc = (rLabel.GetType() & Label::CellMask) == Label::Function;

    if (!IsCode && !IsFunc)
      return;

    Id FuncId = Sha1(rLabel.GetName());
    FunctionDetail FuncDtl;
    if (!GetFunctionDetail(FuncId, FuncDtl))
      return;

    if (!rDoc.SetFunctionDetail(FuncId, FuncDtl))
    {
      Log::Write("os_windows") << "unable to set function detail for " << rLabel.GetName() << " @" << rAddress << LogEnd;
      return;
    }

    if (!rDoc.BindDetailId(rAddress, 0, FuncId))
    {
      Log::Write("os_windows") << "unable to bind id for " << rLabel.GetName() << " @" << rAddress << LogEnd;
      return;
    }
  });

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

  std::ostringstream Query;

  Query << "SELECT * FROM Functions WHERE id LIKE '";
  // HACK: This is a workaround since GUID is 128-bit long and SHA-1 digest is 160-bit (stupid mistake)
  // We could: use MD5 instead, pray god not to have hash collision, or redesign ID...
  auto pIdValues = reinterpret_cast<u8 const*>(FunctionId.data);
  for (size_t i = 0; i < FunctionId.size(); ++i)
    Query << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(pIdValues[i]);
  Query << "%'";

  sqlite3_stmt *pStmt = nullptr;
  if (sqlite3_prepare_v2(m_Database, Query.str().c_str(), -1, &pStmt, nullptr) != SQLITE_OK)
  {
    Log::Write("os_windows") << "sqlite3_prepare: " << sqlite3_errmsg(m_Database) << LogEnd;
    return false;
  }

  if (sqlite3_step(pStmt) != SQLITE_ROW)
  {
    sqlite3_finalize(pStmt);
    return false;
  }

  std::string FuncName = reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 1)) ? reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 1)) : "";
  std::string LibName  = reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 2)) ? reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 2)) : "";
  std::string Parms    = reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 3)) ? reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 3)) : "";
  std::string Result   = reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 4)) ? reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 4)) : "";

  if (sqlite3_finalize(pStmt) != SQLITE_OK)
  {
    Log::Write("os_windows") << "sqlite3_finalize: " << sqlite3_errmsg(m_Database) << LogEnd;
    return false;
  }

  if (FuncName.empty())
    return false;

  std::string Name = LibName.empty() ? FuncName : (LibName + "!" + FuncName);

  TypeDetail ResType;
  TypedValueDetail::List ParmsTpValList;

  if (Result.empty())
    return false;

  if (!_GetTypeFromDatabase(Result, ResType))
    return false;

  if (Parms.empty())
  {
    rFcnDtl = FunctionDetail(Name, ResType, TypedValueDetail::List());
    return true;
  }

  std::vector<std::string> TypedParms;
  boost::algorithm::split(TypedParms, Parms, boost::algorithm::is_any_of(";"));

  for (auto const& rTypedParm : TypedParms)
  {
    std::vector<std::string> CurParm;
    boost::algorithm::split(CurParm, rTypedParm, boost::algorithm::is_any_of(","));
    if (CurParm.size() != 2)
    {
      Log::Write("os_windows") << "malformed parameter typed value" << LogEnd;
      return false;
    }

    TypeDetail ParmTpDtl;
    if (!_GetTypeFromDatabase(CurParm[0], ParmTpDtl))
    {
      Log::Write("os_windows") << "unknown type for one of parameters" << LogEnd;
      return false;
    }

    ValueDetail ParmValDtl(CurParm[1]);

    ParmsTpValList.push_back(TypedValueDetail(std::make_shared<TypeDetail>(ParmTpDtl), ParmValDtl));
  }

  rFcnDtl = FunctionDetail(Name, ResType, ParmsTpValList);
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

bool WindowsOperatingSystem::_GetTypeFromDatabase(std::string const& rTypeId, TypeDetail& rTpDtl) const
{
  if (!_OpenDatabaseIfNeeded())
    return false;

  std::ostringstream Query;
  Query << "SELECT * FROM Types WHERE id = '" << rTypeId << "'";

  sqlite3_stmt *pStmt = nullptr;
  if (sqlite3_prepare_v2(m_Database, Query.str().c_str(), -1, &pStmt, nullptr) != SQLITE_OK)
  {
    Log::Write("os_windows") << "sqlite3_prepare: " << sqlite3_errmsg(m_Database) << LogEnd;
    return false;
  }

  if (sqlite3_step(pStmt) != SQLITE_ROW)
  {
    sqlite3_finalize(pStmt);
    return false;
  }

  auto pTypeName = reinterpret_cast<char const*>(sqlite3_column_text(pStmt, 1));
  if (pTypeName == nullptr)
  {
    sqlite3_finalize(pStmt);
    return false;
  }

  rTpDtl = TypeDetail(pTypeName, TypeDetail::UnknownType, 0);
  sqlite3_finalize(pStmt);

  return true;
}

bool WindowsOperatingSystem::GetDefaultCallingConvention(Document const& rDoc, std::string& rCallingConvention, Address const& rAddress) const
{
  auto spInsn = rDoc.GetCell(rAddress); // TODO(wisk): make sure this is an instruction
  if (spInsn == nullptr)
    return false;
  auto ArchTag = spInsn->GetArchitectureTag();
  auto Mode = spInsn->GetMode();

  auto const& rModMgr = ModuleManager::Instance();
  auto spArch = rModMgr.GetArchitecture(ArchTag);

  if (spArch->GetName() == "x86")
  {
    switch (Mode)
    {
    default:
      return false;
    case 16:
    case 32:
      rCallingConvention = "stdcall";
      return true;
    case 64:
      rCallingConvention = "ms_x64";
      return true;
    }
  }

  return false;
}