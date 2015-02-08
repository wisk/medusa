#include "py_instruction.hpp"

#include <boost/python.hpp>

#include <medusa/instruction.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaInstruction(void)
{
  bp::class_<Instruction, /*bp::bases<Cell>,*/ /*Instruction::SPType, */boost::noncopyable>("Instruction", bp::no_init)
    .def("__str__", &Instruction::ToString)
    .def("__len__", &Instruction::GetLength)
    .def("get_oprd", &Instruction::GetOperand)
    .def("get_oprd_nr", &Instruction::GetNumberOfOperand)
    ;

  bp::register_ptr_to_python<Instruction::SPType>();
}
