#include "py_instruction.hpp"

#include <boost/python.hpp>

#include "medusa/instruction.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaInstruction(void)
{
  bp::class_<Instruction, Instruction::SPType, bp::bases<Cell>, boost::noncopyable>
    ("Instruction", bp::no_init)
    .def("__len__", &Instruction::GetLength)
    ;
}
