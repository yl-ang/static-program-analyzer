#include <qps/ParserUtils.h>

#include <vector>

#include "PQLParserTester.h"
#include "catch.hpp"

// parsing entities
TEST_CASE("one_variable") {
    std::vector<std::string> one_variable = {"variable v1;"};
    std::vector<Synonym> expected_one_variable = {Synonym(DesignEntityType::VARIABLE, "v1")};
    SynonymStore synonymStore = PQLParserTester::parseQueryEntities(one_variable);
    REQUIRE(synonymStore.containsSynonym(expected_one_variable[0]));
}
TEST_CASE("two_variables") {
    std::vector<std::string> two_variables = {"variable v1, v2;"};
    std::vector<Synonym> expected_two_variables = {Synonym(DesignEntityType::VARIABLE, "v1"),
                                                   Synonym(DesignEntityType::VARIABLE, "v2")};
    SynonymStore synonymStore = PQLParserTester::parseQueryEntities(two_variables);
    REQUIRE(synonymStore.containsSynonym(expected_two_variables[0]));
    REQUIRE(synonymStore.containsSynonym(expected_two_variables[1]));
}
TEST_CASE("one_call_assign_stmt") {
    std::vector<std::string> one_call_assign_stmt = {"call c1;", "assign a1;", "stmt s1;"};
    std::vector<Synonym> expected_one_call_assign_stmt = {Synonym(DesignEntityType::CALL, "c1"),
                                                          Synonym(DesignEntityType::ASSIGN, "a1"),
                                                          Synonym(DesignEntityType::STMT, "s1")};

    SynonymStore synonymStore = PQLParserTester::parseQueryEntities(one_call_assign_stmt);

    REQUIRE(synonymStore.containsSynonym(expected_one_call_assign_stmt[0]));
    REQUIRE(synonymStore.containsSynonym(expected_one_call_assign_stmt[1]));
    REQUIRE(synonymStore.containsSynonym(expected_one_call_assign_stmt[2]));
}
TEST_CASE("various_call_assign_stmt") {
    std::vector<std::string> various_call_assign_stmt = {"call c1, c2;", "assign a1;", "stmt s1, s2;"};
    std::vector<Synonym> expected_various_call_assign_stmt = {
        Synonym(DesignEntityType::CALL, "c1"), Synonym(DesignEntityType::CALL, "c2"),
        Synonym(DesignEntityType::ASSIGN, "a1"), Synonym(DesignEntityType::STMT, "s1"),
        Synonym(DesignEntityType::STMT, "s2")};
    SynonymStore synonymStore = PQLParserTester::parseQueryEntities(various_call_assign_stmt);
    REQUIRE(synonymStore.containsSynonym(expected_various_call_assign_stmt[0]));
    REQUIRE(synonymStore.containsSynonym(expected_various_call_assign_stmt[1]));
    REQUIRE(synonymStore.containsSynonym(expected_various_call_assign_stmt[2]));
    REQUIRE(synonymStore.containsSynonym(expected_various_call_assign_stmt[3]));
    REQUIRE(synonymStore.containsSynonym(expected_various_call_assign_stmt[4]));
}

// Testing Find Queries functions
std::vector<std::string> various_assorted = {"variable v1, v2;", "assign a1;", "stmt s1, s2;", "procedure p1;",
                                             "constant c1;"};
SynonymStore entities = PQLParserTester::parseQueryEntities(various_assorted);

using containerPtr = std::shared_ptr<SelectEntContainer>;

// Test cases
TEST_CASE("PQLParserTester: Select (1)") {
    std::string select_1 = "Select v1";
    std::string select_2 = "Select a1";
    std::string select_3 = "Select s1";

    containerPtr result_1 = PQLParserTester::parseSelectClause(select_1);
    containerPtr result_2 = PQLParserTester::parseSelectClause(select_2);
    containerPtr result_3 = PQLParserTester::parseSelectClause(select_3);

    result_1->updateSynonyms(&entities);
    result_2->updateSynonyms(&entities);
    result_3->updateSynonyms(&entities);

    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v1") == result_1->getSynonyms()[0]);
    REQUIRE(result_1->getSynonyms().size() == 1);
    REQUIRE(Synonym(DesignEntityType::ASSIGN, "a1") == result_2->getSynonyms()[0]);
    REQUIRE(result_2->getSynonyms().size() == 1);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_3->getSynonyms()[0]);
    REQUIRE(result_3->getSynonyms().size() == 1);
}

TEST_CASE("PQLParserTester: Select (2))") {
    std::string select_1 = "Select <v1,v2>";
    std::string select_2 = "  Select    <v1,    v2>  ";

    containerPtr result_1 = PQLParserTester::parseSelectClause(select_1);
    containerPtr result_2 = PQLParserTester::parseSelectClause(select_2);

    result_1->updateSynonyms(&entities);
    result_2->updateSynonyms(&entities);

    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v1") == result_1->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v2") == result_1->getSynonyms()[1]);
    REQUIRE(result_1->getSynonyms().size() == 2);
    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v1") == result_2->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v2") == result_2->getSynonyms()[1]);
    REQUIRE(result_2->getSynonyms().size() == 2);
}

