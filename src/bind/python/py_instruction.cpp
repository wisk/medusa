#include "py_instruction.hpp"

#include <boost/python.hpp>

#include <medusa/instruction.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

namespace pydusa
{
  bp::list Instruction_Semantic(Instruction* pInsn)
  {
    bp::list Sem;
    for (auto& rspExpr : pInsn->GetSemantic())
      Sem.append(rspExpr);
    return Sem;
  }

  bp::list Instruction_Operands(Instruction* pInsn)
  {
    bp::list Oprds;
    pInsn->ForEachOperand([&](Expression::SPType spOprdExpr)
    {
      Oprds.append(spOprdExpr);
    });
    return Oprds;
  }

  bp::str Instruction_GetFormat(Instruction* pInsn)
  {
    return bp::str(pInsn->GetFormat());
  }
}

void PydusaInstruction(void)
{
  bp::class_<Instruction, bp::bases<Cell>, boost::noncopyable>("Instruction", bp::no_init)
    .def("__str__", &Instruction::ToString)
    .def("__len__", &Instruction::GetLength)
    .add_property("fmt", pydusa::Instruction_GetFormat)
    .add_property("sem", pydusa::Instruction_Semantic)
    .add_property("oprds", pydusa::Instruction_Operands)
    ;

  bp::register_ptr_to_python<Instruction::SPType>();
}
