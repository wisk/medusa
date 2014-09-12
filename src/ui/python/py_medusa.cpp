#include "py_medusa.hpp"

#include <iostream>

#include <boost/python.hpp>
#include <boost/python/extract.hpp>
#include "medusa/medusa.hpp"
#include "medusa/module.hpp"
#include "medusa/database.hpp"
#include "medusa/loader.hpp"
#include "medusa/architecture.hpp"
#include "medusa/os.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  static bool Medusa_OpenExecutable(Medusa *pCore, bp::str pyExecutablePath, bp::str pyDatabasePath)
  {
    fs::path ExePath = bp::extract<std::string>(pyExecutablePath)().c_str();
    fs::path DbPath  = bp::extract<std::string>(pyDatabasePath)().c_str();

    return pCore->NewDocument(
    ExePath,
    [&](fs::path &rDbPath, std::list<Medusa::Filter> const&)
    {
      rDbPath = DbPath;
      return true;
    },
    [&](BinaryStream::SPType spBinStrm,
        Database::SPType& rspDatabase,
        Loader::SPType& rspLoader,
        Architecture::VSPType& rspArchitectures,
        OperatingSystem::SPType& rspOperatingSystem)
    {
      auto& rModMgr = ModuleManager::Instance();
      auto AllDbs = rModMgr.GetDatabases();
      if (AllDbs.empty())
        return false;
      rspDatabase =  AllDbs.front();
      auto AllLdrs = rModMgr.GetLoaders();
      if (AllLdrs.empty())
        return false;
      rspLoader = AllLdrs.front();
      rspArchitectures = rModMgr.GetArchitectures();
      rspLoader->FilterAndConfigureArchitectures(rspArchitectures);
      if (rspArchitectures.empty())
        return false;
      rspOperatingSystem = rModMgr.GetOperatingSystem(rspLoader, rspArchitectures.front());
      return true;
    },
    [](){ return true; }, [](){ return true; });
  }

  static bool Medusa_OpenDatabase(Medusa *pCore, bp::str pyDatabasePath)
  {
    fs::path DbPath  = bp::extract<std::string>(pyDatabasePath)().c_str();

    return pCore->OpenDocument(
    [&](fs::path &rDbPath, std::list<Medusa::Filter> const&)
    {
      rDbPath = DbPath;
      return true;
    });
  }

  static Document& Medusa_GetDocument(Medusa* pCore)
  {
    return pCore->GetDocument();
  }

  static Cell::SPType Medusa_GetCell(Medusa* pCore, Address const& rAddress)
  {
    return pCore->GetCell(rAddress);
  }

  static Instruction::SPType Medusa_GetInstruction(Medusa* pCore, Address const& rAddress)
  {
    auto spCell = pCore->GetCell(rAddress);
    if (spCell == nullptr)
      return nullptr;
    if (spCell->GetType() != Cell::InstructionType)
      return nullptr;
    return std::static_pointer_cast<Instruction>(spCell);
  }
}

void PydusaMedusa(void)
{
  bp::class_<Medusa, boost::noncopyable>("Medusa", bp::init<>())
    .def("open_exe", pydusa::Medusa_OpenExecutable)
    .def("open_db",  pydusa::Medusa_OpenDatabase)

    .def("wait_for_tasks", &Medusa::WaitForTasks)

    .add_property("document", bp::make_function(pydusa::Medusa_GetDocument,
      bp::return_value_policy<bp::reference_existing_object>()))

    .def("get_cell", pydusa::Medusa_GetCell)
    .def("get_insn", pydusa::Medusa_GetInstruction)
    ;
}
