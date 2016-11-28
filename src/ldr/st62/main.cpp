#include "st62_loader.hpp"

Loader* GetLoader(void) { return new ST62Loader; }
int main(void) { return 0; }
