#include "windows.hpp"

OperatingSystem* GetOperatingSystem()  { return new WindowsOperatingSystem; }
int main() { return 0; }