#include "gameboy_loader.hpp"

Loader* GetLoader() { return new GameBoyLoader; }
int main() { return 0; }