TEST_CASE("PQLParserTester: Select (3))") {
    std::string select_1 = "Select BOOLEAN";
    std::string select_2 = "  Select    BOOLEAN  ";

    containerPtr result_1 = PQLParserTester::parseSelectClause(select_1);
    containerPtr result_2 = PQLParserTester::parseSelectClause(select_2);

    result_1->updateSynonyms(&entities);
    result_2->updateSynonyms(&entities);

    REQUIRE(result_1->getSynonyms().size() == 0);
    REQUIRE(result_2->getSynonyms().size() == 0);
}

TEST_CASE("PQLParserTest: Select (4)") {
    std::string select_1 = "Select <v1.varName, a1.stmt#>";
    std::string select_2 = "Select <p1.procName, c1.value>";

    containerPtr result_1 = PQLParserTester::parseSelectClause(select_1);
    containerPtr result_2 = PQLParserTester::parseSelectClause(select_2);

    result_1->updateSynonyms(&entities);
    result_2->updateSynonyms(&entities);

    REQUIRE(result_1->getSynonyms().size() == 2);
    REQUIRE(result_1->getSynonyms()[0].getAttr().has_value());
    REQUIRE(result_1->getSynonyms()[1].getAttr().has_value());
    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v1", SynonymAttributeType::VARNAME) == result_1->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::ASSIGN, "a1", SynonymAttributeType::STMTNUM) == result_1->getSynonyms()[1]);

    REQUIRE(result_2->getSynonyms().size() == 2);
    REQUIRE(result_2->getSynonyms()[0].getAttr().has_value());
    REQUIRE(result_2->getSynonyms()[1].getAttr().has_value());
    REQUIRE(Synonym(DesignEntityType::PROCEDURE, "p1", SynonymAttributeType::PROCNAME) == result_2->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::CONSTANT, "c1", SynonymAttributeType::VALUE) == result_2->getSynonyms()[1]);
}

TEST_CASE("PQLParserTest: Select updateSyonym returns correct boolean") {
    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select v1.value")->updateSynonyms(&entities));
    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select v1.procName")->updateSynonyms(&entities));

    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select s1.varName")->updateSynonyms(&entities));
    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select s1.procName")->updateSynonyms(&entities));
    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select s1.value")->updateSynonyms(&entities));

    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select a1.varName")->updateSynonyms(&entities));
    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select a1.procName")->updateSynonyms(&entities));
    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select a1.value")->updateSynonyms(&entities));

    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select c1.procName")->updateSynonyms(&entities));
    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select c1.varName")->updateSynonyms(&entities));
    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select c1.stmt#")->updateSynonyms(&entities));

    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select p1.varName")->updateSynonyms(&entities));
    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select p1.stmt#")->updateSynonyms(&entities));
    REQUIRE_FALSE(PQLParserTester::parseSelectClause("Select p1.value")->updateSynonyms(&entities));
}

// Select such that Parent()
// Used for testing Parent/Follows/Parent*/Follows*
// As syntax is the same for all these
// To ensure that Follows/Parent*/Follows* work, will test one clause each minimally
TEST_CASE("PQLParserTester: Parent (1)") {
    std::string select_st_1 = "Select s1 such that Parent(s1,s2)";
    std::string select_st_2 = "Select s1 such that Parent(s1,_)";
    std::string select_st_3 = "Select s1 such that Parent(s1,4)";
    std::vector<std::string> clauseList_1 = getAllClauses(select_st_1);
    std::vector<std::string> clauseList_2 = getAllClauses(select_st_2);
    std::vector<std::string> clauseList_3 = getAllClauses(select_st_3);

    auto parsedClauses_1 = PQLParserTester::parseClauses(clauseList_1);
    auto parsedClauses_2 = PQLParserTester::parseClauses(clauseList_2);
    auto parsedClauses_3 = PQLParserTester::parseClauses(clauseList_3);

    containerPtr result_01 = std::get<0>(parsedClauses_1);
    containerPtr result_02 = std::get<0>(parsedClauses_2);
    containerPtr result_03 = std::get<0>(parsedClauses_3);

    std::vector<SuchThatClause> result_11 = std::get<1>(parsedClauses_1);
    std::vector<SuchThatClause> result_12 = std::get<1>(parsedClauses_2);
    std::vector<SuchThatClause> result_13 = std::get<1>(parsedClauses_3);

    std::vector<PatternClause> result_21 = std::get<2>(parsedClauses_1);
    std::vector<PatternClause> result_22 = std::get<2>(parsedClauses_2);
    std::vector<PatternClause> result_23 = std::get<2>(parsedClauses_3);

    Validator::validateClauses(&entities, result_01, result_11, result_21);
    Validator::validateClauses(&entities, result_02, result_12, result_22);
    Validator::validateClauses(&entities, result_03, result_13, result_23);

    // Select checking
    REQUIRE(result_01->getSynonyms().size() == 1);
    REQUIRE(result_02->getSynonyms().size() == 1);
    REQUIRE(result_03->getSynonyms().size() == 1);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_01->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_02->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_03->getSynonyms()[0]);

    // Such that checking
    Synonym s1 = Synonym(DesignEntityType::STMT, "s1");
    Synonym s2 = Synonym(DesignEntityType::STMT, "s2");
    Wildcard w = Wildcard();
    Integer i = Integer("4");
    SuchThatClause ans1 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&s1), static_cast<ClauseArgument*>(&s2));
    SuchThatClause ans2 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&s1), static_cast<ClauseArgument*>(&w));
    SuchThatClause ans3 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&s1), static_cast<ClauseArgument*>(&i));
    REQUIRE(result_11.size() == 1);
    REQUIRE(result_12.size() == 1);
    REQUIRE(result_13.size() == 1);
    REQUIRE(ans1.equals(result_11[0]));
    REQUIRE(ans2.equals(result_12[0]));
    REQUIRE(ans3.equals(result_13[0]));

    // Pattern checking
    REQUIRE(result_21.size() == 0);
    REQUIRE(result_22.size() == 0);
    REQUIRE(result_23.size() == 0);
}

