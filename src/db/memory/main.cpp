#include "memory_db.hpp"

medusa::Database* GetDatabase(void)  { return new MemoryDatabase; }

int main(void) { return 0; }
