#include "interpreter_emulator.hpp"

Emulator* GetEmulator() { return new InterpreterEmulator(); }

int main() { return 0; }