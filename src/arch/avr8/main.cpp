#include "avr8_architecture.hpp"

Architecture* GetArchitecture(void) { return new Avr8Architecture; }

int main(void) { return 0; }
