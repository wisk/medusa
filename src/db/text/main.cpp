#include "text_db.hpp"

medusa::Database* GetDatabase(void)  { return new TextDatabase; }

int main(void) { return 0; }
