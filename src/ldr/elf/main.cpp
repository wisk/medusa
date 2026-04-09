#include "elf_loader.hpp"

Loader* GetLoader()  { return new ElfLoader; }
int main() { return 0; }
