#include <qps/parser/PQLParser.h>

#include <tuple>

#include "catch.hpp"

// Testing Query Entity Parser

// Functions in PQLParser (for reference)
/**
 * public:
 *  static Query parse(UnparsedQuery);
 *  static std::string getQueryClauses(UnparsedQuery); -> no need
 *  static std::vector<QueryEntity> parseQueryEntities(std::vector<std::string>);
 *  static std::vector<QueryEntity> findSelectClauses(std::vector<QueryEntity>,  std::string);
 *  static std::vector<SuchThatClause> findSuchThatClauses(std::vector<QueryEntity>, std::string);
 *  static std::vector<PatternClause> findPatternClauses(std::vector<QueryEntity>, std::string);
*/

// parseQueryEntities
TEST_CASE("one_variable") {
    std::vector<std::string> one_variable = {"variable v1;"};
    std::vector<QueryEntity> expected_one_variable = {
        QueryEntity(EntityType::VARIABLE, "v1")};

    std::vector<QueryEntity> actual_one_variable =
        PQLParser::parseQueryEntities(one_variable);

    REQUIRE(actual_one_variable.size() == expected_one_variable.size());
    REQUIRE(actual_one_variable[0] == expected_one_variable[0]);
}

TEST_CASE("two_variables") {
    std::vector<std::string> two_variables = {"variable v1, v2;"};
    std::vector<QueryEntity> expected_two_variables = {
        QueryEntity(EntityType::VARIABLE, "v1"),
        QueryEntity(EntityType::VARIABLE, "v2")};
    std::vector<QueryEntity> actual_two_variables =
        PQLParser::parseQueryEntities(two_variables);

    REQUIRE(actual_two_variables.size() == expected_two_variables.size());
    REQUIRE(actual_two_variables[0] == expected_two_variables[0]);
    REQUIRE(actual_two_variables[1] == expected_two_variables[1]);
}

TEST_CASE("one_call_assign_stmt") {
    std::vector<std::string> one_call_assign_stmt = {"call c1;", "assign a1;",
                                                     "stmt s1;"};
    std::vector<QueryEntity> expected_one_call_assign_stmt = {
        QueryEntity(EntityType::CALL, "c1"),
        QueryEntity(EntityType::ASSIGN, "a1"),
        QueryEntity(EntityType::STMT, "s1")};

    std::vector<QueryEntity> actual_one_call_assign_stmt =
        PQLParser::parseQueryEntities(one_call_assign_stmt);

    REQUIRE(actual_one_call_assign_stmt.size() ==
            expected_one_call_assign_stmt.size());
    REQUIRE(actual_one_call_assign_stmt[0] == expected_one_call_assign_stmt[0]);
    REQUIRE(actual_one_call_assign_stmt[1] == expected_one_call_assign_stmt[1]);
    REQUIRE(actual_one_call_assign_stmt[2] == expected_one_call_assign_stmt[2]);
}

TEST_CASE("various_call_assign_stmt") {
    std::vector<std::string> various_call_assign_stmt = {
        "call c1, c2;", "assign a1;", "stmt s1, s2;"};
    std::vector<QueryEntity> expected_various_call_assign_stmt = {
        QueryEntity(EntityType::CALL, "c1"),
        QueryEntity(EntityType::CALL, "c2"),
        QueryEntity(EntityType::ASSIGN, "a1"),
        QueryEntity(EntityType::STMT, "s1"),
        QueryEntity(EntityType::STMT, "s2")};

    std::vector<QueryEntity> actual_various_call_assign_stmt =
        PQLParser::parseQueryEntities(various_call_assign_stmt);

    REQUIRE(actual_various_call_assign_stmt.size() ==
            expected_various_call_assign_stmt.size());
    REQUIRE(actual_various_call_assign_stmt[0] ==
            expected_various_call_assign_stmt[0]);
    REQUIRE(actual_various_call_assign_stmt[1] ==
            expected_various_call_assign_stmt[1]);
    REQUIRE(actual_various_call_assign_stmt[2] ==
            expected_various_call_assign_stmt[2]);
    REQUIRE(actual_various_call_assign_stmt[3] ==
            expected_various_call_assign_stmt[3]);
    REQUIRE(actual_various_call_assign_stmt[4] ==
            expected_various_call_assign_stmt[4]);
}


// Testing Find Queries functions

std::vector<QueryEntity> entities = {
        QueryEntity(EntityType::VARIABLE, "v1"),
        QueryEntity(EntityType::VARIABLE, "v2"),
        QueryEntity(EntityType::ASSIGN, "a1"),
        QueryEntity(EntityType::ASSIGN, "a2"),
        QueryEntity(EntityType::STMT, "s1"),
        QueryEntity(EntityType::STMT, "s2")};

// // Test cases
// // Select
// std::string select_1 = "Select v1";
// std::string select_2 = "  Select    v1  ";

