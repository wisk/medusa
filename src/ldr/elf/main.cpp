#include "elf_loader.hpp"

Loader* GetLoader(void)  { return new ElfLoader; }
int main(void) { return 0; }
