#include "medusa/endian.hpp"

MEDUSA_NAMESPACE_BEGIN

bool TestEndian(EEndianness Endianness)
{
#if defined(MEDUSA_LITTLE_ENDIAN)
  return (Endianness == BigEndian);
#elif defined(MEDUSA_BIG_ENDIAN)
  return (Endianness == LittleEndian);
#else
# error Unable to determine the current endianness
#endif
}

MEDUSA_NAMESPACE_END
