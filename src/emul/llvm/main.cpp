#include "llvm_emulator.hpp"

Emulator* GetEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt) { return new LlvmEmulator(pCpuInfo, pCpuCtxt, pMemCtxt); }

int main() { return 0; }