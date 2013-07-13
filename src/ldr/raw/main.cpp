#include "raw_loader.hpp"

Loader* GetLoader(Document& rDoc)  { return new RawLoader(rDoc); }

int main(void) { return 0; }
