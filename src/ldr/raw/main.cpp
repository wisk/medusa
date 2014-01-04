#include "raw_loader.hpp"

Loader* GetLoader(void)  { return new RawLoader; }
int main(void) { return 0; }
