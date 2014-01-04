#include "gameboy_loader.hpp"

Loader* GetLoader(void) { return new GameBoyLoader; }
int main(void) { return 0; }
