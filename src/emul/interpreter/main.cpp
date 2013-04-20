#include "interpreter_emulator.hpp"

Emulator* GetEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt, VariableContext* pVarCtxt) { return new InterpreterEmulator(pCpuInfo, pCpuCtxt, pMemCtxt, pVarCtxt); }

int main(void) { return 0; }