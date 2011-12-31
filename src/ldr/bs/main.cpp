#include "bootsector_loader.hpp"

Loader* GetLoader(Database& rDatabase)  { return new BootSectorLoader(rDatabase); }

int main(void) { return 0; }
