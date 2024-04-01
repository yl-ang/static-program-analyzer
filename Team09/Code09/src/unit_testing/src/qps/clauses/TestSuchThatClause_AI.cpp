#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

// ai-gen start(copilot, 0, e)
// prompt: test suchthat clause equals
TEST_CASE("SuchThatClause equals") {
    SuchThatClause suchThatClause1(RelationshipType::FOLLOWS, std::make_shared<Integer>("1"),
                                   std::make_shared<Integer>("2"));
    SuchThatClause suchThatClause2(RelationshipType::FOLLOWS, std::make_shared<Integer>("1"),
                                   std::make_shared<Integer>("2"));
    SuchThatClause suchThatClause3(RelationshipType::FOLLOWS, std::make_shared<Integer>("1"),
                                   std::make_shared<Integer>("3"));
    SuchThatClause suchThatClause4(RelationshipType::FOLLOWS, std::make_shared<Integer>("2"),
                                   std::make_shared<Integer>("2"));
    SuchThatClause suchThatClause5(RelationshipType::FOLLOWS_STAR, std::make_shared<Integer>("1"),
                                   std::make_shared<Integer>("2"));
    SuchThatClause suchThatClause6(RelationshipType::PARENT, std::make_shared<Integer>("1"),
                                   std::make_shared<Integer>("2"));
    SuchThatClause suchThatClause7(RelationshipType::PARENT_STAR, std::make_shared<Integer>("1"),
                                   std::make_shared<Integer>("2"));
    SuchThatClause suchThatClause8(RelationshipType::USES, std::make_shared<Integer>("1"),
                                   std::make_shared<Integer>("2"));
    SuchThatClause suchThatClause9(RelationshipType::MODIFIES, std::make_shared<Integer>("1"),
                                   std::make_shared<Integer>("2"));
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
    std::shared_ptr<Synonym> assignSyn = std::make_shared<Synonym>(DesignEntityType::ASSIGN, "a");
    std::shared_ptr<Synonym> variableSyn = std::make_shared<Synonym>(DesignEntityType::VARIABLE, "v");
    std::shared_ptr<Wildcard> wildcard = std::make_shared<Wildcard>();
    std::shared_ptr<Literal> literal = std::make_shared<Literal>("1");

    SECTION("Follows") {
        SECTION("No synonyms") {
            SuchThatClause suchThatClause1(RelationshipType::FOLLOWS, literal, wildcard);
            std::vector<Synonym> synonyms = suchThatClause1.getSynonyms();
            REQUIRE(synonyms.empty());
        }

        SECTION("1 synonym") {
            SuchThatClause suchThatClause2(RelationshipType::FOLLOWS, assignSyn, wildcard);
            std::vector<Synonym> synonyms = suchThatClause2.getSynonyms();
            std::vector<Synonym> expected = {*assignSyn};
            REQUIRE(synonyms == expected);
        }

        SECTION("2 synonyms") {
            SuchThatClause suchThatClause3(RelationshipType::FOLLOWS, assignSyn, variableSyn);
            std::vector<Synonym> synonyms = suchThatClause3.getSynonyms();
            std::vector<Synonym> expected = {*assignSyn, *variableSyn};
            REQUIRE(synonyms == expected);
        }
    }
}
// ai-gen end
