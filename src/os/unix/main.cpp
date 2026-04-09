#include "unix.hpp"

OperatingSystem* GetOperatingSystem() { return new UnixOperatingSystem; }
int main() { return 0; }
