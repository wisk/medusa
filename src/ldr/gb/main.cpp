#include "gameboy_loader.hpp"

Loader* GetLoader(Database& rDatabase)  { return new GameBoyLoader(rDatabase); }

int main(void) { return 0; }