TEST_CASE("PQLParserTester: Parent (2)") {
    std::string select_st_4 = "Select s1 such that Parent(s2,_)";
    std::string select_st_5 = "Select s1 such that Parent(s2,4)";
    std::string select_st_6 = "Select s1 such that Parent(s2,s1)";
    std::vector<std::string> clauseList_4 = getAllClauses(select_st_4);
    std::vector<std::string> clauseList_5 = getAllClauses(select_st_5);
    std::vector<std::string> clauseList_6 = getAllClauses(select_st_6);

    auto parsedClauses_1 = PQLParserTester::parseClauses(clauseList_4);
    auto parsedClauses_2 = PQLParserTester::parseClauses(clauseList_5);
    auto parsedClauses_3 = PQLParserTester::parseClauses(clauseList_6);

    containerPtr result_01 = std::get<0>(parsedClauses_1);
    containerPtr result_02 = std::get<0>(parsedClauses_2);
    containerPtr result_03 = std::get<0>(parsedClauses_3);

    std::vector<SuchThatClause> result_11 = std::get<1>(parsedClauses_1);
    std::vector<SuchThatClause> result_12 = std::get<1>(parsedClauses_2);
    std::vector<SuchThatClause> result_13 = std::get<1>(parsedClauses_3);

    std::vector<PatternClause> result_21 = std::get<2>(parsedClauses_1);
    std::vector<PatternClause> result_22 = std::get<2>(parsedClauses_2);
    std::vector<PatternClause> result_23 = std::get<2>(parsedClauses_3);

    Validator::validateClauses(&entities, result_01, result_11, result_21);
    Validator::validateClauses(&entities, result_02, result_12, result_22);
    Validator::validateClauses(&entities, result_03, result_13, result_23);

    // Select checking
    REQUIRE(result_01->getSynonyms().size() == 1);
    REQUIRE(result_02->getSynonyms().size() == 1);
    REQUIRE(result_03->getSynonyms().size() == 1);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_01->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_02->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_03->getSynonyms()[0]);

    // Such that checking
    Synonym s1 = Synonym(DesignEntityType::STMT, "s1");
    Synonym s2 = Synonym(DesignEntityType::STMT, "s2");
    Wildcard w = Wildcard();
    Integer i = Integer("4");
    SuchThatClause ans1 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&s2), static_cast<ClauseArgument*>(&w));
    SuchThatClause ans2 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&s2), static_cast<ClauseArgument*>(&i));
    SuchThatClause ans3 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&s2), static_cast<ClauseArgument*>(&s1));
    REQUIRE(result_11.size() == 1);
    REQUIRE(result_12.size() == 1);
    REQUIRE(result_13.size() == 1);
    REQUIRE(ans1.equals(result_11[0]));
    REQUIRE(ans2.equals(result_12[0]));
    REQUIRE(ans3.equals(result_13[0]));

    // Pattern checking
    REQUIRE(result_21.size() == 0);
    REQUIRE(result_22.size() == 0);
    REQUIRE(result_23.size() == 0);
}

