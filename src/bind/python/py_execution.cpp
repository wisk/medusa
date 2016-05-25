#include "py_execution.hpp"

#include <boost/python.hpp>

#include <medusa/document.hpp>
#include <medusa/execution.hpp>
#include <medusa/module.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  bool Execution_Initialize(Execution* pExecution, bp::list Args, bp::list Env, bp::str WrkDir)
  {
    std::vector<std::string> VecArgs;
    std::vector<std::string> VecEnv;

    for (u32 i = 0; i < bp::len(Args); ++i)
      VecArgs.push_back(bp::extract<std::string>(Args[i]));

    for (u32 i = 0; i < bp::len(Env); ++i)
      VecEnv.push_back(bp::extract<std::string>(Env[i]));

    std::string StrWrkDir = bp::extract<std::string>(WrkDir);

    return pExecution->Initialize(VecArgs, VecEnv, StrWrkDir);
  }

  std::shared_ptr<Document> Execution_MapModule(Execution* pExecution, bp::str pyModPath, Address const& rAddr)
  {
    Path ModPath = bp::extract<std::string>(pyModPath)().c_str();

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

        auto pGetDbText = rModMgr.LoadModule<medusa::TGetDatabase>(".", "text");
        if (pGetDbText == nullptr)
          return nullptr;
        auto spDbText = medusa::Database::SPType(pGetDbText());

        spDbText->SetBinaryStream(spModBinStrm);
        spModDoc->Use(spDbText);

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

  struct HookCallbackProxy
  {
    HookCallbackProxy(bp::object HookCb) : m_HookCb(HookCb) {}
    Emulator::ReturnType operator()(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
    {
      return bp::extract<Emulator::ReturnType>(m_HookCb(boost::ref(*pCpuCtxt), boost::ref(*pMemCtxt), rAddr));
    }
    bp::object m_HookCb;
  };

  bool Execution_HookInstruction(Execution* pExecution, bp::object HookCb)
  {
    HookCallbackProxy Proxy(HookCb);
    return pExecution->HookInstruction(
      Proxy);
  }

  bool Execution_HookFunction(Execution* pExecution, bp::str FnName, bp::object HookCb)
  {
    HookCallbackProxy Proxy(HookCb);
    return pExecution->HookFunction(
      bp::extract<std::string>(FnName),
      Proxy);
  }

  bool Execution_HookAddr(Execution* pExecution, Address const& rAddr, u32 Type, bp::object HookCb)
  {
    HookCallbackProxy Proxy(HookCb);
    return pExecution->Hook(
      rAddr,
      Type,
      Proxy);
  }

  bp::object Execution_GetFunctionParameter(Execution* pExecution, bp::str CallConv, u16 ParamNo)
  {
    BitVector ParamValue;
    if (!pExecution->GetFunctionParameter(bp::extract<std::string>(CallConv), ParamNo, ParamValue))
      return bp::object();
    // TODO(wisk): handle type larger than 64-bit...
    return bp::object(ParamValue.ConvertTo<u64>());
  }
}

void PydusaExecution(void)
{
  bp::enum_<Emulator::HookType>("Hook")
    .value("UNKNOWN", Emulator::HookUnknown)
    .value("READ",    Emulator::HookOnRead)
    .value("WRITE",   Emulator::HookOnWrite)
    .value("EXECUTE", Emulator::HookOnExecute)
    ;

  bp::enum_<Emulator::ReturnType>("Emulator")
          .value("ERROR",    Emulator::Error)
          .value("BREAK",    Emulator::Break)
          .value("CONTINUE", Emulator::Continue)
          .value("STOP",     Emulator::Stop)
          ;

  bp::class_<Execution, boost::noncopyable>("Execution", bp::init<Document&>())
    .def("init", pydusa::Execution_Initialize)
    .def("set_emulator", &Execution::SetEmulator)
    .def("execute", &Execution::Execute)
    .def("invalidate_cache", &Execution::InvalidateCache)
    .def("map_mod", pydusa::Execution_MapModule)
    .def("hook_insn", pydusa::Execution_HookInstruction)
    .def("hook_fn", pydusa::Execution_HookFunction)
    .def("hook_addr", pydusa::Execution_HookAddr)
    .def("get_hook_name", &Execution::GetHookName)
    .def("get_hook_addr", &Execution::GetHookAddress)
    .def("get_func_param", pydusa::Execution_GetFunctionParameter)
    .def("ret_from_func", &Execution::ReturnFromFunction)
    .add_property("cpu", bp::make_function(&Execution::GetCpuContext, bp::return_internal_reference<>()))
    .add_property("mem", bp::make_function(&Execution::GetMemoryContext, bp::return_internal_reference<>()))
    ;
}
