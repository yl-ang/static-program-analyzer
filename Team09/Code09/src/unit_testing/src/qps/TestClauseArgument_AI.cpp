#include "catch.hpp"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/ExpressionSpec.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Literal.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/Wildcard.h"

// ai-gen start(copilot, 2, e)
// prompt: Write tests for the ClauseArgument class that ensures that the relevant isXXX method returns the correct
// value
TEST_CASE("ClauseArgument isXXX methods") {
    SECTION("isExpressionSpec") {
        std::shared_ptr<ClauseArgument> expressionSpec = std::make_shared<ExpressionSpec>("x");
        REQUIRE(expressionSpec->isExpressionSpec());
        REQUIRE_FALSE(expressionSpec->isInteger());
        REQUIRE_FALSE(expressionSpec->isLiteral());
        REQUIRE_FALSE(expressionSpec->isSynonym());
        REQUIRE_FALSE(expressionSpec->isWildcard());
    }

    SECTION("isInteger") {
        std::shared_ptr<ClauseArgument> integer = std::make_shared<Integer>("4");
        REQUIRE(integer->isInteger());
        REQUIRE_FALSE(integer->isExpressionSpec());
        REQUIRE_FALSE(integer->isLiteral());
        REQUIRE_FALSE(integer->isSynonym());
        REQUIRE_FALSE(integer->isWildcard());
    }

    SECTION("isLiteral") {
        std::shared_ptr<ClauseArgument> literal = std::make_shared<Literal>("v");
        REQUIRE(literal->isLiteral());
        REQUIRE_FALSE(literal->isExpressionSpec());
        REQUIRE_FALSE(literal->isInteger());
        REQUIRE_FALSE(literal->isSynonym());
        REQUIRE_FALSE(literal->isWildcard());
    }

    SECTION("isSynonym") {
        std::shared_ptr<ClauseArgument> synonym = std::make_shared<Synonym>(DesignEntityType::STMT, "s1");
        REQUIRE(synonym->isSynonym());
        REQUIRE_FALSE(synonym->isExpressionSpec());
        REQUIRE_FALSE(synonym->isInteger());
        REQUIRE_FALSE(synonym->isLiteral());
        REQUIRE_FALSE(synonym->isWildcard());
    }

    SECTION("isWildcard") {
        std::shared_ptr<ClauseArgument> wildcard = std::make_shared<Wildcard>();
        REQUIRE(wildcard->isWildcard());
        REQUIRE_FALSE(wildcard->isExpressionSpec());
        REQUIRE_FALSE(wildcard->isInteger());
        REQUIRE_FALSE(wildcard->isLiteral());
        REQUIRE_FALSE(wildcard->isSynonym());
    }
}
// ai-gen end

// ai-gen start(copilot, 0, e)
// prompt: Write tests for the ClauseArgument class that ensures that the values are retrievable
TEST_CASE("ClauseArgument getValue methods") {
    SECTION("ExpressionSpec") {
        std::shared_ptr<ClauseArgument> expressionSpec = std::make_shared<ExpressionSpec>("x");
        REQUIRE(expressionSpec->getValue() == "x");
    }

    SECTION("Integer") {
        std::shared_ptr<ClauseArgument> integer = std::make_shared<Integer>("4");
        REQUIRE(integer->getValue() == "4");
    }

    SECTION("Literal") {
        std::shared_ptr<ClauseArgument> literal = std::make_shared<Literal>("v");
        REQUIRE(literal->getValue() == "v");
    }

    SECTION("Synonym") {
        std::shared_ptr<ClauseArgument> synonym = std::make_shared<Synonym>(DesignEntityType::STMT, "s1");
        REQUIRE(synonym->getValue() == "s1");
    }

    SECTION("Wildcard") {
        std::shared_ptr<ClauseArgument> wildcard = std::make_shared<Wildcard>();
        REQUIRE(wildcard->getValue() == "_");
    }
}
// ai-gen end