TEST_CASE("PQLParserTester: Parent (3)") {
    std::string select_st_7 = "Select s1 such that Parent(_,s1)";
    std::string select_st_8 = "Select s1 such that Parent(4,s1)";
    std::string select_st_9 = "Select s1 such that Parent(_,s2)";

    std::vector<std::string> clauseList_7 = getAllClauses(select_st_7);
    std::vector<std::string> clauseList_8 = getAllClauses(select_st_8);
    std::vector<std::string> clauseList_9 = getAllClauses(select_st_9);

    auto parsedClauses_1 = PQLParserTester::parseClauses(clauseList_7);
    auto parsedClauses_2 = PQLParserTester::parseClauses(clauseList_8);
    auto parsedClauses_3 = PQLParserTester::parseClauses(clauseList_9);

    containerPtr result_01 = std::get<0>(parsedClauses_1);
    containerPtr result_02 = std::get<0>(parsedClauses_2);
    containerPtr result_03 = std::get<0>(parsedClauses_3);

    std::vector<SuchThatClause> result_11 = std::get<1>(parsedClauses_1);
    std::vector<SuchThatClause> result_12 = std::get<1>(parsedClauses_2);
    std::vector<SuchThatClause> result_13 = std::get<1>(parsedClauses_3);

    std::vector<PatternClause> result_21 = std::get<2>(parsedClauses_1);
    std::vector<PatternClause> result_22 = std::get<2>(parsedClauses_2);
    std::vector<PatternClause> result_23 = std::get<2>(parsedClauses_3);

    Validator::validateClauses(&entities, result_01, result_11, result_21);
    Validator::validateClauses(&entities, result_02, result_12, result_22);
    Validator::validateClauses(&entities, result_03, result_13, result_23);

    // Select checking
    REQUIRE(result_01->getSynonyms().size() == 1);
    REQUIRE(result_02->getSynonyms().size() == 1);
    REQUIRE(result_03->getSynonyms().size() == 1);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_01->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_02->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_03->getSynonyms()[0]);

    // Such that checking
    Synonym s1 = Synonym(DesignEntityType::STMT, "s1");
    Synonym s2 = Synonym(DesignEntityType::STMT, "s2");
    Wildcard w = Wildcard();
    Integer i = Integer("4");
    SuchThatClause ans1 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&w), static_cast<ClauseArgument*>(&s1));
    SuchThatClause ans2 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&i), static_cast<ClauseArgument*>(&s1));
    SuchThatClause ans3 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&w), static_cast<ClauseArgument*>(&s2));
    SuchThatClause ans4 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&i), static_cast<ClauseArgument*>(&s2));
    REQUIRE(result_11.size() == 1);
    REQUIRE(result_12.size() == 1);
    REQUIRE(result_13.size() == 1);
    REQUIRE(ans1.equals(result_11[0]));
    REQUIRE(ans2.equals(result_12[0]));
    REQUIRE(ans3.equals(result_13[0]));

    // Pattern checking
    REQUIRE(result_21.size() == 0);
    REQUIRE(result_22.size() == 0);
    REQUIRE(result_23.size() == 0);
}

