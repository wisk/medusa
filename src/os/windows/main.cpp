#include "windows.hpp"

OperatingSystem* GetOperatingSystem(Database& rDatabase)  { return new WindowsOperatingSystem(rDatabase); }

int main(void) { return 0; }