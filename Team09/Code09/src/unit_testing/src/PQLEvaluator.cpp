
#include "PQLEvaluator.h"

#include "catch.hpp"
using namespace std;
void require(bool b) {
	REQUIRE(b);
}

TEST_CASE("1st Test") {

	PQLEvaluator pe;



	require(1 == 1);
}