// Spacing and other relationships
TEST_CASE("PQLParserTester: Parent (4)") {
    std::string select_st_11 = "   Select   s1   such  that  Parent (s1 ,  s2 ) ";
    std::string select_st_12 = "Select s1 such that Parent*(s1,s2)";
    std::string select_st_13 = "Select s1 such that Follows(s1,s2)";
    std::string select_st_14 = "Select s1 such that Follows*(s1,s2)";
    std::vector<std::string> clauseList_11 = getAllClauses(select_st_11);
    std::vector<std::string> clauseList_12 = getAllClauses(select_st_12);
    std::vector<std::string> clauseList_13 = getAllClauses(select_st_13);
    std::vector<std::string> clauseList_14 = getAllClauses(select_st_14);

    auto parsedClauses_1 = PQLParserTester::parseClauses(clauseList_11);
    auto parsedClauses_2 = PQLParserTester::parseClauses(clauseList_12);
    auto parsedClauses_3 = PQLParserTester::parseClauses(clauseList_13);
    auto parsedClauses_4 = PQLParserTester::parseClauses(clauseList_14);

    containerPtr result_01 = std::get<0>(parsedClauses_1);
    containerPtr result_02 = std::get<0>(parsedClauses_2);
    containerPtr result_03 = std::get<0>(parsedClauses_3);
    containerPtr result_04 = std::get<0>(parsedClauses_4);

    std::vector<SuchThatClause> result_11 = std::get<1>(parsedClauses_1);
    std::vector<SuchThatClause> result_12 = std::get<1>(parsedClauses_2);
    std::vector<SuchThatClause> result_13 = std::get<1>(parsedClauses_3);
    std::vector<SuchThatClause> result_14 = std::get<1>(parsedClauses_4);

    std::vector<PatternClause> result_21 = std::get<2>(parsedClauses_1);
    std::vector<PatternClause> result_22 = std::get<2>(parsedClauses_2);
    std::vector<PatternClause> result_23 = std::get<2>(parsedClauses_3);
    std::vector<PatternClause> result_24 = std::get<2>(parsedClauses_4);

    Validator::validateClauses(&entities, result_01, result_11, result_21);
    Validator::validateClauses(&entities, result_02, result_12, result_22);
    Validator::validateClauses(&entities, result_03, result_13, result_23);
    Validator::validateClauses(&entities, result_04, result_14, result_24);

    // Select checking
    REQUIRE(result_01->getSynonyms().size() == 1);
    REQUIRE(result_02->getSynonyms().size() == 1);
    REQUIRE(result_03->getSynonyms().size() == 1);
    REQUIRE(result_04->getSynonyms().size() == 1);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_01->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_02->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_03->getSynonyms()[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_04->getSynonyms()[0]);

    // Such that checking
    Synonym s1 = Synonym(DesignEntityType::STMT, "s1");
    Synonym s2 = Synonym(DesignEntityType::STMT, "s2");
    SuchThatClause ans1 =
        SuchThatClause(RelationshipType::PARENT, static_cast<ClauseArgument*>(&s1), static_cast<ClauseArgument*>(&s2));
    SuchThatClause ans2 = SuchThatClause(RelationshipType::PARENT_STAR, static_cast<ClauseArgument*>(&s1),
                                         static_cast<ClauseArgument*>(&s2));
    SuchThatClause ans3 =
        SuchThatClause(RelationshipType::FOLLOWS, static_cast<ClauseArgument*>(&s1), static_cast<ClauseArgument*>(&s2));
    SuchThatClause ans4 = SuchThatClause(RelationshipType::FOLLOWS_STAR, static_cast<ClauseArgument*>(&s1),
                                         static_cast<ClauseArgument*>(&s2));
    REQUIRE(result_11.size() == 1);
    REQUIRE(result_12.size() == 1);
    REQUIRE(result_13.size() == 1);
    REQUIRE(result_14.size() == 1);
    REQUIRE(ans1.equals(result_11[0]));
    REQUIRE(ans2.equals(result_12[0]));
    REQUIRE(ans3.equals(result_13[0]));
    REQUIRE(ans4.equals(result_14[0]));

    // Pattern checking
    REQUIRE(result_21.size() == 0);
    REQUIRE(result_22.size() == 0);
    REQUIRE(result_23.size() == 0);
    REQUIRE(result_24.size() == 0);
}

// Uses/Modifies
/**
 * UsesS : 'Uses' '(' stmtRef ',' entRef ')'
 * UsesP : 'Uses' '(' entRef ',' entRef ')'
 * ModifiesS : 'Modifies' '(' stmtRef ',' entRef ')'
 * ModifiesP : 'Modifies' '(' entRef ',' entRef ')'
 */
// Actually parser should be same as above - SKIP THESE

// Pattern
// pattern-cl : 'pattern' syn-assign '(' entRef ',' expression-spec ')'
// expression-spec :  '"' expr'"' | '_' '"' expr '"' '_' | '_'
// entRef : synonym | '_' | '"' IDENT '"'
TEST_CASE("PQLParserTester: Select ... pattern (1)") {
    std::string select_pt_1 = "Select v1 pattern a1(v1,\"x+y\")";
    std::string select_pt_2 = "Select v1 pattern a1(v1,\"x + y\")";
    std::string select_pt_3 = "Select v1 pattern a1(v1,\"x +y\")";
    std::string select_pt_4 = "Select v1 pattern a1(v1,\" x +y \")";
    std::vector<std::string> clauseList_1 = getAllClauses(select_pt_1);
    std::vector<std::string> clauseList_2 = getAllClauses(select_pt_2);
    std::vector<std::string> clauseList_3 = getAllClauses(select_pt_3);
    std::vector<std::string> clauseList_4 = getAllClauses(select_pt_4);

    auto parsedClauses_1 = PQLParserTester::parseClauses(clauseList_1);
    auto parsedClauses_2 = PQLParserTester::parseClauses(clauseList_2);
    auto parsedClauses_3 = PQLParserTester::parseClauses(clauseList_3);
    auto parsedClauses_4 = PQLParserTester::parseClauses(clauseList_4);

    // ignore
    containerPtr result_01 = std::get<0>(parsedClauses_1);
    containerPtr result_02 = std::get<0>(parsedClauses_2);
    containerPtr result_03 = std::get<0>(parsedClauses_3);
    containerPtr result_04 = std::get<0>(parsedClauses_4);

    // ignore
    std::vector<SuchThatClause> result_11 = std::get<1>(parsedClauses_1);
    std::vector<SuchThatClause> result_12 = std::get<1>(parsedClauses_2);
    std::vector<SuchThatClause> result_13 = std::get<1>(parsedClauses_3);
    std::vector<SuchThatClause> result_14 = std::get<1>(parsedClauses_4);

    std::vector<PatternClause> result_21 = std::get<2>(parsedClauses_1);
    std::vector<PatternClause> result_22 = std::get<2>(parsedClauses_2);
    std::vector<PatternClause> result_23 = std::get<2>(parsedClauses_3);
    std::vector<PatternClause> result_24 = std::get<2>(parsedClauses_4);

    Validator::validateClauses(&entities, result_01, result_11, result_21);
    Validator::validateClauses(&entities, result_02, result_12, result_22);
    Validator::validateClauses(&entities, result_03, result_13, result_23);
    Validator::validateClauses(&entities, result_04, result_14, result_24);

    Synonym a1 = Synonym(DesignEntityType::ASSIGN, "a1");
    Synonym v1 = Synonym(DesignEntityType::VARIABLE, "v1");
    ExpressionSpec l = ExpressionSpec("\"x+y\"");
    PatternClause ans1 = PatternClause(static_cast<ClauseArgument*>(&a1), static_cast<ClauseArgument*>(&v1),
                                       static_cast<ClauseArgument*>(&l));
    REQUIRE(result_21.size() == 1);
    REQUIRE(result_22.size() == 1);
    REQUIRE(result_23.size() == 1);
    REQUIRE(result_24.size() == 1);
    REQUIRE(ans1.equals(result_21[0]));
    REQUIRE(ans1.equals(result_22[0]));
    REQUIRE(ans1.equals(result_23[0]));
    REQUIRE(ans1.equals(result_24[0]));
}

