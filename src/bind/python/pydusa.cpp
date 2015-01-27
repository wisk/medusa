#include <boost/python.hpp>

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
  return true; // TODO: lol i'm lying
}

Binding* GetBinding(void) { return new BindingPython; }

int main(void) { return 0; }

MEDUSA_NAMESPACE_END