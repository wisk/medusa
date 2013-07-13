#include "bootsector_loader.hpp"

Loader* GetLoader(Document& rDoc)  { return new BootSectorLoader(rDoc); }

int main(void) { return 0; }