TEST_CASE("PQLParserTester: Select ... pattern (2)") {
    std::string select_pt_1 = "Select v1 pattern a1(v1,_\"x+y\"_)";
    std::string select_pt_2 = "Select v1 pattern a1(v1,_\"x + y\"_)";
    std::string select_pt_3 = "Select v1 pattern a1(v1,_\"x +y\"_)";
    std::string select_pt_4 = "Select v1 pattern a1(v1,_\" x +y \"_)";
    std::vector<std::string> clauseList_1 = getAllClauses(select_pt_1);
    std::vector<std::string> clauseList_2 = getAllClauses(select_pt_2);
    std::vector<std::string> clauseList_3 = getAllClauses(select_pt_3);
    std::vector<std::string> clauseList_4 = getAllClauses(select_pt_4);

    auto parsedClauses_1 = PQLParserTester::parseClauses(clauseList_1);
    auto parsedClauses_2 = PQLParserTester::parseClauses(clauseList_2);
    auto parsedClauses_3 = PQLParserTester::parseClauses(clauseList_3);
    auto parsedClauses_4 = PQLParserTester::parseClauses(clauseList_4);

    // ignore
    containerPtr result_01 = std::get<0>(parsedClauses_1);
    containerPtr result_02 = std::get<0>(parsedClauses_2);
    containerPtr result_03 = std::get<0>(parsedClauses_3);
    containerPtr result_04 = std::get<0>(parsedClauses_4);

    // ignore
    std::vector<SuchThatClause> result_11 = std::get<1>(parsedClauses_1);
    std::vector<SuchThatClause> result_12 = std::get<1>(parsedClauses_2);
    std::vector<SuchThatClause> result_13 = std::get<1>(parsedClauses_3);
    std::vector<SuchThatClause> result_14 = std::get<1>(parsedClauses_4);

    std::vector<PatternClause> result_21 = std::get<2>(parsedClauses_1);
    std::vector<PatternClause> result_22 = std::get<2>(parsedClauses_2);
    std::vector<PatternClause> result_23 = std::get<2>(parsedClauses_3);
    std::vector<PatternClause> result_24 = std::get<2>(parsedClauses_4);

    Validator::validateClauses(&entities, result_01, result_11, result_21);
    Validator::validateClauses(&entities, result_02, result_12, result_22);
    Validator::validateClauses(&entities, result_03, result_13, result_23);
    Validator::validateClauses(&entities, result_04, result_14, result_24);

    Synonym a1 = Synonym(DesignEntityType::ASSIGN, "a1");
    Synonym v1 = Synonym(DesignEntityType::VARIABLE, "v1");
    ExpressionSpec l = ExpressionSpec("_\"x+y\"_");
    PatternClause ans1 = PatternClause(static_cast<ClauseArgument*>(&a1), static_cast<ClauseArgument*>(&v1),
                                       static_cast<ClauseArgument*>(&l));
    REQUIRE(result_21.size() == 1);
    REQUIRE(result_22.size() == 1);
    REQUIRE(result_23.size() == 1);
    REQUIRE(result_24.size() == 1);
    REQUIRE(ans1.equals(result_21[0]));
    REQUIRE(ans1.equals(result_22[0]));
    REQUIRE(ans1.equals(result_23[0]));
    REQUIRE(ans1.equals(result_24[0]));
}

