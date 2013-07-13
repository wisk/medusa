#include "pe_loader.hpp"

Loader* GetLoader(Document& rDoc)  { return new PeLoader(rDoc); }

int main(void) { return 0; }
