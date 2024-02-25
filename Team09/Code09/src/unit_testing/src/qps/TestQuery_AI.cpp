#include <algorithm>

#include "PKB/PKB.h"
#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"
#include "qps/evaluator/Query.h"

// ai-gen start(copilot, 2 e)
// prompt: test that Query's evaluate works as expected
TEST_CASE(
    "Evaluate query with select statement with only 1 synonym and no other "
    "clauses") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    pfw.setVariables({"v1", "v2"});
    PKBFacadeReader pkbReader{pkb};

    std::vector<Synonym> selectEntities{{DesignEntityType::VARIABLE, "v"}};
    Query q{selectEntities, {}, {}};
    std::vector<std::string> result = q.evaluate(pkbReader);
    std::sort(result.begin(), result.end());

    std::vector<std::string> expected{"v1", "v2"};
    std::sort(expected.begin(), expected.end());

    REQUIRE(result == expected);
}
// ai-gen end

TEST_CASE(
    "Evaluate query with select statement with only 1 synonym and 1 such that "
    "clause") {
    PKB pkb{};
    PKBFacadeWriter pfw{pkb};
    PKBFacadeReader pkbReader{pkb};

    // ai-gen start(copilot, 1 e)
    // prompt: Select s such that Follows(s, 2)
    SECTION("Select s such that Follows(s, 2)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setFollowsStore(followsStoreEntries);

        std::vector<Synonym> selectEntities{{DesignEntityType::STMT, "s"}};
        std::vector<SuchThatClause> suchThatClauses{
            SuchThatClause{RelationshipType::FOLLOWS, new Synonym{DesignEntityType::STMT, "s"}, new Integer{"2"}}};
        Query q{selectEntities, suchThatClauses, {}};

        std::vector<std::string> result = q.evaluate(pkbReader);
        std::sort(result.begin(), result.end());

        std::vector<std::string> expected{"1"};
        std::sort(expected.begin(), expected.end());

        REQUIRE(result == expected);
    }
    // ai-gen end

    SECTION("Select s such that Follows(s1, 2) / Should not show any results") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setFollowsStore(followsStoreEntries);

        std::vector<Synonym> selectEntities{{DesignEntityType::STMT, "s"}};
        std::vector<SuchThatClause> suchThatClauses{
            SuchThatClause{RelationshipType::FOLLOWS, new Synonym{DesignEntityType::STMT, "s1"}, new Integer{"2"}}};
        Query q{selectEntities, suchThatClauses, {}};

        std::vector<std::string> result = q.evaluate(pkbReader);
        std::sort(result.begin(), result.end());

        std::vector<std::string> expected{};
        std::sort(expected.begin(), expected.end());

        REQUIRE(result == expected);
    }

    // prompt: Select s such that FollowsStar(s, 3)
    SECTION("Select s such that FollowsStar(s, 3)") {
        std::unordered_set<std::pair<int, int>> followsStoreEntries{std::pair<int, int>{1, 2},
                                                                    std::pair<int, int>{2, 3}};
        pfw.setFollowsStore(followsStoreEntries);

        std::vector<Synonym> selectEntities{{DesignEntityType::STMT, "s"}};
        std::vector<SuchThatClause> suchThatClauses{
            SuchThatClause{RelationshipType::FOLLOWS_STAR, new Synonym{DesignEntityType::STMT, "s"}, new Integer{"3"}}};
        Query q{selectEntities, suchThatClauses, {}};

        std::vector<std::string> result = q.evaluate(pkbReader);
        std::sort(result.begin(), result.end());

        std::vector<std::string> expected{"1", "2"};
        std::sort(expected.begin(), expected.end());

        REQUIRE(result == expected);
    }

    // ai-gen start(copilot, 0 e)
    // prompt: Select s such that Parent(s, 2)
    SECTION("Select s such that Parent(s, 2)") {
        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setParentStore(parentStoreEntries);

        std::vector<Synonym> selectEntities{{DesignEntityType::STMT, "s"}};
        std::vector<SuchThatClause> suchThatClauses{
            SuchThatClause{RelationshipType::PARENT, new Synonym{DesignEntityType::STMT, "s"}, new Integer{"2"}}};
        Query q{selectEntities, suchThatClauses, {}};

        std::vector<std::string> result = q.evaluate(pkbReader);
        std::sort(result.begin(), result.end());

        std::vector<std::string> expected{"1"};
        std::sort(expected.begin(), expected.end());

        REQUIRE(result == expected);
    }
    // ai-gen end

    // ai-gen start(copilot, 0, e)
    // prompt: Select s such that Parent(s, 2) / Should not show any results
    SECTION("Select s such that Parent(s1, 2) / Should not show any results") {
        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2}};
        pfw.setParentStore(parentStoreEntries);

        std::vector<Synonym> selectEntities{{DesignEntityType::STMT, "s"}};
        std::vector<SuchThatClause> suchThatClauses{
            SuchThatClause{RelationshipType::PARENT, new Synonym{DesignEntityType::STMT, "s1"}, new Integer{"2"}}};
        Query q{selectEntities, suchThatClauses, {}};

        std::vector<std::string> result = q.evaluate(pkbReader);
        std::sort(result.begin(), result.end());

        std::vector<std::string> expected{};
        std::sort(expected.begin(), expected.end());

        REQUIRE(result == expected);
    }
    // ai-gen end

    // ai-gen start(copilot, 0, e)
    // prompt: Select s such that ParentStar(s, 3)
    SECTION("Select s such that ParentStar(s, 3)") {
        std::unordered_set<std::pair<int, int>> parentStoreEntries{std::pair<int, int>{1, 2},
                                                                   std::pair<int, int>{2, 3}};
        pfw.setParentStore(parentStoreEntries);

        std::vector<Synonym> selectEntities{{DesignEntityType::STMT, "s"}};
        std::vector<SuchThatClause> suchThatClauses{
            SuchThatClause{RelationshipType::PARENT_STAR, new Synonym{DesignEntityType::STMT, "s"}, new Integer{"3"}}};
        Query q{selectEntities, suchThatClauses, {}};

        std::vector<std::string> result = q.evaluate(pkbReader);
        std::sort(result.begin(), result.end());

        std::vector<std::string> expected{"1", "2"};
        std::sort(expected.begin(), expected.end());

        REQUIRE(result == expected);
    }
    // ai-gen end

    // ai-gen start(copilot, 0, e)
    // prompt: Select s such that Uses(s, 'variable_x')
    SECTION("Select s such that Uses(s, 'variable_x')") {
        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{1, "variable_x"}};
        pfw.setUsesStore(usesStoreEntries);

        std::vector<Synonym> selectEntities{{DesignEntityType::STMT, "s"}};
        std::vector<SuchThatClause> suchThatClauses{SuchThatClause{
            RelationshipType::USES, new Synonym{DesignEntityType::STMT, "s"}, new Literal{"variable_x"}}};
        Query q{selectEntities, suchThatClauses, {}};

        std::vector<std::string> result = q.evaluate(pkbReader);
        std::sort(result.begin(), result.end());

        std::vector<std::string> expected{"1"};
        std::sort(expected.begin(), expected.end());

        REQUIRE(result == expected);
    }
    // ai-gen end

    // ai-gen start(copilot, 0, e)
    // prompt: Select s such that Uses(1, v1)
    SECTION("Select s such that Uses(1, v1)") {
        std::unordered_set<std::pair<int, std::string>> usesStoreEntries{std::pair<int, std::string>{1, "variable_x"}};
        pfw.setUsesStore(usesStoreEntries);

        std::vector<Synonym> selectEntities{{DesignEntityType::VARIABLE, "v1"}};
        std::vector<SuchThatClause> suchThatClauses{
            SuchThatClause{RelationshipType::USES, new Integer{"1"}, new Synonym{DesignEntityType::VARIABLE, "v1"}}};
        Query q{selectEntities, suchThatClauses, {}};

        std::vector<std::string> result = q.evaluate(pkbReader);
        std::sort(result.begin(), result.end());

        std::vector<std::string> expected{"variable_x"};
        std::sort(expected.begin(), expected.end());

        REQUIRE(result == expected);
    }
    // ai-gen end

    // ai-gen start(copilot, 0, e)
    // prompt: Select s such that Modifies(s, 'variable_x')
    SECTION("Select s such that Modifies(s, 'variable_x')") {
        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{
            std::pair<int, std::string>{1, "variable_x"}};
        pfw.setModifiesStore(modifiesStoreEntries);

        std::vector<Synonym> selectEntities{{DesignEntityType::STMT, "s"}};
        std::vector<SuchThatClause> suchThatClauses{SuchThatClause{
            RelationshipType::MODIFIES, new Synonym{DesignEntityType::STMT, "s"}, new Literal{"variable_x"}}};
        Query q{selectEntities, suchThatClauses, {}};

        std::vector<std::string> result = q.evaluate(pkbReader);
        std::sort(result.begin(), result.end());

        std::vector<std::string> expected{"1"};
        std::sort(expected.begin(), expected.end());

        REQUIRE(result == expected);
    }
    // ai-gen end

    SECTION("Select s such that Modifies(1, v1)") {
        std::unordered_set<std::pair<int, std::string>> modifiesStoreEntries{
            std::pair<int, std::string>{1, "variable_x"}};
        pfw.setModifiesStore(modifiesStoreEntries);

        std::vector<Synonym> selectEntities{{DesignEntityType::VARIABLE, "v1"}};
        std::vector<SuchThatClause> suchThatClauses{SuchThatClause{RelationshipType::MODIFIES, new Integer{"1"},
                                                                   new Synonym{DesignEntityType::VARIABLE, "v1"}}};
        Query q{selectEntities, suchThatClauses, {}};

        std::vector<std::string> result = q.evaluate(pkbReader);
        std::sort(result.begin(), result.end());

        std::vector<std::string> expected{"variable_x"};
        std::sort(expected.begin(), expected.end());

        REQUIRE(result == expected);
    }
}
