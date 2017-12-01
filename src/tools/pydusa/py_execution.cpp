#include "pydusa.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <medusa/document.hpp>
#include <medusa/execution.hpp>
#include <medusa/module.hpp>
#include <medusa/user_configuration.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  std::shared_ptr<Document> Execution_MapModule(Execution* pExecution, std::string const& rModPath, std::string const& rModDbPath, Address const& rAddr)
  {
    Path ModPath = rModPath;

    auto* pMemCtxt = pExecution->GetMemoryContext();
    auto* pCpuCtxt = pExecution->GetCpuContext();

    if (pMemCtxt == nullptr)
      return nullptr;
    if (pCpuCtxt == nullptr)
      return nullptr;

    try
    {
      auto spModBinStrm = std::make_shared<FileBinaryStream>(ModPath);

      auto& rModMgr     = ModuleManager::Instance();
      auto const& rLdrs = rModMgr.GetLoaders();
      auto spArch       = pExecution->GetArchitecture();
      auto spModDoc     = std::make_shared<Document>();

      if (spArch == nullptr)
        return nullptr;

      for (auto spLdr : rLdrs)
      {
        if (!spLdr->IsCompatible(*spModBinStrm))
          continue;

        UserConfiguration UserCfg;
        auto DbModPath = UserCfg.GetOption("core.modules_path");
        if (DbModPath.empty())
          DbModPath = ".";
        auto pGetDb = rModMgr.LoadModule<medusa::TGetDatabase>(DbModPath, "soci");
        if (pGetDb == nullptr)
        {
          Log::Write("pydusa") << "failed to get database module" << LogEnd;
          return nullptr;
        }

        auto spDbMod = pGetDb();
        if (!spDbMod->Create(rModDbPath, true))
        {
          Log::Write("pydusa") << "failed to create database at " << rModDbPath << LogEnd;
        }
        auto spDb = medusa::Database::SPType(spDbMod);

        spDb->SetBinaryStream(spModBinStrm);
        spModDoc->Open(spDb);

        if (!spLdr->Map(*spModDoc, { spArch }, rAddr))
        {
          Log::Write("pydusa") << "failed to map " << ModPath.string() << " at " << rAddr << LogEnd;
          return nullptr;
        }

        if (!pMemCtxt->MapDocument(*spModDoc, pCpuCtxt))
        {
          Log::Write("pydusa") << "failed to map " << ModPath.string() << " at " << rAddr << " to memory context" << LogEnd;
          return nullptr;
        }

        return spModDoc;
      }
    }
    catch (Exception const& e)
    {
      Log::Write("pydusa") << e.What() << LogEnd;
      return nullptr;
    }

    Log::Write("pydusa").Level(LogError) << "unable to find suitable loader for file: " << ModPath.string() << LogEnd;
    return nullptr;
  }

  bool Execution_HookAddress(Execution* pExecution, Address const& rAddress, Emulator::HookType Type, Emulator::HookCallback Callback)
  {
    return pExecution->Hook(rAddress, Type, Callback);
  }

  bool Execution_HookLabel(Execution* pExecution, std::string const& rLabelName, Emulator::HookType Type, Emulator::HookCallback Callback)
  {
    return pExecution->Hook(rLabelName, Type, Callback);
  }

  py::object Execution_GetFunctionParameter(Execution* pExecution, std::string const& rCallConv, u32 ParamNo)
  {
    BitVector ParamVal;
    if (!pExecution->GetFunctionParameter(rCallConv, ParamNo, ParamVal))
      return py::none();
    return py::cast(ParamVal.ConvertTo<u64>());
  }

  py::object Execution_GetCallingConvention(Execution const* pExecution, Address const& rAddress)
  {
    std::string CallConv;
    if (!pExecution->GetCallingConvention(CallConv, rAddress))
      return py::none();
    return py::cast(CallConv);
  }
}

void PydusaExecution(py::module& rMod)
{
  py::enum_<Emulator::HookType>(rMod, "Hook")
    .value("UNKNOWN", Emulator::HookUnknown)
    .value("READ",    Emulator::HookOnRead)
    .value("WRITE",   Emulator::HookOnWrite)
    .value("EXECUTE", Emulator::HookOnExecute)
    ;

  py::enum_<Emulator::ReturnType>(rMod, "Emulator")
    .value("ERROR",    Emulator::Error)
    .value("BREAK",    Emulator::Break)
    .value("CONTINUE", Emulator::Continue)
    .value("STOP",     Emulator::Stop)
    ;

  py::class_<Execution>(rMod, "Execution")
    .def(py::init<Document&>())

    .def("init", &Execution::Initialize)
    .def("set_emulator", &Execution::SetEmulator)
    .def("execute", &Execution::Execute)
    .def("invalidate_cache", &Execution::InvalidateCache)
    .def("map_module", pydusa::Execution_MapModule)
    .def("hook_instruction", &Execution::HookInstruction)
    .def("hook_address", pydusa::Execution_HookAddress)
    .def("hook_label", pydusa::Execution_HookLabel)
    .def("hook_imported_function", &Execution::HookFunction)
    .def("get_hook_name", &Execution::GetHookName)
    .def("get_hook_address", &Execution::GetHookAddress)
    .def("get_calling_convention", pydusa::Execution_GetCallingConvention)
    .def("get_function_parameter", pydusa::Execution_GetFunctionParameter)
    .def("return_from_function", &Execution::ReturnFromFunction)
    .def_property_readonly("cpu", &Execution::GetCpuContext, py::return_value_policy::reference_internal)
    .def_property_readonly("mem", &Execution::GetMemoryContext, py::return_value_policy::reference_internal)
    ;
}
