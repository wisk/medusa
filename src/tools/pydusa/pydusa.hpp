#ifndef PYDUSA_HPP
#define PYDUSA_HPP

#include <nanobind/nanobind.h>

namespace nb = nanobind;

void PydusaLogger(nb::module_& rPyMod);
void PydusaException(nb::module_& rPyMod);

void PydusaAddress(nb::module_& rPyMod);
void PydusaLabel(nb::module_& rPyMod);
void PydusaExpression(nb::module_& rPyMod);
void PydusaBinaryStream(nb::module_& rPyMod);
void PydusaCell(nb::module_& rPyMod);
void PydusaInstruction(nb::module_& rPyMod);
void PydusaMemoryArea(nb::module_& rPyMod);
void PydusaDatabase(nb::module_& rPyMod);
void PydusaCompiler(nb::module_& rPyMod);

void PydusaConfiguration(nb::module_& rPyMod);
void PydusaArchitecture(nb::module_& rPyMod);
void PydusaLoader(nb::module_& rPyMod);

void PydusaDocument(nb::module_& rPyMod);
void PydusaMedusa(nb::module_& rPyMod);

void PydusaContext(nb::module_& rPyMod);
void PydusaExecution(nb::module_& rPyMod);
void PydusaSymbolic(nb::module_& rPyMod);

void PydusaCpuInformation(nb::module_& rPyMod);

void PydusaModule(nb::module_& rPyMod);

#endif // !PYDUSA_HPP
