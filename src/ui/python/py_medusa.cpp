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
    [&](BinaryStream::SharedPtr spBinStrm,
        Database::SharedPtr& rspDatabase,
        Loader::SharedPtr& rspLoader,
        Architecture::VectorSharedPtr& rspArchitectures,
        OperatingSystem::SharedPtr& rspOperatingSystem)
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

  static bool Medusa_OpenDatabase(Medusa *pCore, bp::str pyPath)
  {
    return false;
  }
}

void PydusaMedusa(void)
{
  bp::class_<Medusa, boost::noncopyable>("Medusa", bp::init<>())
    .def("OpenExecutable", pydusa::Medusa_OpenExecutable)
    .def("OpenDatabase", pydusa::Medusa_OpenDatabase)
    ;
}
