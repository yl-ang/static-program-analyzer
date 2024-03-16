#include <qps/parser/PQLParser.h>

#include <tuple>

#include "catch.hpp"

// parsing entities

TEST_CASE("one_variable") {
    std::vector<std::string> one_variable = {"variable v1;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {{"v1", DesignEntityType::VARIABLE}};

    SynonymStore expected_one_variable = SynonymStore(storeMap);
    SynonymStore actual_one_variable = PQLParser::parseQueryEntities(one_variable);

    REQUIRE(actual_one_variable == expected_one_variable);
}

TEST_CASE("two_variables") {
    std::vector<std::string> two_variables = {"variable v1, v2;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {{"v1", DesignEntityType::VARIABLE},
                                                                  {"v2", DesignEntityType::VARIABLE}};

    SynonymStore expected_two_variables = SynonymStore(storeMap);
    SynonymStore actual_two_variables = PQLParser::parseQueryEntities(two_variables);

    REQUIRE(actual_two_variables == expected_two_variables);
}

TEST_CASE("one_call_assign_stmt") {
    std::vector<std::string> one_call_assign_stmt = {"call c1;", "assign a1;", "stmt s1;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {
        {"c1", DesignEntityType::CALL}, {"a1", DesignEntityType::ASSIGN}, {"s1", DesignEntityType::STMT}};

    SynonymStore expected_one_call_assign_stmt = SynonymStore(storeMap);
    SynonymStore actual_one_call_assign_stmt = PQLParser::parseQueryEntities(one_call_assign_stmt);

    REQUIRE(actual_one_call_assign_stmt == expected_one_call_assign_stmt);
}

TEST_CASE("various_call_assign_stmt") {
    std::vector<std::string> various_call_assign_stmt = {"call c1, c2;", "assign a1;", "stmt s1, s2;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {{"c1", DesignEntityType::CALL},
                                                                  {"c2", DesignEntityType::CALL},
                                                                  {"a1", DesignEntityType::ASSIGN},
                                                                  {"s1", DesignEntityType::STMT},
                                                                  {"s2", DesignEntityType::STMT}};

    SynonymStore expected_various_call_assign_stmt = SynonymStore(storeMap);
    SynonymStore actual_various_call_assign_stmt = PQLParser::parseQueryEntities(various_call_assign_stmt);

    REQUIRE(actual_various_call_assign_stmt == expected_various_call_assign_stmt);
}

TEST_CASE("duplicate_synonym_same_stmt") {
    std::vector<std::string> various_call_assign_stmt = {"call c1, c1;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {{"c1", DesignEntityType::CALL}};

    SynonymStore expected_various_call_assign_stmt = SynonymStore(storeMap);
    SynonymStore actual_various_call_assign_stmt = PQLParser::parseQueryEntities(various_call_assign_stmt);

    REQUIRE(actual_various_call_assign_stmt == expected_various_call_assign_stmt);
    REQUIRE_FALSE(actual_various_call_assign_stmt.isValidStore());
}

TEST_CASE("duplicate_synonym_diff_stmt") {
    std::vector<std::string> various_call_assign_stmt = {"call c1;", "call c1;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {{"c1", DesignEntityType::CALL}};

    SynonymStore expected_various_call_assign_stmt = SynonymStore(storeMap);
    SynonymStore actual_various_call_assign_stmt = PQLParser::parseQueryEntities(various_call_assign_stmt);

    REQUIRE(actual_various_call_assign_stmt == expected_various_call_assign_stmt);
    REQUIRE_FALSE(actual_various_call_assign_stmt.isValidStore());
}

// Testing Find Queries functions

// Test cases

TEST_CASE("PQLParser: Select (1)") {
    std::string select_1 = "Select v1";
    std::string select_2 = "Select a1";
    std::string select_3 = "Select s1";

    std::vector<Synonym> result_1 = PQLParser::findSelectClauses(select_1);
    std::vector<Synonym> result_2 = PQLParser::findSelectClauses(select_2);
    std::vector<Synonym> result_3 = PQLParser::findSelectClauses(select_3);

    REQUIRE(Synonym(DesignEntityType::UNKNOWN, "v1") == result_1[0]);
    REQUIRE(result_1.size() == 1);
    REQUIRE(Synonym(DesignEntityType::UNKNOWN, "a1") == result_2[0]);
    REQUIRE(result_2.size() == 1);
    REQUIRE(Synonym(DesignEntityType::UNKNOWN, "s1") == result_3[0]);
    REQUIRE(result_3.size() == 1);
}

TEST_CASE("PQLParser: Select (2))") {
    std::string select_1 = "Select v1";

    std::vector<Synonym> result_1 = PQLParser::findSelectClauses(select_1);
    std::vector<SuchThatClause> result_3 = PQLParser::findSuchThatClauses(select_1);
    std::vector<PatternClause> result_5 = PQLParser::findPatternClauses(select_1);

    REQUIRE(Synonym(DesignEntityType::UNKNOWN, "v1") == result_1[0]);
    REQUIRE(result_1.size() == 1);

    REQUIRE(result_3.size() == 0);
    REQUIRE(result_5.size() == 0);
}

// NEW ONES below

// parsing entities

TEST_CASE("one_variable") {
    std::vector<std::string> one_variable = {"variable v1;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {{"v1", DesignEntityType::VARIABLE}};

    SynonymStore expected_one_variable = SynonymStore(storeMap);
    SynonymStore actual_one_variable = PQLParser::parseQueryEntities(one_variable);

    REQUIRE(actual_one_variable == expected_one_variable);
}

TEST_CASE("two_variables") {
    std::vector<std::string> two_variables = {"variable v1, v2;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {{"v1", DesignEntityType::VARIABLE},
                                                                  {"v2", DesignEntityType::VARIABLE}};

    SynonymStore expected_two_variables = SynonymStore(storeMap);
    SynonymStore actual_two_variables = PQLParser::parseQueryEntities(two_variables);

    REQUIRE(actual_two_variables == expected_two_variables);
}

TEST_CASE("one_call_assign_stmt") {
    std::vector<std::string> one_call_assign_stmt = {"call c1;", "assign a1;", "stmt s1;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {
        {"c1", DesignEntityType::CALL}, {"a1", DesignEntityType::ASSIGN}, {"s1", DesignEntityType::STMT}};

    SynonymStore expected_one_call_assign_stmt = SynonymStore(storeMap);
    SynonymStore actual_one_call_assign_stmt = PQLParser::parseQueryEntities(one_call_assign_stmt);

    REQUIRE(actual_one_call_assign_stmt == expected_one_call_assign_stmt);
}

TEST_CASE("various_call_assign_stmt") {
    std::vector<std::string> various_call_assign_stmt = {"call c1, c2;", "assign a1;", "stmt s1, s2;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {{"c1", DesignEntityType::CALL},
                                                                  {"c2", DesignEntityType::CALL},
                                                                  {"a1", DesignEntityType::ASSIGN},
                                                                  {"s1", DesignEntityType::STMT},
                                                                  {"s2", DesignEntityType::STMT}};

    SynonymStore expected_various_call_assign_stmt = SynonymStore(storeMap);
    SynonymStore actual_various_call_assign_stmt = PQLParser::parseQueryEntities(various_call_assign_stmt);

    REQUIRE(actual_various_call_assign_stmt == expected_various_call_assign_stmt);
}

TEST_CASE("duplicate_synonym_same_stmt") {
    std::vector<std::string> various_call_assign_stmt = {"call c1, c1;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {{"c1", DesignEntityType::CALL}};

    SynonymStore expected_various_call_assign_stmt = SynonymStore(storeMap);
    SynonymStore actual_various_call_assign_stmt = PQLParser::parseQueryEntities(various_call_assign_stmt);

    REQUIRE(actual_various_call_assign_stmt == expected_various_call_assign_stmt);
    REQUIRE_FALSE(actual_various_call_assign_stmt.isValidStore());
}

TEST_CASE("duplicate_synonym_diff_stmt") {
    std::vector<std::string> various_call_assign_stmt = {"call c1;", "call c1;"};
    std::unordered_map<std::string, DesignEntityType> storeMap = {{"c1", DesignEntityType::CALL}};

    SynonymStore expected_various_call_assign_stmt = SynonymStore(storeMap);
    SynonymStore actual_various_call_assign_stmt = PQLParser::parseQueryEntities(various_call_assign_stmt);

    REQUIRE(actual_various_call_assign_stmt == expected_various_call_assign_stmt);
    REQUIRE_FALSE(actual_various_call_assign_stmt.isValidStore());
}

// Testing Find Queries functions

// Test cases

TEST_CASE("PQLParser: Select (1)") {
    std::string select_1 = "Select v1";
    std::string select_2 = "Select a1";
    std::string select_3 = "Select s1";

    std::vector<Synonym> result_1 = PQLParser::findSelectClauses(select_1);
    std::vector<Synonym> result_2 = PQLParser::findSelectClauses(select_2);
    std::vector<Synonym> result_3 = PQLParser::findSelectClauses(select_3);

    REQUIRE(Synonym(DesignEntityType::UNKNOWN, "v1") == result_1[0]);
    REQUIRE(result_1.size() == 1);
    REQUIRE(Synonym(DesignEntityType::UNKNOWN, "a1") == result_2[0]);
    REQUIRE(result_2.size() == 1);
    REQUIRE(Synonym(DesignEntityType::UNKNOWN, "s1") == result_3[0]);
    REQUIRE(result_3.size() == 1);
}

TEST_CASE("PQLParser: Select (2))") {
    std::string select_1 = "Select v1";

    std::vector<Synonym> result_1 = PQLParser::findSelectClauses(select_1);
    std::vector<SuchThatClause> result_3 = PQLParser::findSuchThatClauses(select_1);
    std::vector<PatternClause> result_5 = PQLParser::findPatternClauses(select_1);

    REQUIRE(Synonym(DesignEntityType::UNKNOWN, "v1") == result_1[0]);
    REQUIRE(result_1.size() == 1);

    REQUIRE(result_3.size() == 0);
    REQUIRE(result_5.size() == 0);
}

// Select such that Parent()
// Used for testing Parent/Follows/Parent*/Follows*
// As syntax is the same for all these
// To ensure that Follows/Parent*/Follows* work, will test one clause each minimally
TEST_CASE("PQLParser: Parent (1)") {
    std::string select_st_1 = "Select s1 such that Parent(s1,s2)";
    std::string select_st_2 = "Select s1 such that Parent(s1,_)";
    std::string select_st_3 = "Select s1 such that Parent(s1,4)";

    std::vector<Synonym> result_01 = PQLParser::findSelectClauses(select_st_1);
    std::vector<Synonym> result_02 = PQLParser::findSelectClauses(select_st_2);
    std::vector<Synonym> result_03 = PQLParser::findSelectClauses(select_st_3);

    std::vector<SuchThatClause> result_11 = PQLParser::findSuchThatClauses(select_st_1);
    std::vector<SuchThatClause> result_12 = PQLParser::findSuchThatClauses(select_st_2);
    std::vector<SuchThatClause> result_13 = PQLParser::findSuchThatClauses(select_st_3);

    std::vector<PatternClause> result_21 = PQLParser::findPatternClauses(select_st_1);
    std::vector<PatternClause> result_22 = PQLParser::findPatternClauses(select_st_2);
    std::vector<PatternClause> result_23 = PQLParser::findPatternClauses(select_st_3);

    // Select checking
    REQUIRE(result_01.size() == 1);
    REQUIRE(result_02.size() == 1);
    REQUIRE(result_03.size() == 1);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_01[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_02[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_03[0]);

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

TEST_CASE("PQLParser: Parent (2)") {
    std::string select_st_4 = "Select s1 such that Parent(s2,_)";
    std::string select_st_5 = "Select s1 such that Parent(s2,4)";
    std::string select_st_6 = "Select s1 such that Parent(s2,s1)";

    std::vector<Synonym> result_01 = PQLParser::findSelectClauses(select_st_4);
    std::vector<Synonym> result_02 = PQLParser::findSelectClauses(select_st_5);
    std::vector<Synonym> result_03 = PQLParser::findSelectClauses(select_st_6);

    std::vector<SuchThatClause> result_11 = PQLParser::findSuchThatClauses(select_st_4);
    std::vector<SuchThatClause> result_12 = PQLParser::findSuchThatClauses(select_st_5);
    std::vector<SuchThatClause> result_13 = PQLParser::findSuchThatClauses(select_st_6);

    std::vector<PatternClause> result_21 = PQLParser::findPatternClauses(select_st_4);
    std::vector<PatternClause> result_22 = PQLParser::findPatternClauses(select_st_5);
    std::vector<PatternClause> result_23 = PQLParser::findPatternClauses(select_st_6);

    // Select checking
    REQUIRE(result_01.size() == 1);
    REQUIRE(result_02.size() == 1);
    REQUIRE(result_03.size() == 1);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_01[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_02[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_03[0]);

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

TEST_CASE("PQLParser: Parent (3)") {
    std::string select_st_7 = "Select s1 such that Parent(_,s1)";
    std::string select_st_8 = "Select s1 such that Parent(4,s1)";
    std::string select_st_9 = "Select s1 such that Parent(_,s2)";
    std::string select_st_10 = "Select s1 such that Parent(4,s2)";

    std::vector<Synonym> result_01 = PQLParser::findSelectClauses(select_st_7);
    std::vector<Synonym> result_02 = PQLParser::findSelectClauses(select_st_8);
    std::vector<Synonym> result_03 = PQLParser::findSelectClauses(select_st_9);
    std::vector<Synonym> result_04 = PQLParser::findSelectClauses(select_st_10);

    std::vector<SuchThatClause> result_11 = PQLParser::findSuchThatClauses(select_st_7);
    std::vector<SuchThatClause> result_12 = PQLParser::findSuchThatClauses(select_st_8);
    std::vector<SuchThatClause> result_13 = PQLParser::findSuchThatClauses(select_st_9);
    std::vector<SuchThatClause> result_14 = PQLParser::findSuchThatClauses(select_st_10);

    std::vector<PatternClause> result_21 = PQLParser::findPatternClauses(select_st_7);
    std::vector<PatternClause> result_22 = PQLParser::findPatternClauses(select_st_8);
    std::vector<PatternClause> result_23 = PQLParser::findPatternClauses(select_st_9);
    std::vector<PatternClause> result_24 = PQLParser::findPatternClauses(select_st_10);

    // Select checking
    REQUIRE(result_01.size() == 1);
    REQUIRE(result_02.size() == 1);
    REQUIRE(result_03.size() == 1);
    REQUIRE(result_04.size() == 1);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_01[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_02[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_03[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_04[0]);

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

// Spacing and other relationships
TEST_CASE("PQLParser: Parent (4)") {
    std::string select_st_11 = "   Select   s1   such  that  Parent (s1 ,  s2 ) ";
    std::string select_st_12 = "Select s1 such that Parent*(s1,s2)";
    std::string select_st_13 = "Select s1 such that Follows(s1,s2)";
    std::string select_st_14 = "Select s1 such that Follows*(s1,s2)";

    std::vector<Synonym> result_01 = PQLParser::findSelectClauses(select_st_11);
    std::vector<Synonym> result_02 = PQLParser::findSelectClauses(select_st_12);
    std::vector<Synonym> result_03 = PQLParser::findSelectClauses(select_st_13);
    std::vector<Synonym> result_04 = PQLParser::findSelectClauses(select_st_14);

    std::vector<SuchThatClause> result_11 = PQLParser::findSuchThatClauses(select_st_11);
    std::vector<SuchThatClause> result_12 = PQLParser::findSuchThatClauses(select_st_12);
    std::vector<SuchThatClause> result_13 = PQLParser::findSuchThatClauses(select_st_13);
    std::vector<SuchThatClause> result_14 = PQLParser::findSuchThatClauses(select_st_14);

    std::vector<PatternClause> result_21 = PQLParser::findPatternClauses(select_st_11);
    std::vector<PatternClause> result_22 = PQLParser::findPatternClauses(select_st_12);
    std::vector<PatternClause> result_23 = PQLParser::findPatternClauses(select_st_13);
    std::vector<PatternClause> result_24 = PQLParser::findPatternClauses(select_st_14);

    // Select checking
    REQUIRE(result_01.size() == 1);
    REQUIRE(result_02.size() == 1);
    REQUIRE(result_03.size() == 1);
    REQUIRE(result_04.size() == 1);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_01[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_02[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_03[0]);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_04[0]);

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
TEST_CASE("PQLParser: Select ... pattern (1)") {
    std::string select_pt_1 = "Select v1 pattern a1(v1,\"x+y\")";
    std::string select_pt_2 = "Select v1 pattern a1(v1,\"x + y\")";
    std::string select_pt_3 = "Select v1 pattern a1(v1,\"x +y\")";
    std::string select_pt_4 = "Select v1 pattern a1(v1,\" x +y \")";

    std::vector<PatternClause> result_1 = PQLParser::findPatternClauses(select_pt_1);
    std::vector<PatternClause> result_2 = PQLParser::findPatternClauses(select_pt_2);
    std::vector<PatternClause> result_3 = PQLParser::findPatternClauses(select_pt_3);
    std::vector<PatternClause> result_4 = PQLParser::findPatternClauses(select_pt_4);

    Synonym a1 = Synonym(DesignEntityType::ASSIGN, "a1");
    Synonym v1 = Synonym(DesignEntityType::VARIABLE, "v1");
    ExpressionSpec l = ExpressionSpec("\"x+y\"");
    PatternClause ans1 = PatternClause(static_cast<ClauseArgument*>(&a1), static_cast<ClauseArgument*>(&v1),
                                       static_cast<ClauseArgument*>(&l));
    REQUIRE(result_1.size() == 1);
    REQUIRE(result_2.size() == 1);
    REQUIRE(result_3.size() == 1);
    REQUIRE(result_4.size() == 1);
    REQUIRE(ans1.equals(result_1[0]));
    REQUIRE(ans1.equals(result_2[0]));
    REQUIRE(ans1.equals(result_3[0]));
    REQUIRE(ans1.equals(result_4[0]));
}

TEST_CASE("PQLParser: Select ... pattern (2)") {
    std::string select_pt_1 = "Select v1 pattern a1(v1,_\"x+y\"_)";
    std::string select_pt_2 = "Select v1 pattern a1(v1,_\"x + y\"_)";
    std::string select_pt_3 = "Select v1 pattern a1(v1,_\"x +y\"_)";
    std::string select_pt_4 = "Select v1 pattern a1(v1,_\" x +y \"_)";

    std::vector<PatternClause> result_1 = PQLParser::findPatternClauses(select_pt_1);
    std::vector<PatternClause> result_2 = PQLParser::findPatternClauses(select_pt_2);
    std::vector<PatternClause> result_3 = PQLParser::findPatternClauses(select_pt_3);
    std::vector<PatternClause> result_4 = PQLParser::findPatternClauses(select_pt_4);

    Synonym a1 = Synonym(DesignEntityType::ASSIGN, "a1");
    Synonym v1 = Synonym(DesignEntityType::VARIABLE, "v1");
    ExpressionSpec l = ExpressionSpec("_\"x+y\"_");
    PatternClause ans1 = PatternClause(static_cast<ClauseArgument*>(&a1), static_cast<ClauseArgument*>(&v1),
                                       static_cast<ClauseArgument*>(&l));
    REQUIRE(result_1.size() == 1);
    REQUIRE(result_2.size() == 1);
    REQUIRE(result_3.size() == 1);
    REQUIRE(result_4.size() == 1);
    REQUIRE(ans1.equals(result_1[0]));
    REQUIRE(ans1.equals(result_2[0]));
    REQUIRE(ans1.equals(result_3[0]));
    REQUIRE(ans1.equals(result_4[0]));
}

TEST_CASE("PQLParser: Select ... pattern (3)") {
    std::string select_pt_1 = "Select v1 pattern a1(v1,_\"x+y\"_)";
    std::string select_pt_2 = "Select v1 pattern a1(_,_\"x + y\"_)";
    std::string select_pt_3 = "Select v1 pattern a1(\"Whatever\",_\"x +y\"_)";
    std::string select_pt_4 = "   Select v1   pattern  a1  (v1,   \"Whatever\")   ";

    std::vector<PatternClause> result_1 = PQLParser::findPatternClauses(select_pt_1);
    std::vector<PatternClause> result_2 = PQLParser::findPatternClauses(select_pt_2);
    std::vector<PatternClause> result_3 = PQLParser::findPatternClauses(select_pt_3);
    std::vector<PatternClause> result_4 = PQLParser::findPatternClauses(select_pt_4);

    Synonym a1 = Synonym(DesignEntityType::ASSIGN, "a1");
    Synonym v1 = Synonym(DesignEntityType::VARIABLE, "v1");
    Synonym v2 = Synonym(DesignEntityType::VARIABLE, "v2");
    ExpressionSpec l = ExpressionSpec("_\"x+y\"_");
    ExpressionSpec ew = ExpressionSpec("\"Whatever\"");
    Literal lw = Literal("Whatever");
    Wildcard w = Wildcard();
    PatternClause ans1 = PatternClause(static_cast<ClauseArgument*>(&a1), static_cast<ClauseArgument*>(&v1),
                                       static_cast<ClauseArgument*>(&l));
    PatternClause ans2 = PatternClause(static_cast<ClauseArgument*>(&a1), static_cast<ClauseArgument*>(&w),
                                       static_cast<ClauseArgument*>(&l));
    PatternClause ans3 = PatternClause(static_cast<ClauseArgument*>(&a1), static_cast<ClauseArgument*>(&lw),
                                       static_cast<ClauseArgument*>(&l));
    PatternClause ans4 = PatternClause(static_cast<ClauseArgument*>(&a1), static_cast<ClauseArgument*>(&v1),
                                       static_cast<ClauseArgument*>(&ew));
    REQUIRE(result_1.size() == 1);
    REQUIRE(result_2.size() == 1);
    REQUIRE(result_3.size() == 1);
    REQUIRE(result_4.size() == 1);
    REQUIRE(ans1.equals(result_1[0]));
    REQUIRE(ans2.equals(result_2[0]));
    REQUIRE(ans3.equals(result_3[0]));
    REQUIRE(ans4.equals(result_4[0]));
}