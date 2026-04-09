#include "arm_architecture.hpp"

int main() { return 0; }

Architecture* GetArchitecture()
{
  return new ArmArchitecture;
}