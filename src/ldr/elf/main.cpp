#include "elf_loader.hpp"

Loader* GetLoader(Document& rDoc)  { return new ElfLoader(rDoc); }

int main(void) { return 0; }
