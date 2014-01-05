#include "unix.hpp"

OperatingSystem* GetOperatingSystem(void) { return new UnixOperatingSystem; }
int main(void) { return 0; }
