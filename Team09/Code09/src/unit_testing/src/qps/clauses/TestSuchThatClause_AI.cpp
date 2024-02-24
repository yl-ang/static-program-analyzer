#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

// ai-gen start(copilot, 0, e)
// prompt: test suchthatclause gettype
TEST_CASE("SuchThatClause getType") {
    SuchThatClause suchThatClause(RelationshipType::FOLLOWS, new Integer("1"), new Integer("2"));
    REQUIRE(suchThatClause.getType() == ClauseType::SUCH_THAT);
}
// ai-gen end

// ai-gen start(copilot, 0, e)
// prompt: test suchthat clause equals
TEST_CASE("SuchThatClause equals") {
    SuchThatClause suchThatClause1(RelationshipType::FOLLOWS, new Integer("1"), new Integer("2"));
    SuchThatClause suchThatClause2(RelationshipType::FOLLOWS, new Integer("1"), new Integer("2"));
    SuchThatClause suchThatClause3(RelationshipType::FOLLOWS, new Integer("1"), new Integer("3"));
    SuchThatClause suchThatClause4(RelationshipType::FOLLOWS, new Integer("2"), new Integer("2"));
    SuchThatClause suchThatClause5(RelationshipType::FOLLOWS_STAR, new Integer("1"), new Integer("2"));
    SuchThatClause suchThatClause6(RelationshipType::PARENT, new Integer("1"), new Integer("2"));
    SuchThatClause suchThatClause7(RelationshipType::PARENT_STAR, new Integer("1"), new Integer("2"));
    SuchThatClause suchThatClause8(RelationshipType::USES, new Integer("1"), new Integer("2"));
    SuchThatClause suchThatClause9(RelationshipType::MODIFIES, new Integer("1"), new Integer("2"));
    REQUIRE(suchThatClause1.equals(suchThatClause2) == true);
    REQUIRE(suchThatClause1.equals(suchThatClause3) == false);
    REQUIRE(suchThatClause1.equals(suchThatClause4) == false);
    REQUIRE(suchThatClause1.equals(suchThatClause5) == false);
    REQUIRE(suchThatClause1.equals(suchThatClause6) == false);
    REQUIRE(suchThatClause1.equals(suchThatClause7) == false);
    REQUIRE(suchThatClause1.equals(suchThatClause8) == false);
    REQUIRE(suchThatClause1.equals(suchThatClause9) == false);
}
// ai-gen end

// ai-gen start(copilot, 0, e)
// prompt: test suchthatclause determineRelationshiptype
TEST_CASE("SuchThatClause determineRelationshipType") {
    REQUIRE(SuchThatClause::determineRelationshipType("Follows") == RelationshipType::FOLLOWS);
    REQUIRE(SuchThatClause::determineRelationshipType("Follows*") == RelationshipType::FOLLOWS_STAR);
    REQUIRE(SuchThatClause::determineRelationshipType("Parent") == RelationshipType::PARENT);
    REQUIRE(SuchThatClause::determineRelationshipType("Parent*") == RelationshipType::PARENT_STAR);
    REQUIRE(SuchThatClause::determineRelationshipType("Uses") == RelationshipType::USES);
    REQUIRE(SuchThatClause::determineRelationshipType("Modifies") == RelationshipType::MODIFIES);
}
// ai-gen end
