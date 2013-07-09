#include "dummy_serialize.hpp"

int main(void) { return 0; }

medusa::Serialize* GetSerialize(void) { return new DummySerialize; }