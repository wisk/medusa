#include "sqlite3_db.hpp"

medusa::Database* GetDatabase(void)  { return new Sqlite3Database; }

int main(void) { return 0; }
