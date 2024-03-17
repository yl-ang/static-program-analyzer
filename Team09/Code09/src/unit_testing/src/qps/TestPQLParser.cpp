#include <qps/parser/PQLParser.h>

#include <tuple>

#include "catch.hpp"

// parsing entities
TEST_CASE("one_variable") {
   std::vector<std::string> one_variable = {"variable v1;"};
   std::vector<Synonym> expected_one_variable = {Synonym(DesignEntityType::VARIABLE, "v1")};
   SynonymStore synonymStore = PQLParser::parseQueryEntities(one_variable);
   REQUIRE(synonymStore.containsSynonym(expected_one_variable[0]));
}
TEST_CASE("two_variables") {
   std::vector<std::string> two_variables = {"variable v1, v2;"};
   std::vector<Synonym> expected_two_variables = {Synonym(DesignEntityType::VARIABLE, "v1"),
                                                  Synonym(DesignEntityType::VARIABLE, "v2")};
   SynonymStore synonymStore = PQLParser::parseQueryEntities(two_variables);
   REQUIRE(synonymStore.containsSynonym(expected_two_variables[0]));
   REQUIRE(synonymStore.containsSynonym(expected_two_variables[1]));
}
TEST_CASE("one_call_assign_stmt") {
   std::vector<std::string> one_call_assign_stmt = {"call c1;", "assign a1;", "stmt s1;"};
   std::vector<Synonym> expected_one_call_assign_stmt = {Synonym(DesignEntityType::CALL, "c1"),
                                                         Synonym(DesignEntityType::ASSIGN, "a1"),
                                                         Synonym(DesignEntityType::STMT, "s1")};

   SynonymStore synonymStore = PQLParser::parseQueryEntities(one_call_assign_stmt);
   
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
   SynonymStore synonymStore = PQLParser::parseQueryEntities(various_call_assign_stmt);
   REQUIRE(synonymStore.containsSynonym(expected_various_call_assign_stmt[0]));
   REQUIRE(synonymStore.containsSynonym(expected_various_call_assign_stmt[1]));
   REQUIRE(synonymStore.containsSynonym(expected_various_call_assign_stmt[2]));
   REQUIRE(synonymStore.containsSynonym(expected_various_call_assign_stmt[3]));
   REQUIRE(synonymStore.containsSynonym(expected_various_call_assign_stmt[4]));
}

// Testing Find Queries functions
std::vector<std::string> various_assorted = {"variable v1, v2;", "assign a1;", "stmt s1, s2;"};
SynonymStore entities = PQLParser::parseQueryEntities(various_assorted);

// Test cases

TEST_CASE("PQLParser: Select (1)") {
    std::string select_1 = "Select v1";
    std::string select_2 = "Select a1";
    std::string select_3 = "Select s1";

    std::vector<Synonym> result_1 = PQLParser::findSelectClauses(select_1);
    std::vector<Synonym> result_2 = PQLParser::findSelectClauses(select_2);
    std::vector<Synonym> result_3 = PQLParser::findSelectClauses(select_3);
    std::vector<SuchThatClause> suchThatClauses = {};
    std::vector<PatternClause> patternClauses = {};
    Validator::validateClauses(&entities, result_1, suchThatClauses, patternClauses);
    Validator::validateClauses(&entities, result_2, suchThatClauses, patternClauses);
    Validator::validateClauses(&entities, result_3, suchThatClauses, patternClauses);

    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v1") == result_1[0]);
    REQUIRE(result_1.size() == 1);
    REQUIRE(Synonym(DesignEntityType::ASSIGN, "a1") == result_2[0]);
    REQUIRE(result_2.size() == 1);
    REQUIRE(Synonym(DesignEntityType::STMT, "s1") == result_3[0]);
    REQUIRE(result_3.size() == 1);
}