// // Select such that Parent/Follows
// std::string select_st_1 = "Select s1 such that Parent(s1,s2)";
// std::string select_st_2 = "  Select s1  such  that Parent  ( s1,  s2 ) ";
// std::string select_st_star_1 = "Select s1 such that Parent*(s1,s2)";
// std::string select_st_star_2 = "   Select s1  such  that Parent*  (s1,  s2)   ";
// std::string select_st_1 = "Select s1 such that Follows(s1,s2)";
// std::string select_st_2 = "  Select s1  such  that Follows  ( s1,  s2 ) ";
// std::string select_st_star_1 = "Select s1 such that Follows*(s1,s2)";
// std::string select_st_star_2 = "   Select s1  such  that Follows*  (s1,  s2)   ";

// // Select pattern
// std::string select_pt_1 = "Select s1 pattern a1(v1,v2)";
// std::string select_pt_2 = "   Select s1   pattern  a1  (v1,v2)   ";

// // Select such that pattern
// std::string select_stp_1 = "Select s1 such that Follows(s1,s2) pattern a1(v1,v2)";
// std::string select_stp_2 = "   Select s1 such   that Follows  (s1,  s2) pattern a1  (v1,  v2)   ";
// std::string select_stp_3 = "Select s1 such that Follows*(s1,s2) pattern a1(v1,v2)";
// std::string select_stp_4 = "   Select s1 such   that Follows*  (s1,  s2) pattern a1  (v1,  v2)   ";

TEST_CASE("PQLParser for Select ...") {
    std::string select_1 = "Select v1";
    std::string select_2 = "  Select    v1  ";
    
    std::vector<QueryEntity> result_1 = PQLParser::findSelectClauses(entities, select_1);
    std::vector<QueryEntity> result_2 = PQLParser::findSelectClauses(entities, select_2);
    std::vector<SuchThatClause> result_3 = PQLParser::findSuchThatClauses(entities, select_1);
    std::vector<SuchThatClause> result_4 = PQLParser::findSuchThatClauses(entities, select_2);
    std::vector<PatternClause> result_5 = PQLParser::findPatternClauses(entities, select_1);
    std::vector<PatternClause> result_6 = PQLParser::findPatternClauses(entities, select_2);
    
    REQUIRE(QueryEntity(EntityType::VARIABLE, "v1")==result_1[0]);
    REQUIRE(result_1.size() == 1);
    REQUIRE(QueryEntity(EntityType::VARIABLE, "v1")==result_2[0]);
    REQUIRE(result_2.size() == 1);

    REQUIRE(result_3.size() == 0);
    REQUIRE(result_4.size() == 0);
    REQUIRE(result_5.size() == 0);
    REQUIRE(result_6.size() == 0);
}

// Select such that Parent/Follows
TEST_CASE("PQLParser for Select ... st Parent(*)/Follows(*)") {
    // Select such that Parent/Follows
    std::string select_st_1 = "Select s1 such that Parent(s1,s2)";
    std::string select_st_2 = "  Select s1  such  that Parent  ( s1,  s2 ) ";
    std::string select_st_star_1 = "Select s1 such that Parent*(s1,s2)";
    std::string select_st_star_2 = "   Select s1  such  that Parent*  (s1,  s2)   ";
    
    std::vector<QueryEntity> result_01 = PQLParser::findSelectClauses(entities, select_st_1);
    std::vector<QueryEntity> result_02 = PQLParser::findSelectClauses(entities, select_st_2);
    std::vector<QueryEntity> result_03 = PQLParser::findSelectClauses(entities, select_st_star_1);
    std::vector<QueryEntity> result_04 = PQLParser::findSelectClauses(entities, select_st_star_2);

    std::vector<SuchThatClause> result_11 = PQLParser::findSuchThatClauses(entities, select_st_1);
    std::vector<SuchThatClause> result_12 = PQLParser::findSuchThatClauses(entities, select_st_2);
    std::vector<SuchThatClause> result_13 = PQLParser::findSuchThatClauses(entities, select_st_star_1);
    std::vector<SuchThatClause> result_14 = PQLParser::findSuchThatClauses(entities, select_st_star_2);

    std::vector<PatternClause> result_21 = PQLParser::findPatternClauses(entities, select_st_1);
    std::vector<PatternClause> result_22 = PQLParser::findPatternClauses(entities, select_st_2);
    std::vector<PatternClause> result_23 = PQLParser::findPatternClauses(entities, select_st_star_1);
    std::vector<PatternClause> result_24 = PQLParser::findPatternClauses(entities, select_st_star_2);
    
    REQUIRE(QueryEntity(EntityType::STMT, "s1")==result_01[0]);
    REQUIRE(result_01.size() == 1);
    REQUIRE(QueryEntity(EntityType::STMT, "s1")==result_02[0]);
    REQUIRE(result_02.size() == 1);
    REQUIRE(QueryEntity(EntityType::STMT, "s1")==result_03[0]);
    REQUIRE(result_03.size() == 1);
    REQUIRE(QueryEntity(EntityType::STMT, "s1")==result_04[0]);
    REQUIRE(result_04.size() == 1);

    SuchThatClause ans1 = SuchThatClause(SuchThatClauseType::PARENT,
                            QueryEntity(EntityType::STMT, "s1"),
                            QueryEntity(EntityType::STMT, "s2"));
    SuchThatClause ans2 = SuchThatClause(SuchThatClauseType::PARENT_STAR,
                            QueryEntity(EntityType::STMT, "s1"),
                            QueryEntity(EntityType::STMT, "s2"));
    REQUIRE(ans1.equals(result_11[0]));
    REQUIRE(result_11.size() == 1);
    REQUIRE(ans1.equals(result_12[0]));
    REQUIRE(result_12.size() == 1);
    REQUIRE(ans2.equals(result_13[0]));
    REQUIRE(result_13.size() == 1);
    REQUIRE(ans2.equals(result_14[0]));
    REQUIRE(result_14.size() == 1);

    REQUIRE(result_21.size() == 0);
    REQUIRE(result_22.size() == 0);
    REQUIRE(result_23.size() == 0);
    REQUIRE(result_24.size() == 0);
}

