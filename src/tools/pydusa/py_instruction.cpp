#include "pydusa.hpp"

#include <nanobind/nanobind.h>

#include <medusa/instruction.hpp>

namespace nb = nanobind;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  nb::list Instruction_Semantic(Instruction* pInsn)
  {
    nb::list Sem;
    for (auto& rspExpr : pInsn->GetSemantic())
      Sem.append(nb::cast(rspExpr));
    return Sem;
  }

  nb::list Instruction_Operands(Instruction* pInsn)
  {
    nb::list Oprds;
    pInsn->ForEachOperand([&](Expression::SPType spOprdExpr)
    {
      Oprds.append(nb::cast(spOprdExpr));
    });
    return Oprds;
  }

  nb::str Instruction_GetFormat(Instruction* pInsn)
  {
    return nb::str(pInsn->GetFormat());
  }
}

void PydusaInstruction(nb::module_& rMod)
{
  nb::class_<Instruction, Cell>(rMod, "Instruction")
    .def("__str__", &Instruction::ToString)
    .def("__len__", &Instruction::GetSize)
    .def_prop_ro("format", pydusa::Instruction_GetFormat)
    .def_prop_ro("semantic", pydusa::Instruction_Semantic)
    .def_prop_ro("operands", pydusa::Instruction_Operands)
    ;
}
