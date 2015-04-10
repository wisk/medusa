#include "py_medusa.hpp"

#include <iostream>

#include <boost/python.hpp>
#include <boost/python/extract.hpp>
#include <medusa/medusa.hpp>
#include <medusa/module.hpp>
#include <medusa/database.hpp>
#include <medusa/loader.hpp>
#include <medusa/architecture.hpp>
#include <medusa/os.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  static bool Medusa_OpenExecutable(Medusa *pCore, bp::str pyExecutablePath, bp::str pyDatabasePath, bool StartAnalyzer)
  {
    Path ExePath = bp::extract<std::string>(pyExecutablePath)().c_str();
    Path DbPath  = bp::extract<std::string>(pyDatabasePath)().c_str();

    return pCore->NewDocument(
    std::make_shared<FileBinaryStream>(ExePath),
    StartAnalyzer,
    [&](Path &rDbPath, std::list<Medusa::Filter> const&)
    {
      rDbPath = DbPath;
      return true;
    });
  }

  static bool Medusa_OpenDatabase(Medusa *pCore, bp::str pyDatabasePath)
  {
    Path DbPath  = bp::extract<std::string>(pyDatabasePath)().c_str();

    return pCore->OpenDocument(
    [&](Path &rDbPath, std::list<Medusa::Filter> const&)
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

  static bp::object Medusa_FormatCell(Medusa* pCore, Address const& rAddress)
  {
    auto spCell = pCore->GetCell(rAddress);
    if (spCell == nullptr)
      return bp::object();

    auto const& rModMgr = ModuleManager::Instance();
    auto spArch = rModMgr.GetArchitecture(spCell->GetArchitectureTag());
    if (spArch == nullptr)
      return bp::object();

    PrintData PD;
    PD.PrependAddress(false);
    if (!spArch->FormatCell(pCore->GetDocument(), rAddress, *spCell, PD))
      return bp::object();

    return bp::str(PD.GetTexts());
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
    .def("fmt_cell", pydusa::Medusa_FormatCell)
    .def("get_insn", pydusa::Medusa_GetInstruction)
    ;
}