TEST_CASE("PQLParser: Select (2))") {
    std::string select_1 = "Select <v1,v2>";
    std::string select_2 = "  Select    <v1,    v2>  ";

    std::vector<Synonym> result_1 = PQLParser::findSelectClauses(select_1);
    std::vector<Synonym> result_2 = PQLParser::findSelectClauses(select_2);
    std::vector<SuchThatClause> suchThatClauses = {};
    std::vector<PatternClause> patternClauses = {};
    Validator::validateClauses(&entities, result_1, suchThatClauses, patternClauses);
    Validator::validateClauses(&entities, result_2, suchThatClauses, patternClauses);

    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v1") == result_1[0]);
    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v2") == result_1[1]);
    REQUIRE(result_1.size() == 2);
    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v1") == result_2[0]);
    REQUIRE(Synonym(DesignEntityType::VARIABLE, "v2") == result_2[1]);
    REQUIRE(result_2.size() == 2);
}

TEST_CASE("PQLParser: Select (3))") {
    std::string select_1 = "Select BOOLEAN";
    std::string select_2 = "  Select    BOOLEAN  ";

    std::vector<Synonym> result_1 = PQLParser::findSelectClauses(select_1);
    std::vector<Synonym> result_2 = PQLParser::findSelectClauses(select_2);
    std::vector<SuchThatClause> suchThatClauses = {};
    std::vector<PatternClause> patternClauses = {};
    Validator::validateClauses(&entities, result_1, suchThatClauses, patternClauses);
    Validator::validateClauses(&entities, result_2, suchThatClauses, patternClauses);

    REQUIRE(result_1.size() == 0);
    REQUIRE(result_2.size() == 0);
}

