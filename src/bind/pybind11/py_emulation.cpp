#include "py_emulation.hpp"

#include <boost/utility.hpp>
#include <boost/python.hpp>

#include <medusa/emulation.hpp>

namespace bp = boost::python;

MEDUSA_NAMESPACE_USE

void PydusaEmulation(void)
{
  bp::class_<Emulator>("Emulator")
    ;
}