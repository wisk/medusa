#include "windows.hpp"

OperatingSystem* GetOperatingSystem(void)  { return new WindowsOperatingSystem; }
int main(void) { return 0; }