#include "avr8_loader.hpp"

Loader* GetLoader(void)  { return new Avr8Loader; }
int main(void) { return 0; } 
