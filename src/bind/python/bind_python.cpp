#include "bind_python.hpp"
#include <medusa/user_configuration.hpp>

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
  std::string ModPath = ".";
  medusa::UserConfiguration UsrCfg;
  UsrCfg.GetOption("core.modules_path", ModPath);

  auto const AddCurDirToPythonPath = (boost::format(
    "import sys\n"
    "medusa_path = '%s'\n"
    "if not medusa_path in sys.path:\n"
    "  sys.path.append(medusa_path)\n")
    % ModPath).str();
    ;

  if (!Execute(AddCurDirToPythonPath))
    return false;

  auto const BindMedusa =
    "import pydusa\n"
    "pydusa.core = None\n"

    "class HookStdOut:\n"
    "  def write(self, s):\n"
    "    pydusa.std_write(s)\n"
    "  def flush(self):\n"
    "    pydusa.std_flush()\n"
    "  def close(self):\n"
    "    pydusa.std_flush()\n"
    "sys.stdout = HookStdOut()\n"

    "class HookStdErr:\n"
    "  def write(self, s):\n"
    "    pydusa.std_write(s)\n"
    "  def flush(self):\n"
    "    pydusa.std_flush()\n"
    "  def close(self):\n"
    "    pydusa.std_flush()\n"
    "sys.stderr = HookStdErr()\n"
    ;

  if (!Execute(BindMedusa))
    return false;

  try
  {
    auto PydusaCore = bp::object(boost::ref(rCore));
    auto PydusaModule = bp::import("pydusa");
    PydusaModule.attr("core") = PydusaCore;
  }
  catch (bp::error_already_set const&)
  {
    PyErr_Print();
    return false;
  }

  return true;
}

bool BindingPython::Unbind(Medusa& rCore)
{
  auto const UnbindMedusa =
    "sys.stdout = sys.__stdout__\n"
    "sys.stderr = sys.__stderr__\n"

    "pydusa.core = None\n"
    "del pydusa\n";
  return Execute(UnbindMedusa);
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
    PyErr_Print();
    return false;
  }
  return true;
}

Binding* GetBinding(void) { return new BindingPython; }

int main(void) { return 0; }