// Select pattern
TEST_CASE("PQLParser for Select ... pattern") {
    std::string select_pt_1 = "Select v1 pattern a1(v1,v2)";
    std::string select_pt_2 = "   Select v1   pattern  a1  (v1,v2)   ";
    
    std::vector<QueryEntity> result_1 = PQLParser::findSelectClauses(entities, select_pt_1);
    std::vector<QueryEntity> result_2 = PQLParser::findSelectClauses(entities, select_pt_2);
    std::vector<SuchThatClause> result_3 = PQLParser::findSuchThatClauses(entities, select_pt_1);
    std::vector<SuchThatClause> result_4 = PQLParser::findSuchThatClauses(entities, select_pt_2);
    std::vector<PatternClause> result_5 = PQLParser::findPatternClauses(entities, select_pt_1);
    std::vector<PatternClause> result_6 = PQLParser::findPatternClauses(entities, select_pt_2);
    
    REQUIRE(QueryEntity(EntityType::VARIABLE, "v1")==result_1[0]);
    REQUIRE(result_1.size() == 1);
    REQUIRE(QueryEntity(EntityType::VARIABLE, "v1")==result_2[0]);
    REQUIRE(result_2.size() == 1);

    REQUIRE(result_3.size() == 0);
    REQUIRE(result_4.size() == 0);

    PatternClause ans1 = PatternClause(QueryEntity(EntityType::ASSIGN, "a1"),
                                        QueryEntity(EntityType::VARIABLE, "v1"),
                                        QueryEntity(EntityType::VARIABLE, "v2"));
    REQUIRE(ans1.equals(result_5[0]));
    REQUIRE(result_5.size() == 1);
    REQUIRE(ans1.equals(result_6[0]));
    REQUIRE(result_6.size() == 1);
}

// Select st pattern
TEST_CASE("PQLParser for Select ... st ... pattern") {
    std::string select_stp_1 = "Select s1 such that Follows(s1,s2) pattern a1(v1,v2)";
    std::string select_stp_2 = "   Select s1 such   that Follows  (s1,  s2) pattern a1  (v1,  v2)   ";
    
    std::vector<QueryEntity> result_1 = PQLParser::findSelectClauses(entities, select_stp_1);
    std::vector<QueryEntity> result_2 = PQLParser::findSelectClauses(entities, select_stp_2);
    std::vector<SuchThatClause> result_3 = PQLParser::findSuchThatClauses(entities, select_stp_1);
    std::vector<SuchThatClause> result_4 = PQLParser::findSuchThatClauses(entities, select_stp_2);
    std::vector<PatternClause> result_5 = PQLParser::findPatternClauses(entities, select_stp_1);
    std::vector<PatternClause> result_6 = PQLParser::findPatternClauses(entities, select_stp_2);
    
    REQUIRE(result_1[0] == QueryEntity(EntityType::STMT, "s1"));
    REQUIRE(result_1.size() == 1);
    REQUIRE(result_2[0] == QueryEntity(EntityType::STMT, "s1"));
    REQUIRE(result_2.size() == 1);

    SuchThatClause ans1 = SuchThatClause(SuchThatClauseType::FOLLOWS,
                            QueryEntity(EntityType::STMT, "s1"),
                            QueryEntity(EntityType::STMT, "s2"));
    REQUIRE(ans1.equals(result_3[0]));
    REQUIRE(result_3.size() == 1);
    REQUIRE(ans1.equals(result_4[0]));
    REQUIRE(result_4.size() == 1);

    // Didn't test FOLLOWS_STAR as should be same as Follows + Going to change many things anyway.

    PatternClause ans2 = PatternClause(QueryEntity(EntityType::ASSIGN, "a1"),
                                        QueryEntity(EntityType::VARIABLE, "v1"),
                                        QueryEntity(EntityType::VARIABLE, "v2"));
    REQUIRE(ans2.equals(result_5[0]));
    REQUIRE(result_5.size() == 1);
    REQUIRE(ans2.equals(result_6[0]));
    REQUIRE(result_6.size() == 1);
}