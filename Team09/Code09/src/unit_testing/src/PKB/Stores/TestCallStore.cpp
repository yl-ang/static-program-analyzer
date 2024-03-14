#include "PKB/Stores/CallStore.h"
#include "catch.hpp"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/Wildcard.h"

TEST_CASE("CallStore - Call Tests") {
    CallStore callStore;
    callStore.setCallStore(
        {{"First", "Second"}, {"First", "Third"}, {"Second", "Third"}, {"Third", "Fouth"}, {"Second", "Fouth"}});

    SECTION("Test getCaller") {
        REQUIRE(callStore.getCaller("First").empty());
        REQUIRE(callStore.getCaller("Second") == std::unordered_set<Procedure>{"First"});
        REQUIRE(callStore.getCaller("Third") == std::unordered_set<Procedure>{"First", "Second"});
        REQUIRE(callStore.getCaller("Fouth") == std::unordered_set<Procedure>{"Second", "Third"});
    }

    SECTION("Test getCallee") {
        REQUIRE(callStore.getCallee("First") == std::unordered_set<Procedure>{"Second", "Third"});
        REQUIRE(callStore.getCallee("Second") == std::unordered_set<Procedure>{"Third", "Fouth"});
        REQUIRE(callStore.getCallee("Third") == std::unordered_set<Procedure>{"Fouth"});
        REQUIRE(callStore.getCallee("Fouth").empty());
    }

    SECTION("Test hasCallRelationship") {
        REQUIRE(callStore.hasCallRelationship("First", "Second"));
        REQUIRE(callStore.hasCallRelationship("First", "Third"));
        REQUIRE(callStore.hasCallRelationship("Second", "Fouth"));
        REQUIRE(callStore.hasCallRelationship("Second", "Third"));
        REQUIRE(callStore.hasCallRelationship("Third", "Fouth"));

        REQUIRE_FALSE(callStore.hasCallRelationship("First", "First"));
        REQUIRE_FALSE(callStore.hasCallRelationship("First", "Fouth"));
        REQUIRE_FALSE(callStore.hasCallRelationship("Third", "First"));
        REQUIRE_FALSE(callStore.hasCallRelationship("Third", "Second"));
        REQUIRE_FALSE(callStore.hasCallRelationship("Fouth", "First"));
    }

    SECTION("Test hasCallRelationship with Wildcards arg1 and arg2, non-empty nextStore (Expecting True)") {
        ClauseArgument *wildcardArg1 = new Wildcard();
        ClauseArgument *wildcardArg2 = new Wildcard();
        REQUIRE(callStore.hasCallRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasCallRelationship with Wildcard arg1 and PROCEDURE arg2 (Expecting True)") {
        ClauseArgument *wildcardArg1 = new Wildcard();
        ClauseArgument *SynonymArg2 = new Synonym(DesignEntityType::PROCEDURE, "Second");
        REQUIRE(callStore.hasCallRelationship(*wildcardArg1, *SynonymArg2));

        delete wildcardArg1;
        delete SynonymArg2;
    }

    SECTION("Test hasCallRelationship with Wildcard arg1 and PROCEDURE arg2 (Expecting False)") {
        ClauseArgument *wildcardArg1 = new Wildcard();
        ClauseArgument *SynonymArg2 = new Synonym(DesignEntityType::PROCEDURE, "First");
        REQUIRE_FALSE(callStore.hasCallRelationship(*wildcardArg1, *SynonymArg2));

        delete wildcardArg1;
        delete SynonymArg2;
    }

    SECTION("Test hasCallRelationship with PROCEDURE arg1 and Wildcard arg2 (Expecting True)") {
        ClauseArgument *SynonymArg1 = new Synonym(DesignEntityType::PROCEDURE, "Second");
        ClauseArgument *wildcardArg2 = new Wildcard();
        REQUIRE(callStore.hasCallRelationship(*SynonymArg1, *wildcardArg2));

        delete SynonymArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasCallRelationship with PROCEDURE arg1 and Wildcard arg2 (Expecting False)") {
        ClauseArgument *SynonymArg1 = new Synonym(DesignEntityType::PROCEDURE, "Fouth");
        ClauseArgument *wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(callStore.hasCallRelationship(*SynonymArg1, *wildcardArg2));

        delete SynonymArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasCallRelationship with PROCEDURE arg1 and PROCEDURE arg2 (Expecting True)") {
        ClauseArgument *SynonymArg1 = new Synonym(DesignEntityType::PROCEDURE, "First");
        ClauseArgument *SynonymArg2 = new Synonym(DesignEntityType::PROCEDURE, "Second");
        REQUIRE(callStore.hasCallRelationship(*SynonymArg1, *SynonymArg2));

        delete SynonymArg1;
        delete SynonymArg2;
    }

    SECTION("Test hasCallRelationship with PROCEDURE arg1 and PROCEDURE arg2 (Expecting False)") {
        ClauseArgument *SynonymArg1 = new Synonym(DesignEntityType::PROCEDURE, "First");
        ClauseArgument *SynonymArg2 = new Synonym(DesignEntityType::PROCEDURE, "Fouth");
        REQUIRE_FALSE(callStore.hasCallRelationship(*SynonymArg1, *SynonymArg2));

        delete SynonymArg1;
        delete SynonymArg2;
    }
}
