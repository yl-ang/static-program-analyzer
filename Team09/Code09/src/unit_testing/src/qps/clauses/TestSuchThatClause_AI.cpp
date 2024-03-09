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
TEST_CASE("Relationship determineRelationshipType") {
    REQUIRE(RelationshipBuilder::determineRelationshipType("Follows") == RelationshipType::FOLLOWS);
    REQUIRE(RelationshipBuilder::determineRelationshipType("Follows*") == RelationshipType::FOLLOWS_STAR);
    REQUIRE(RelationshipBuilder::determineRelationshipType("Parent") == RelationshipType::PARENT);
    REQUIRE(RelationshipBuilder::determineRelationshipType("Parent*") == RelationshipType::PARENT_STAR);
    REQUIRE(RelationshipBuilder::determineRelationshipType("Uses") == RelationshipType::USES);
    REQUIRE(RelationshipBuilder::determineRelationshipType("Modifies") == RelationshipType::MODIFIES);
}
// ai-gen end

// ai-gen start(copilot, 0, e)
// prompt: use copilot
TEST_CASE("Test suchThat getSynonyms") {
    Synonym assignSyn = Synonym(DesignEntityType::ASSIGN, "a");
    Synonym variableSyn = Synonym(DesignEntityType::VARIABLE, "v");
    Wildcard wildcard = Wildcard();
    Literal literal = Literal("1");

    SECTION("Follows") {
        SECTION("No synonyms") {
            SuchThatClause suchThatClause1(RelationshipType::FOLLOWS, &literal, &wildcard);
            std::vector<Synonym> synonyms = suchThatClause1.getSynonyms();
            REQUIRE(synonyms.empty());
        }

        SECTION("1 synonym") {
            SuchThatClause suchThatClause2(RelationshipType::FOLLOWS, &assignSyn, &wildcard);
            std::vector<Synonym> synonyms = suchThatClause2.getSynonyms();
            std::vector<Synonym> expected = {assignSyn};
            REQUIRE(synonyms == expected);
        }

        SECTION("2 synonyms") {
            SuchThatClause suchThatClause3(RelationshipType::FOLLOWS, &assignSyn, &variableSyn);
            std::vector<Synonym> synonyms = suchThatClause3.getSynonyms();
            std::vector<Synonym> expected = {assignSyn, variableSyn};
            REQUIRE(synonyms == expected);
        }
    }
}
// ai-gen end

// ai-gen start(copilot, 0, e)
// prompt: use copilot
TEST_CASE("Test suchThat getSynonyms") {
    Synonym assignSyn = Synonym(DesignEntityType::ASSIGN, "a");
    Synonym variableSyn = Synonym(DesignEntityType::VARIABLE, "v");
    Wildcard wildcard = Wildcard();
    Literal literal = Literal("1");

    SECTION("Follows") {
        SECTION("No synonyms") {
            SuchThatClause suchThatClause1(RelationshipType::FOLLOWS, &literal, &wildcard);
            std::vector<Synonym> synonyms = suchThatClause1.getSynonyms();
            REQUIRE(synonyms.empty());
        }

        SECTION("1 synonym") {
            SuchThatClause suchThatClause2(RelationshipType::FOLLOWS, &assignSyn, &wildcard);
            std::vector<Synonym> synonyms = suchThatClause2.getSynonyms();
            std::vector<Synonym> expected = {assignSyn};
            REQUIRE(synonyms == expected);
        }

        SECTION("2 synonyms") {
            SuchThatClause suchThatClause3(RelationshipType::FOLLOWS, &assignSyn, &variableSyn);
            std::vector<Synonym> synonyms = suchThatClause3.getSynonyms();
            std::vector<Synonym> expected = {assignSyn, variableSyn};
            REQUIRE(synonyms == expected);
        }
    }
}
// ai-gen end
