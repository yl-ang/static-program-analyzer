#include "catch.hpp"
#include "sp/ast/Matchable.h"

TEST_CASE("Basic test") {
    SECTION("Matches basic variable") {
        std::string sameInput = "a";
        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString(sameInput);
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString(sameInput);

        REQUIRE(matchable->match(other));
    }

    SECTION("Does not match basic variable") {
        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString("a");
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString("b");

        REQUIRE_FALSE(matchable->match(other));
    }

    SECTION("Matches an expression") {
        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString("a + b");
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString("a + b");

        REQUIRE(matchable->match(other));
    }

    SECTION("Does not match an expression") {
        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString("a + c");
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString("a + b");

        REQUIRE_FALSE(matchable->match(other));
    }

    SECTION("Matches semi-complex expression") {
        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString("(a+b)-c");
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString("a+b-c");

        REQUIRE(matchable->match(other));
    }

    SECTION("Does not match if rearrange parenthesis") {
        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString("a+(b-c)");
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString("a+b-c");

        REQUIRE_FALSE(matchable->match(other));
    }
}
