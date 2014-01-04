#include "bootsector_loader.hpp"

Loader* GetLoader(void)  { return new BootSectorLoader; }
int main(void) { return 0; }
