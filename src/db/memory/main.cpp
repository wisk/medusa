#include "memory_db.hpp"

medusa::Database* GetDatabase()  { return new MemoryDatabase; }

int main() { return 0; }
