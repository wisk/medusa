#include "unix.hpp"

OperatingSystem* GetOperatingSystem(Document& rDoc)
{
  return new UnixOperatingSystem(rDoc);
}

int main(void) { return 0; }
