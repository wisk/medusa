#include "interpreter_emulator.hpp"

Emulator* GetEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt) { return new InterpreterEmulator(pCpuInfo, pCpuCtxt, pMemCtxt); }

int main(void) { return 0; }