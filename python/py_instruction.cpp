#include "py_instruction.hpp"

#include <boost/python.hpp>

#include "medusa/instruction.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaInstruction(void)
{
  bp::class_<Instruction, bp::bases<Cell> >("Instruction", bp::no_init)
    .def("__str__", &Instruction::ToString, bp::return_internal_reference<>())
    .def("__len__", &Instruction::GetLength)
    ;
}
