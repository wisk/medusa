#include "bind_python.hpp"
#include <medusa/user_configuration.hpp>

#include <Python.h>

#include <mutex>

#include <fmt/format.h>

MEDUSA_NAMESPACE_USE

std::once_flag g_PythonInit;

static bool g_PythonAvailable = false;

static void EnsurePythonInitialized()
{
  std::call_once(g_PythonInit, [](){
    // Pre-check: try to configure Python without aborting
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    status = Py_InitializeFromConfig(&config);
    PyConfig_Clear(&config);
    g_PythonAvailable = !PyStatus_Exception(status);
  });
}

BindingPython::BindingPython(void)
{
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

  auto const AddCurDirToPythonPath = fmt::format(
    "import sys\n"
    "medusa_path = '{}'\n"
    "if not medusa_path in sys.path:\n"
    "  sys.path.append(medusa_path)\n", ModPath);

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

bool BindingPython::Execute(std::string const& rCode)
{
  EnsurePythonInitialized();
  if (!g_PythonAvailable)
    return false;
  return PyRun_SimpleString(rCode.c_str()) == 0;
}

Binding* GetBinding(void) { return new BindingPython; }

int main(void) { return 0; }
