#ifndef PYDUSA_HPP
#define PYDUSA_HPP

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

void PydusaLogger(py::module& rPyMod);
void PydusaException(py::module& rPyMod);

void PydusaAddress(py::module& rPyMod);
void PydusaLabel(py::module& rPyMod);
void PydusaExpression(py::module& rPyMod);
void PydusaBinaryStream(py::module& rPyMod);
void PydusaCell(py::module& rPyMod);
void PydusaInstruction(py::module& rPyMod);
void PydusaMemoryArea(py::module& rPyMod);
void PydusaDatabase(py::module& rPyMod);
void PydusaCompiler(py::module& rPyMod);

void PydusaConfiguration(py::module& rPyMod);
void PydusaArchitecture(py::module& rPyMod);
void PydusaLoader(py::module& rPyMod);

void PydusaDocument(py::module& rPyMod);
void PydusaMedusa(py::module& rPyMod);

void PydusaContext(py::module& rPyMod);
void PydusaExecution(py::module& rPyMod);
void PydusaSymbolic(py::module& rPyMod);

void PydusaCpuInformation(py::module& rPyMod);

void PydusaModule(py::module& rPyMod);

#endif // !PYDUSA_HPP