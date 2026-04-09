#include "bootsector_loader.hpp"

Loader* GetLoader()  { return new BootSectorLoader; }
int main() { return 0; }
