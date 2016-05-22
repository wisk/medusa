#include <pybind11/pybind11.h>

#include "pydusa.hpp"

namespace py = pybind11;

PYBIND11_PLUGIN(pydusa)
{
  py::module m("pydusa", "pydusa is python binding for medusa");

  PydusaLogger(m);
  PydusaException(m);

  PydusaAddress(m);
  PydusaLabel(m);
  PydusaExpression(m);
  PydusaBinaryStream(m);
  PydusaCell(m);
  PydusaInstruction(m);
  PydusaMemoryArea(m);
  PydusaXRefs(m);
  PydusaDatabase(m);

  PydusaConfiguration(m);
  PydusaArchitecture(m);
  PydusaLoader(m);

  PydusaDocument(m);
  PydusaMedusa(m);

  PydusaContext(m);
  PydusaExecution(m);
  PydusaSymbolic(m);

  PydusaCpuInformation(m);

  return m.ptr();
}