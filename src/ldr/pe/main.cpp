#include "pe_loader.hpp"

Loader* GetLoader(void)  { return new PeLoader; }
int main(void) { return 0; }
