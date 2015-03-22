#include "dos_loader.hpp"

Loader* GetLoader(void)  { return new DosLoader; }
int main(void) { return 0; }
