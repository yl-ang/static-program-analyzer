
#include "catch.hpp"
#include "qps/clauses/ClauseEvaluatorUtils.h"

TEST_CASE("Test isIdentLiteral") {
    SECTION("Is Ident Literal") {
        REQUIRE(ClauseEvaluatorUtils::isIdentLiteral("a"));
        REQUIRE(ClauseEvaluatorUtils::isIdentLiteral("h13asdf24"));
        REQUIRE(ClauseEvaluatorUtils::isIdentLiteral("z41adfsadfcc23cs4123"));
    }

    SECTION("Is not Ident Literal") {
        REQUIRE_FALSE(ClauseEvaluatorUtils::isIdentLiteral("1"));
        REQUIRE_FALSE(ClauseEvaluatorUtils::isIdentLiteral("2341123"));
        REQUIRE_FALSE(ClauseEvaluatorUtils::isIdentLiteral("21nsdkfjsa12123"));
    }
}
