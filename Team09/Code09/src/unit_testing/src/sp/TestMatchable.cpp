#include "catch.hpp"
#include "sp/ast/Matchable.h"

TEST_CASE("Basic test") {
    SECTION("Matches basic variable") {
        std::vector<std::string> input = {"a"};
        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString(input);

        std::vector<std::string> input2 = {"a"};
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString(input2);

        REQUIRE(matchable->match(other));
    }

    SECTION("Does not match basic variable") {
        std::vector<std::string> input = {"a"};
        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString(input);

        std::vector<std::string> input2 = {"b"};
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString(input2);

        REQUIRE_FALSE(matchable->match(other));
    }

    SECTION("Matches an expression") {
        std::vector<std::string> input = {"a", "+", "b"};
        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString(input);

        std::vector<std::string> input2 = {"a", "+", "b"};
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString(input2);

        REQUIRE(matchable->match(other));
    }

    SECTION("Does not match an expression") {
        std::vector<std::string> input = {"a", "+", "c"};
        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString(input);

        std::vector<std::string> input2 = {"a", "+", "b"};
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString(input2);

        REQUIRE_FALSE(matchable->match(other));
    }

    SECTION("Matches semi-complex expression") {
        std::vector<std::string> input = {"(", "a", "+", "b", ")", "-", "c"};

        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString(input);

        std::vector<std::string> input2 = {"a", "+", "b", "-", "c"};
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString(input2);

        REQUIRE(matchable->match(other));
    }

    SECTION("Does not match if rearrange parenthesis") {
        std::vector<std::string> input = {"a", "+", "(", "b", "-", "c", ")"};

        std::shared_ptr<Matchable> matchable = MatchableBuilder::fromExpressionString(input);

        std::vector<std::string> input2 = {"a", "+", "b", "-", "c"};
        std::shared_ptr<Matchable> other = MatchableBuilder::fromExpressionString(input2);

        REQUIRE_FALSE(matchable->match(other));
    }
}
