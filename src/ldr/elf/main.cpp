#include "elf_loader.hpp"

Loader* GetLoader(Database& rDatabase)  { return new ElfLoader(rDatabase); }

int main(void) { return 0; }
