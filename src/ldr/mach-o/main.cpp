#include "mach-o_loader.hpp"

Loader* GetLoader(void)  { return new MachOLoader; }
int main(void) { return 0; }
