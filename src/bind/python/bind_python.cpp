#include "bind_python.hpp"

#include <boost/python.hpp>

#include <mutex>

MEDUSA_NAMESPACE_USE

namespace bp = boost::python;
std::once_flag g_PythonInit;

BindingPython::BindingPython(void)
{
  std::call_once(g_PythonInit, [](){ Py_Initialize(); });
}

BindingPython::~BindingPython(void)
{
}

std::string BindingPython::GetName(void) const
{
  return "python";
}

bool BindingPython::Bind(Medusa& rCore)
{
  if (!Execute("import pydusa"))
    return false;
  if (!Execute("pydusa.core = None"))
    return false;

  try
  {
    auto PydusaCore = bp::object(boost::ref(rCore));
    auto PydusaModule = bp::import("pydusa");
    PydusaModule.attr("core") = PydusaCore;
  }
  catch (bp::error_already_set const&)
  {
    // TODO: remove this or intercept stderr and print message using Log::Write
    PyErr_Print();
    return false;
  }

  return true;
}

//ref: http://www.boost.org/doc/libs/1_57_0/libs/python/doc/tutorial/doc/html/python/embedding.html
bool BindingPython::Execute(std::string const& rCode)
{
  auto MainModule = bp::import("__main__");
  auto MainNamespace = MainModule.attr("__dict__");

  try
  {
    auto Ignored = bp::exec(rCode.c_str(), MainNamespace);
  }
  catch (bp::error_already_set const&)
  {
    // TODO: remove this or intercept stderr and print message using Log::Write
    PyErr_Print();
    return false;
  }
  return true;
}

Binding* GetBinding(void) { return new BindingPython; }

int main(void) { return 0; }