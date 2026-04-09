#include "pe_loader.hpp"

Loader* GetLoader()  { return new PeLoader; }
int main() { return 0; }
