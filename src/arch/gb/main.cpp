#include "gameboy_architecture.hpp"

Architecture* GetArchitecture(void) { return new GameBoyArchitecture; }

int main(void) { return 0; }
