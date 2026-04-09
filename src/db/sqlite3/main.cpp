#include "sqlite3_db.hpp"

medusa::Database* GetDatabase()  { return new Sqlite3Database; }

int main() { return 0; }
