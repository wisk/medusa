#include "llvm_emulator.hpp"

Emulator* GetEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, VariableContext* pVarCtxt) { return new LlvmEmulator(pCpuInfo, pCpuCtxt, pMemCtxt, pVarCtxt); }

int main(void) { return 0; }