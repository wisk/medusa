#include "mach-o_loader.hpp"

Loader* GetLoader()  { return new MachOLoader; }
int main() { return 0; }
