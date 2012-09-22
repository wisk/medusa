#include <boost/python.hpp>

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
#include "py_medusa.hpp"

namespace bp = boost::python;

BOOST_PYTHON_MODULE(pydusa)
{
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

  PydusaMedusa();
}