#include "x86_architecture.hpp"

int main() { return 0; }

Architecture* GetArchitecture()
{
  return new X86Architecture;
}
