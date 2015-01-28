#include <boost/python.hpp>
#include <mutex>

#include "pydusa.hpp"

#include "py_logger.hpp"
#include "py_exception.hpp"
#include "py_address.hpp"
#include "py_binary_stream.hpp"
#include "py_cell.hpp"
#include "py_configuration.hpp"
#include "py_architecture.hpp"
#include "py_loader.hpp"
#include "py_memory_area.hpp"
#include "py_xrefs.hpp"
#include "py_database.hpp"
#include "py_document.hpp"
#include "py_medusa.hpp"

namespace bp = boost::python;

BOOST_PYTHON_MODULE(pydusa)
{
  PydusaLogger();
  PydusaException();

  PydusaAddress();

  PydusaBinaryStream();
  PydusaCell();
  PydusaMemoryArea();
  PydusaXRefs();
  PydusaDatabase();

  PydusaConfiguration();
  PydusaArchitecture();
  PydusaLoader();

  PydusaDocument();
  PydusaMedusa();
}

MEDUSA_NAMESPACE_BEGIN

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

MEDUSA_NAMESPACE_END