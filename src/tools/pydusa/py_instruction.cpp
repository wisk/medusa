#include "pydusa.hpp"

#include <pybind11/pybind11.h>

#include <medusa/instruction.hpp>

namespace py = pybind11;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  py::list Instruction_Semantic(Instruction* pInsn)
  {
    py::list Sem;
    for (auto& rspExpr : pInsn->GetSemantic())
      Sem.append(py::cast(rspExpr));
    return Sem;
  }

  py::list Instruction_Operands(Instruction* pInsn)
  {
    py::list Oprds;
    pInsn->ForEachOperand([&](Expression::SPType spOprdExpr)
    {
      Oprds.append(py::cast(spOprdExpr));
    });
    return Oprds;
  }

  py::str Instruction_GetFormat(Instruction* pInsn)
  {
    return py::str(pInsn->GetFormat());
  }
}

void PydusaInstruction(py::module& rMod)
{
  py::class_<Instruction, Instruction::SPType>(rMod, "Instruction", py::base<Cell>())
    .def("__str__", &Instruction::ToString)
    .def("__len__", &Instruction::GetLength)
    .def_property_readonly("format", pydusa::Instruction_GetFormat)
    .def_property_readonly("semantic", pydusa::Instruction_Semantic)
    .def_property_readonly("operands", pydusa::Instruction_Operands)
    ;
}
