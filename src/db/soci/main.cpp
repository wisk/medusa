#include "soci_db.hpp"

medusa::Database* GetDatabase(void)  { return new SociDatabase; }

int main(void) { return 0; }
