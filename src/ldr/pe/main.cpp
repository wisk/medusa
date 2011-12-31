#include "pe_loader.hpp"

Loader* GetLoader(Database& rDatabase)  { return new PeLoader(rDatabase); }

int main(void) { return 0; }
