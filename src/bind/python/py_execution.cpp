#include "py_execution.hpp"

#include <boost/python.hpp>

#include <medusa/document.hpp>
#include <medusa/execution.hpp>

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

  struct HookCallbackProxy
  {
    HookCallbackProxy(bp::object HookCb) : m_HookCb(HookCb) {}
    void operator()(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, Address const& rAddr)
    {
      m_HookCb(boost::ref(*pCpuCtxt), boost::ref(*pMemCtxt), rAddr);
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
    return false;
  }
}

void PydusaExecution(void)
{
  bp::class_<Execution, boost::noncopyable>("Execution", bp::init<Document&>())
    .def("init", pydusa::Execution_Initialize)
    .def("set_emulator", &Execution::SetEmulator)
    .def("execute", &Execution::Execute)
    .def("hook_insn", pydusa::Execution_HookInstruction)
    .def("hook_fn", pydusa::Execution_HookFunction)
    .def("get_hook_name", &Execution::GetHookName)
    .add_property("cpu", bp::make_function(&Execution::GetCpuContext, bp::return_internal_reference<>()))
    .add_property("mem", bp::make_function(&Execution::GetMemoryContext, bp::return_internal_reference<>()))
    ;
}