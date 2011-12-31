#include "raw_loader.hpp"

Loader* GetLoader(Database& rDatabase)  { return new RawLoader(rDatabase); }

int main(void) { return 0; }
