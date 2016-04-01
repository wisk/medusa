#ifndef ARM_HELPER_HPP
#define ARM_HELPER_HPP

#include <medusa/expression.hpp>
#include "arm_def.hpp"

namespace arm
{
  medusa::u32 RegisterFromValue(char const* pRegTypeName, medusa::u32 RegVal);
}

#endif // !ARM_HELPER_HPP