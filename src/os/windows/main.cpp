#include "windows.hpp"

OperatingSystem* GetOperatingSystem(Document& rDoc)  { return new WindowsOperatingSystem(rDoc); }

int main(void) { return 0; }