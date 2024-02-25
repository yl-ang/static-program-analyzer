#include "PKB/Stores/PatternStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/Wildcard.h"

TEST_CASE("PatternStore - All Tests") {
    PatternStore patternStore;

    SECTION("Test hasPattern with Wildcard, Wildcard (Expecting True)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        REQUIRE(patternStore.hasPattern(1, *new Wildcard(), *new Wildcard()));

        // Cleanup
        delete new Wildcard();
        delete new Wildcard();
    }

    SECTION("Test hasPattern with Wildcard, Wildcard (Expecting False)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        REQUIRE_FALSE(patternStore.hasPattern(4, *new Wildcard(), *new Wildcard()));

        // Cleanup
        delete new Wildcard();
        delete new Wildcard();
    }

    SECTION("Test hasPattern with Wildcard, Synonyms (Expecting True)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        REQUIRE(patternStore.hasPattern(1, *new Wildcard(), *new Synonym(DesignEntityType::VARIABLE, "y")));

        // Cleanup
        delete new Wildcard();
        delete new Synonym(DesignEntityType::VARIABLE, "y");
    }

    SECTION("Test hasPattern with Wildcard, Synonyms (Expecting False)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        REQUIRE_FALSE(patternStore.hasPattern(1, *new Wildcard(), *new Synonym(DesignEntityType::VARIABLE, "z")));

        // Cleanup
        delete new Wildcard();
        delete new Synonym(DesignEntityType::VARIABLE, "z");
    }

    SECTION("Test hasPattern with Synonyms, Wildcard (Expecting True)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        REQUIRE(patternStore.hasPattern(2, *new Synonym(DesignEntityType::VARIABLE, "a"), *new Wildcard()));

        // Cleanup
        delete new Synonym(DesignEntityType::VARIABLE, "a");
        delete new Wildcard();
    }

    SECTION("Test hasPattern with Synonyms, Wildcard (Expecting False)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        REQUIRE_FALSE(patternStore.hasPattern(3, *new Synonym(DesignEntityType::VARIABLE, "c"), *new Wildcard()));

        // Cleanup
        delete new Synonym(DesignEntityType::VARIABLE, "c");
        delete new Wildcard();
    }

    SECTION("Test hasPattern with Synonyms, Synonyms (Expecting True)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        REQUIRE(patternStore.hasPattern(2, *new Synonym(DesignEntityType::VARIABLE, "a"),
                                        *new Synonym(DesignEntityType::VARIABLE, "b")));

        // Cleanup
        delete new Synonym(DesignEntityType::VARIABLE, "a");
        delete new Synonym(DesignEntityType::VARIABLE, "b");
    }

    SECTION("Test hasPattern with Synonyms, Synonyms (Expecting False)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        REQUIRE_FALSE(patternStore.hasPattern(3, *new Synonym(DesignEntityType::VARIABLE, "a"),
                                              *new Synonym(DesignEntityType::VARIABLE, "c")));

        // Cleanup
        delete new Synonym(DesignEntityType::VARIABLE, "a");
        delete new Synonym(DesignEntityType::VARIABLE, "c");
    }
}