// Select such that Parent()
// Used for testing Parent/Follows/Parent*/Follows*
// As syntax is the same for all these
// To ensure that Follows/Parent*/Follows* work, will test one clause each minimally
TEST_CASE("PQLParser: Parent (1)") {
    std::string select_st_1 = "Select s1 such that Parent(s1,s2)";
    std::string select_st_2 = "Select s1 such that Parent(s1,_)";
    std::string select_st_3 = "Select s1 such that Parent(s1,4)";
    std::vector<std::string> clauseList_1 = getAllClauses(select_st_1);
    std::vector<std::string> clauseList_2 = getAllClauses(select_st_2);
    std::vector<std::string> clauseList_3 = getAllClauses(select_st_3);

    std::vector<Synonym> result_01 = PQLParser::findSelectClauses(select_st_1);
    std::vector<Synonym> result_02 = PQLParser::findSelectClauses(select_st_2);
    std::vector<Synonym> result_03 = PQLParser::findSelectClauses(select_st_3);

    std::vector<SuchThatClause> result_11 = PQLParser::parseSuchThatClauses(clauseList_1);
    std::vector<SuchThatClause> result_12 = PQLParser::parseSuchThatClauses(clauseList_2);
    std::vector<SuchThatClause> result_13 = PQLParser::parseSuchThatClauses(clauseList_3);

    std::vector<PatternClause> result_21 = PQLParser::parsePatternClauses(clauseList_1);
    std::vector<PatternClause> result_22 = PQLParser::parsePatternClauses(clauseList_2);
    std::vector<PatternClause> result_23 = PQLParser::parsePatternClauses(clauseList_3);

    Validator::validateClauses(&entities, result_01, result_11, result_21);
    Validator::validateClauses(&entities, result_02, result_12, result_22);
    Validator::validateClauses(&entities, result_03, result_13, result_23);

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
    std::vector<std::string> clauseList_4 = getAllClauses(select_st_4);
    std::vector<std::string> clauseList_5 = getAllClauses(select_st_5);
    std::vector<std::string> clauseList_6 = getAllClauses(select_st_6);

    std::vector<Synonym> result_01 = PQLParser::findSelectClauses(select_st_4);
    std::vector<Synonym> result_02 = PQLParser::findSelectClauses(select_st_5);
    std::vector<Synonym> result_03 = PQLParser::findSelectClauses(select_st_6);

    std::vector<SuchThatClause> result_11 = PQLParser::parseSuchThatClauses(clauseList_4);
    std::vector<SuchThatClause> result_12 = PQLParser::parseSuchThatClauses(clauseList_5);
    std::vector<SuchThatClause> result_13 = PQLParser::parseSuchThatClauses(clauseList_6);

    std::vector<PatternClause> result_21 = PQLParser::parsePatternClauses(clauseList_4);
    std::vector<PatternClause> result_22 = PQLParser::parsePatternClauses(clauseList_5);
    std::vector<PatternClause> result_23 = PQLParser::parsePatternClauses(clauseList_6);

    Validator::validateClauses(&entities, result_01, result_11, result_21);
    Validator::validateClauses(&entities, result_02, result_12, result_22);
    Validator::validateClauses(&entities, result_03, result_13, result_23);

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
    std::vector<std::string> clauseList_7 = getAllClauses(select_st_7);
    std::vector<std::string> clauseList_8 = getAllClauses(select_st_8);
    std::vector<std::string> clauseList_9 = getAllClauses(select_st_9);
    std::vector<std::string> clauseList_10 = getAllClauses(select_st_10);

    std::vector<Synonym> result_01 = PQLParser::findSelectClauses(select_st_7);
    std::vector<Synonym> result_02 = PQLParser::findSelectClauses(select_st_8);
    std::vector<Synonym> result_03 = PQLParser::findSelectClauses(select_st_9);
    std::vector<Synonym> result_04 = PQLParser::findSelectClauses(select_st_10);

    std::vector<SuchThatClause> result_11 = PQLParser::parseSuchThatClauses(clauseList_7);
    std::vector<SuchThatClause> result_12 = PQLParser::parseSuchThatClauses(clauseList_8);
    std::vector<SuchThatClause> result_13 = PQLParser::parseSuchThatClauses(clauseList_9);
    std::vector<SuchThatClause> result_14 = PQLParser::parseSuchThatClauses(clauseList_10);

    std::vector<PatternClause> result_21 = PQLParser::parsePatternClauses(clauseList_7);
    std::vector<PatternClause> result_22 = PQLParser::parsePatternClauses(clauseList_8);
    std::vector<PatternClause> result_23 = PQLParser::parsePatternClauses(clauseList_9);
    std::vector<PatternClause> result_24 = PQLParser::parsePatternClauses(clauseList_10);

    Validator::validateClauses(&entities, result_01, result_11, result_21);
    Validator::validateClauses(&entities, result_02, result_12, result_22);
    Validator::validateClauses(&entities, result_03, result_13, result_23);
    Validator::validateClauses(&entities, result_04, result_14, result_24);

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
    std::vector<std::string> clauseList_11 = getAllClauses(select_st_11);
    std::vector<std::string> clauseList_12 = getAllClauses(select_st_12);
    std::vector<std::string> clauseList_13 = getAllClauses(select_st_13);
    std::vector<std::string> clauseList_14 = getAllClauses(select_st_14);

    std::vector<Synonym> result_01 = PQLParser::findSelectClauses(select_st_11);
    std::vector<Synonym> result_02 = PQLParser::findSelectClauses(select_st_12);
    std::vector<Synonym> result_03 = PQLParser::findSelectClauses(select_st_13);
    std::vector<Synonym> result_04 = PQLParser::findSelectClauses(select_st_14);

    std::vector<SuchThatClause> result_11 = PQLParser::parseSuchThatClauses(clauseList_11);
    std::vector<SuchThatClause> result_12 = PQLParser::parseSuchThatClauses(clauseList_12);
    std::vector<SuchThatClause> result_13 = PQLParser::parseSuchThatClauses(clauseList_13);
    std::vector<SuchThatClause> result_14 = PQLParser::parseSuchThatClauses(clauseList_14);

    std::vector<PatternClause> result_21 = PQLParser::parsePatternClauses(clauseList_11);
    std::vector<PatternClause> result_22 = PQLParser::parsePatternClauses(clauseList_12);
    std::vector<PatternClause> result_23 = PQLParser::parsePatternClauses(clauseList_13);
    std::vector<PatternClause> result_24 = PQLParser::parsePatternClauses(clauseList_14);

    Validator::validateClauses(&entities, result_01, result_11, result_21);
    Validator::validateClauses(&entities, result_02, result_12, result_22);
    Validator::validateClauses(&entities, result_03, result_13, result_23);
    Validator::validateClauses(&entities, result_04, result_14, result_24);

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
    std::vector<std::string> clauseList_1 = getAllClauses(select_pt_1);
    std::vector<std::string> clauseList_2 = getAllClauses(select_pt_2);
    std::vector<std::string> clauseList_3 = getAllClauses(select_pt_3);
    std::vector<std::string> clauseList_4 = getAllClauses(select_pt_4);

    std::vector<PatternClause> result_1 = PQLParser::parsePatternClauses(clauseList_1);
    std::vector<PatternClause> result_2 = PQLParser::parsePatternClauses(clauseList_2);
    std::vector<PatternClause> result_3 = PQLParser::parsePatternClauses(clauseList_3);
    std::vector<PatternClause> result_4 = PQLParser::parsePatternClauses(clauseList_4);
    
    std::vector<SuchThatClause> suchThatClauses = {};
    std::vector<Synonym> selectClauses = {};
    Validator::validateClauses(&entities, selectClauses, suchThatClauses, result_1);
    Validator::validateClauses(&entities, selectClauses, suchThatClauses, result_2);
    Validator::validateClauses(&entities, selectClauses, suchThatClauses, result_3);
    Validator::validateClauses(&entities, selectClauses, suchThatClauses, result_4);

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
    std::vector<std::string> clauseList_1 = getAllClauses(select_pt_1);
    std::vector<std::string> clauseList_2 = getAllClauses(select_pt_2);
    std::vector<std::string> clauseList_3 = getAllClauses(select_pt_3);
    std::vector<std::string> clauseList_4 = getAllClauses(select_pt_4);

    std::vector<PatternClause> result_1 = PQLParser::parsePatternClauses(clauseList_1);
    std::vector<PatternClause> result_2 = PQLParser::parsePatternClauses(clauseList_2);
    std::vector<PatternClause> result_3 = PQLParser::parsePatternClauses(clauseList_3);
    std::vector<PatternClause> result_4 = PQLParser::parsePatternClauses(clauseList_4);

    std::vector<SuchThatClause> suchThatClauses = {};
    std::vector<Synonym> selectClauses = {};
    Validator::validateClauses(&entities, selectClauses, suchThatClauses, result_1);
    Validator::validateClauses(&entities, selectClauses, suchThatClauses, result_2);
    Validator::validateClauses(&entities, selectClauses, suchThatClauses, result_3);
    Validator::validateClauses(&entities, selectClauses, suchThatClauses, result_4);

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

TEST_CASE("PQLParser: Select ... pattern (4)") {
    std::string select_pt_1 = "Select a such that Uses (a1, v) pattern a (v, _\"temp\"_)";

    // Testing Find Queries functions
    std::vector<std::string> various_assorted_1 = {"assign a, a1;", "variable v;"};
    SynonymStore entities_1 = PQLParser::parseQueryEntities(various_assorted_1);

    std::vector<std::string> clauseList_1 = getAllClauses(select_pt_1);
    std::vector<Synonym> selectClauses = {Synonym(DesignEntityType::ASSIGN, "a")};
    std::vector<SuchThatClause> suchThatClauses = PQLParser::parseSuchThatClauses(clauseList_1);
    std::vector<PatternClause> patternClauses = PQLParser::parsePatternClauses(clauseList_1);

    Validator::validateClauses(&entities_1, selectClauses, suchThatClauses, patternClauses);

    Synonym a = Synonym(DesignEntityType::ASSIGN, "a");
    Synonym a1 = Synonym(DesignEntityType::ASSIGN, "a1");
    Synonym v = Synonym(DesignEntityType::VARIABLE, "v");

    ExpressionSpec temp = ExpressionSpec("_\"temp\"_");

    SuchThatClause s = SuchThatClause(RelationshipType::USES, static_cast<ClauseArgument*>(&a1), static_cast<ClauseArgument*>(&v));
    PatternClause p = PatternClause(static_cast<ClauseArgument*>(&a), static_cast<ClauseArgument*>(&v),
                                       static_cast<ClauseArgument*>(&temp));
    REQUIRE(suchThatClauses.size() == 1);
    REQUIRE(patternClauses.size() == 1);
    REQUIRE(s.equals(suchThatClauses[0]));
    REQUIRE(p.equals(patternClauses[0]));
}