TEST_CASE("PQLParserTester: Select ... pattern (4)") {
    std::string select_pt_1 = "Select a such that Uses (a1, v) pattern a (v, _\"temp\"_)";

    // Testing Find Queries functions
    std::vector<std::string> various_assorted_1 = {"assign a, a1;", "variable v;"};
    SynonymStore entities = PQLParserTester::parseQueryEntities(various_assorted_1);
    std::vector<std::string> clauseList_1 = getAllClauses(select_pt_1);
    auto parsedClauses_1 = PQLParserTester::parseClauses(clauseList_1);

    // ignore one line below
    containerPtr result_01 = std::get<0>(parsedClauses_1);
    std::vector<SuchThatClause> result_11 = std::get<1>(parsedClauses_1);
    std::vector<PatternClause> result_21 = std::get<2>(parsedClauses_1);
    Validator::validateClauses(&entities, result_01, result_11, result_21);

    Synonym a = Synonym(DesignEntityType::ASSIGN, "a");
    Synonym a1 = Synonym(DesignEntityType::ASSIGN, "a1");
    Synonym v = Synonym(DesignEntityType::VARIABLE, "v");
    ExpressionSpec temp = ExpressionSpec("_\"temp\"_");

    SuchThatClause s =
        SuchThatClause(RelationshipType::USES, static_cast<ClauseArgument*>(&a1), static_cast<ClauseArgument*>(&v));
    PatternClause p = PatternClause(static_cast<ClauseArgument*>(&a), static_cast<ClauseArgument*>(&v),
                                    static_cast<ClauseArgument*>(&temp));
    REQUIRE(result_11.size() == 1);
    REQUIRE(result_21.size() == 1);
    REQUIRE(s.equals(result_11[0]));
    REQUIRE(p.equals(result_21[0]));
}

TEST_CASE("And (1)") {
    std::vector<std::string> clauseList_1 = {"Select v1", "such that Modifies(s1, s2)", "and Modifies(s2, s1)"};
    PQLParserTester::modifyClauseList(clauseList_1);
    REQUIRE(clauseList_1[0] == "Select v1");
    REQUIRE(clauseList_1[1] == "such that Modifies(s1, s2)");
    REQUIRE(clauseList_1[2] == "such that Modifies(s2, s1)");
}

TEST_CASE("And (2)") {
    std::vector<std::string> clauseList_1 = {"Select and", "such that Modifies( and , _)", "and Parent(s, s1)"};
    PQLParserTester::modifyClauseList(clauseList_1);
    REQUIRE(clauseList_1[0] == "Select and");
    REQUIRE(clauseList_1[1] == "such that Modifies( and , _)");
    REQUIRE(clauseList_1[2] == "such that Parent(s, s1)");
}

TEST_CASE("And (3)") {
    std::string inputString_And =
        "Select <and, s1> such that Follows( and , 2) and Parent(s1, s2) pattern a(a, \"expr+x)\") and Modifies(v, )";
    std::vector<std::string> result = getAllClauses(inputString_And);
    std::vector<std::string> result_2 = getAllClauses(inputString_And);
}

#define REQUIRE_THROW_SYNTAX_ERROR(expr) REQUIRE_THROWS_AS(expr, QPSSyntaxError)
#define REQUIRE_THROW_SEMANTIC_ERROR(expr) REQUIRE_THROWS_AS(expr, QPSSemanticError)
using vectorString = std::vector<std::string>;

