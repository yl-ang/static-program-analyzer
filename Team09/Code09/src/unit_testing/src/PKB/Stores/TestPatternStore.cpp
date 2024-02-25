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

        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();

        REQUIRE(patternStore.hasPattern(1, *wildcardArg1, *wildcardArg2));

        // Cleanup
        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasPattern with Wildcard, Wildcard (Expecting False)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* wildcardArg2 = new Wildcard();

        REQUIRE_FALSE(patternStore.hasPattern(4, *new Wildcard(), *new Wildcard()));

        // Cleanup
        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasPattern with Wildcard, Synonyms (Expecting True)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymsArg2 = new Synonym(DesignEntityType::VARIABLE, "y");

        REQUIRE(patternStore.hasPattern(1, *wildcardArg1, *synonymsArg2));

        // Cleanup
        delete wildcardArg1;
        delete synonymsArg2;
    }

    SECTION("Test hasPattern with Wildcard, Synonyms (Expecting False)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymsArg2 = new Synonym(DesignEntityType::VARIABLE, "z");

        REQUIRE_FALSE(patternStore.hasPattern(1, *wildcardArg1, *synonymsArg2));

        // Cleanup
        delete wildcardArg1;
        delete synonymsArg2;
    }

    SECTION("Test hasPattern with Wildcard, Synonyms (Expecting False)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        ClauseArgument* wildcardArg1 = new Wildcard();
        ClauseArgument* synonymsArg2 = new Synonym(DesignEntityType::VARIABLE, "y");

        REQUIRE_FALSE(patternStore.hasPattern(4, *wildcardArg1, *synonymsArg2));

        // Cleanup
        delete wildcardArg1;
        delete synonymsArg2;
    }

    SECTION("Test hasPattern with Synonyms, Wildcard (Expecting True)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        ClauseArgument* synonymsArg1 = new Synonym(DesignEntityType::VARIABLE, "a");
        ClauseArgument* wildcardArg2 = new Wildcard();

        REQUIRE(patternStore.hasPattern(2, *synonymsArg1, *wildcardArg2));

        // Cleanup
        delete synonymsArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasPattern with Synonyms, Wildcard (Expecting False)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        ClauseArgument* synonymsArg1 = new Synonym(DesignEntityType::VARIABLE, "x");
        ClauseArgument* wildcardArg2 = new Wildcard();

        REQUIRE_FALSE(patternStore.hasPattern(4, *synonymsArg1, *wildcardArg2));

        // Cleanup
        delete synonymsArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasPattern with Synonyms, Synonyms (Expecting True)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        ClauseArgument* synonymsArg1 = new Synonym(DesignEntityType::VARIABLE, "a");
        ClauseArgument* synonymsArg2 = new Synonym(DesignEntityType::VARIABLE, "b");

        REQUIRE(patternStore.hasPattern(2, *synonymsArg1, *synonymsArg2));

        // Cleanup
        delete synonymsArg1;
        delete synonymsArg2;
    }

    SECTION("Test hasPattern with Synonyms, Synonyms (Expecting False)") {
        std::unordered_set<std::pair<StmtNum, std::pair<std::string, std::string>>> patterns = {
            {1, {"x", "y"}}, {2, {"a", "b"}}, {3, {"p", "q"}}};

        patternStore.setPatterns(patterns);

        ClauseArgument* synonymsArg1 = new Synonym(DesignEntityType::VARIABLE, "a");
        ClauseArgument* synonymsArg2 = new Synonym(DesignEntityType::VARIABLE, "c");

        REQUIRE_FALSE(patternStore.hasPattern(3, *synonymsArg1, *synonymsArg2));

        // Cleanup
        delete synonymsArg1;
        delete synonymsArg2;
    }
}
