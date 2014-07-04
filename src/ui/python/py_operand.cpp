#include "py_operand.hpp"

#include <boost/python.hpp>

#include "medusa/operand.hpp"

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaOperand(void)
{
  bp::class_<Operand>("Operand", bp::no_init)
    .add_property("Type",     &Operand::GetType,     &Operand::SetType    )
    .add_property("Reg",      &Operand::GetReg,      &Operand::SetReg     )
    .add_property("SecReg",   &Operand::GetSecReg,   &Operand::SetSecReg  )
    .add_property("Seg",      &Operand::GetSeg,      &Operand::SetSeg     )
    .add_property("Value",    &Operand::GetValue,    &Operand::SetValue   )
    .add_property("SegValue", &Operand::GetSegValue, &Operand::SetSegValue)
  ;
}
