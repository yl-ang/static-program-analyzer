#include "TNode.h"
#include "catch.hpp"

using namespace std; // NOLINT

void require(bool b) {
    REQUIRE(b);
}

TEST_CASE("Right Test") {
    TNode T;
    require(1 == 1);
}

// TEST_CASE("Wrong Test") {
//     require(2 == 1);
// }

TEST_CASE("Another Right Test") {
    require(3 == 3);
}
