#include "TNode.h"
#include "catch.hpp"

using namespace std; // NOLINT

void require(bool b) {
    REQUIRE(b);
}

TEST_CASE("Test 1") {
    TNode T;
    require(1 == 1);
}
