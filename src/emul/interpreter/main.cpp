#include "interpreter_emulator.hpp"

Emulator* GetEmulator(void) { return new InterpreterEmulator(); }

int main(void) { return 0; }