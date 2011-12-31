#include "sqlite_serialize.hpp"

int main(void) { return 0; }

medusa::Serialize* GetSerialize(void) { return new SqliteSerialize; }
