#include "z80_architecture.hpp"

Architecture* GetArchitecture(void) { return new Z80Architecture; }

int main(void) { return 0; }