TEST_CASE("SyntaxError") {
    SECTION("No Select") {
        vectorString input = {"stmt s;", "variable v;", ""};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Missing synonym name") {
        vectorString input = {"stmt ;", "variable v;", "Select s"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Declaration not synonym") {
        vectorString input = {"stmt 1s;", "variable v;", "Select s such that Follows* (1, 2)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Two Selects, one with semi-colon") {
        vectorString input = {"stmt s;", "Select s;", "Select s such that Follows* (1, 2)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Two Selects, combine in one statement") {
        vectorString input = {"stmt s;", "Select s Select s such that Follows* (1, 2)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Tuple return, missing open angular bracket") {
        vectorString input = {"stmt s;", "Select s> such that Follows* (1, 2)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Tuple return, missing close angular bracket") {
        vectorString input = {"stmt s;", "Select <s such that Follows* (1, 2)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Tuple return, extra comma") {
        vectorString input = {"stmt s;", "Select <,s> such that Follows* (1, 2)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Follow all lower case") {
        vectorString input = {"stmt s;", "variable v;", "Select s such that follows(1, 2)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("RelRef<space>*") {
        vectorString input = {"stmt s;", "variable v;", "Select s such that Follows * (1, 2)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Next, with ident") {
        vectorString input = {"stmt s;", "variable v;", "Select s such that Next(\"x\", 2)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Modifies, with two wildcards") {
        vectorString input = {"stmt s;", "variable v;", "Select s such that Modifies(__, _)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Uses, missing value") {
        vectorString input = {"stmt s;", "variable v;", "Select s such that Uses(s,)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Uses, one synonym") {
        vectorString input = {"stmt s;", "variable v;", "Select s such that Uses(s)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Pattern, Capitalized pattern") {
        vectorString input = {"assign a;", "Select a such that Pattern a (_,_)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Pattern, Messy and wrong expression") {
        vectorString input = {"assign a;", "Select a such that pattern a (_, \"(x + 1))/ 2)\")"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Pattern, Missing value") {
        vectorString input = {"assign a;", "Select a such that pattern a (_,)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Pattern, Missing front underscore") {
        vectorString input = {"assign a;", "Select a such that pattern a (_, _\"x\")"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Pattern, Missing back underscore") {
        vectorString input = {"assign a;", "Select a such that pattern a (_, \"x\"_)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Pattern, Missing synonym") {
        vectorString input = {"assign a;", "Select a such that pattern (_, \"x\"_)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Missing relCond after such that") {
        vectorString input = {"assign a;", "stmt s1;", "Select s1 such that pattern a (s1, _)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Missing patternCond after pattern") {
        vectorString input = {"assign a;", "stmt s1;", "Select s1 pattern"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Misspelt and, such that") {
        vectorString input = {"assign a,a1;", "stmt s1;", "Select s1 such that Modifies (s1, _) amd Uses (s1,_)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Misspelt and, pattern") {
        vectorString input = {"assign a,a1;", "stmt s1;", "Select s1 pattern a (_, _) amd a1(_,_)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Missing relCond after and, such that") {
        vectorString input = {"assign a,a1;", "stmt s1;", "Select s1 such that Modifies (s1, _) and "};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Missing relCond after and, pattern") {
        vectorString input = {"assign a,a1;", "stmt s1;", "Select s1 pattern a (_, _) and "};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("patternCond after such that") {
        vectorString input = {"assign a,a1;", "stmt s1;", "Select s1 such that a (_, _)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("relCond after and, such that") {
        vectorString input = {"assign a,a1;", "stmt s1;", "Select s1 such that Modifies (s1, _) and a (_, _)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }

    SECTION("and as a synonym, pattern") {
        vectorString input = {"assign and, a;", "stmt s1;", "Select s1 pattern and (_, _) and a (_, _)"};
        REQUIRE_NOTHROW(PQLParserTester::parse(input));
    }

    SECTION("Syntax Error before undeclared relCond synonym after and, pattern") {
        vectorString input = {"assign a,a1;", "stmt s1;", "Select s1 pattern a (, _) and Modifies (s1, _)"};
        REQUIRE_THROW_SYNTAX_ERROR(PQLParserTester::parse(input));
    }
}

TEST_CASE("SemanticError") {
    SECTION("No declarations") {
        vectorString input = {"Select s such that Parent(1,2)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Missing declaration") {
        vectorString input = {"stmt s1;", "Select s such that Parent(1,2)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Missing BOOLEAN declaration") {
        vectorString input = {"stmt s1;", "Select <BOOLEAN> such that Parent(1,2)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Missing BOOLEAN declaration(2)") {
        vectorString input = {"stmt s1;", "Select <s1, BOOLEAN> such that Parent(1,2)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Follow, first arg not subtype of statement") {
        vectorString input = {"stmt s1;", "variable v;", "Select s1 such that Follows(v, 2)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Parent, second arg not subtype of statement") {
        vectorString input = {"variable s1;", "Select s1 such that Parent(1,s1)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Modifies, second arg not variable") {
        vectorString input = {"stmt s1;", "Select s1 such that Modifies(1,s1)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Modifies, first arg is wildcard") {
        vectorString input = {"variable s1;", "Select s1 such that Modifies(_,s1)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Uses, second arg not variable") {
        vectorString input = {"stmt s1;", "Select s1 such that Uses(1,s1)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Uses, first arg is wildcard") {
        vectorString input = {"variable s1;", "Select s1 such that Uses(_,s1)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    // SECTION("Calls, first arg not procedure") {
    //     vectorString input = {"variable s1;", "Select s1 such that Calls(s1,_)"};
    //     REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    // }

    // SECTION("Calls, second arg not procedure") {
    //     vectorString input = {"variable s1;", "Select s1 such that Calls(_,s1)"};
    //     REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    // }

    SECTION("Pattern, first arg not variable") {
        vectorString input = {"assign a;", "stmt s1;", "Select s1 pattern a (s1, _)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Undeclared relCond synonym after pattern") {
        vectorString input = {"assign a,a1;", "stmt s1;", "Select s1 pattern Modifies (s1, _)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }

    SECTION("Undeclared relCond synonym after and, pattern") {
        vectorString input = {"assign a,a1;", "stmt s1;", "Select s1 pattern a (_, _) and Modifies (s1, _)"};
        REQUIRE_THROW_SEMANTIC_ERROR(PQLParserTester::parse(input));
    }
}
