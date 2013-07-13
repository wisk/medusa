#include "gameboy_loader.hpp"

Loader* GetLoader(Document& rDoc)  { return new GameBoyLoader(rDoc); }

int main(void) { return 0; }
