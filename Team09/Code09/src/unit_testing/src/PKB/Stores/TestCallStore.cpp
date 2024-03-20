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
TEST_CASE("CallStore - CallStar Tests") {
    CallStore callStore;
    callStore.setCallStore({{"One", "Two"},
                            {"Two", "Three"},
                            {"Two", "Four"},
                            {"Three", "Five"},
                            {"Four", "Five"},
                            {"Five", "Six"},
                            {"Six", "Seven"},
                            {"Six", "Eight"},
                            {"Five", "Seven"},
                            {"Five", "Eight"}});

    SECTION("Test getCallerStar") {
        REQUIRE(callStore.getCallerStar("One").empty());
        REQUIRE(callStore.getCallerStar("Two") == std::unordered_set<Procedure>{"One"});
        REQUIRE(callStore.getCallerStar("Three") == std::unordered_set<Procedure>{"Two", "One"});
        REQUIRE(callStore.getCallerStar("Four") == std::unordered_set<Procedure>{"Two", "One"});
        REQUIRE(callStore.getCallerStar("Five") == std::unordered_set<Procedure>{"Four", "Three", "Two", "One"});
        REQUIRE(callStore.getCallerStar("Six") == std::unordered_set<Procedure>{"Five", "Four", "Three", "Two", "One"});
        REQUIRE(callStore.getCallerStar("Seven") ==
                std::unordered_set<Procedure>{"Six", "Three", "Five", "Four", "Two", "One"});
        REQUIRE(callStore.getCallerStar("Eight") ==
                std::unordered_set<Procedure>{"Six", "Three", "Five", "Four", "Two", "One"});
    }

    SECTION("Test getCalleeStar") {
        REQUIRE(callStore.getCalleeStar("One") ==
                std::unordered_set<Procedure>{"Two", "Three", "Six", "Four", "Five", "Seven", "Eight"});
        REQUIRE(callStore.getCalleeStar("Two") ==
                std::unordered_set<Procedure>{"Three", "Six", "Four", "Five", "Seven", "Eight"});
        REQUIRE(callStore.getCalleeStar("Three") == std::unordered_set<Procedure>{"Five", "Seven", "Six", "Eight"});
        REQUIRE(callStore.getCalleeStar("Four") == std::unordered_set<Procedure>{"Five", "Seven", "Six", "Eight"});
        REQUIRE(callStore.getCalleeStar("Five") == std::unordered_set<Procedure>{"Seven", "Six", "Eight"});
        REQUIRE(callStore.getCalleeStar("Six") == std::unordered_set<Procedure>{"Seven", "Eight"});
        REQUIRE(callStore.getCalleeStar("Seven").empty());
        REQUIRE(callStore.getCalleeStar("Eight").empty());
    }

    SECTION("Test hasCallStarRelationship") {
        REQUIRE(callStore.hasCallStarRelationship("One", "Eight"));
        REQUIRE(callStore.hasCallStarRelationship("Two", "Five"));
        REQUIRE(callStore.hasCallStarRelationship("Two", "Seven"));
        REQUIRE(callStore.hasCallStarRelationship("Three", "Six"));
        REQUIRE(callStore.hasCallStarRelationship("Three", "Seven"));

        REQUIRE_FALSE(callStore.hasCallStarRelationship("Eight", "One"));
        REQUIRE_FALSE(callStore.hasCallStarRelationship("Three", "Four"));
        REQUIRE_FALSE(callStore.hasCallStarRelationship("Seven", "Eight"));
        REQUIRE_FALSE(callStore.hasCallStarRelationship("Seven", "Second"));
        REQUIRE_FALSE(callStore.hasCallStarRelationship("Six", "First"));
    }

    SECTION("Test hasCallStarRelationship with Wildcards arg1 and arg2, non-empty nextStore (Expecting True)") {
        ClauseArgument *wildcardArg1 = new Wildcard();
        ClauseArgument *wildcardArg2 = new Wildcard();
        REQUIRE(callStore.hasCallStarRelationship(*wildcardArg1, *wildcardArg2));

        delete wildcardArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasCallStarRelationship with Wildcard arg1 and PROCEDURE arg2 (Expecting True)") {
        ClauseArgument *wildcardArg1 = new Wildcard();
        ClauseArgument *SynonymArg2 = new Synonym(DesignEntityType::PROCEDURE, "Eight");
        REQUIRE(callStore.hasCallStarRelationship(*wildcardArg1, *SynonymArg2));

        delete wildcardArg1;
        delete SynonymArg2;
    }

    SECTION("Test hasCallStarRelationship with Wildcard arg1 and PROCEDURE arg2 (Expecting False)") {
        ClauseArgument *wildcardArg1 = new Wildcard();
        ClauseArgument *SynonymArg2 = new Synonym(DesignEntityType::PROCEDURE, "One");
        REQUIRE_FALSE(callStore.hasCallStarRelationship(*wildcardArg1, *SynonymArg2));

        delete wildcardArg1;
        delete SynonymArg2;
    }

    SECTION("Test hasCallStarRelationship with PROCEDURE arg1 and Wildcard arg2 (Expecting True)") {
        ClauseArgument *SynonymArg1 = new Synonym(DesignEntityType::PROCEDURE, "Four");
        ClauseArgument *wildcardArg2 = new Wildcard();
        REQUIRE(callStore.hasCallStarRelationship(*SynonymArg1, *wildcardArg2));

        delete SynonymArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasCallStarRelationship with PROCEDURE arg1 and Wildcard arg2 (Expecting False)") {
        ClauseArgument *SynonymArg1 = new Synonym(DesignEntityType::PROCEDURE, "Eight");
        ClauseArgument *wildcardArg2 = new Wildcard();
        REQUIRE_FALSE(callStore.hasCallStarRelationship(*SynonymArg1, *wildcardArg2));

        delete SynonymArg1;
        delete wildcardArg2;
    }

    SECTION("Test hasCallStarRelationship with PROCEDURE arg1 and PROCEDURE arg2 (Expecting True)") {
        ClauseArgument *SynonymArg1 = new Synonym(DesignEntityType::PROCEDURE, "One");
        ClauseArgument *SynonymArg2 = new Synonym(DesignEntityType::PROCEDURE, "Eight");
        REQUIRE(callStore.hasCallStarRelationship(*SynonymArg1, *SynonymArg2));

        delete SynonymArg1;
        delete SynonymArg2;
    }

    SECTION("Test hasCallStarRelationship with PROCEDURE arg1 and PROCEDURE arg2 (Expecting False)") {
        ClauseArgument *SynonymArg1 = new Synonym(DesignEntityType::PROCEDURE, "Three");
        ClauseArgument *SynonymArg2 = new Synonym(DesignEntityType::PROCEDURE, "Four");
        REQUIRE_FALSE(callStore.hasCallStarRelationship(*SynonymArg1, *SynonymArg2));

        delete SynonymArg1;
        delete SynonymArg2;
    }
}

TEST_CASE("CallStore - Call Statement Test") {
    CallStore callStore;
    callStore.setCallStmtStore({{"Main", 1}, {"Main", 2}, {"Main", 3}, {"Main", 4}, {"Second", 5}, {"Second", 6}});

    SECTION("Test getStmtNumFromCall") {
        REQUIRE(callStore.getStmtNumFromCall("Main") == std::unordered_set<StmtNum>{1, 2, 3, 4});
        REQUIRE(callStore.getStmtNumFromCall("Second") == std::unordered_set<StmtNum>{5, 6});
    }

    SECTION("Test getCallFromStmtNum") {
        REQUIRE(callStore.getCallFromStmtNum(1) == "Main");
        REQUIRE(callStore.getCallFromStmtNum(6) == "Second");
    }
}
