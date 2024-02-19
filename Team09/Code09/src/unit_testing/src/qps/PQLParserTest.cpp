// #include <qps/clauses/QueryClause.h>
// #include <qps/parser/PQLParser.h>
// #include <qps/parser/QueryEntity.h>

// #include <tuple>

// #include "catch.hpp"

// // Testing Query Entity Parser

// TEST_CASE("one_variable") {
//     std::vector<std::string> one_variable = {"variable v1;"};
//     std::vector<QueryEntity> expected_one_variable = {
//         QueryEntity(EntityType::VARIABLE, "v1")};

//     std::vector<QueryEntity> actual_one_variable =
//         PQLParser::parseQueryEntities(one_variable);

//     REQUIRE(actual_one_variable.size() == expected_one_variable.size());
//     REQUIRE(actual_one_variable[0] == expected_one_variable[0]);
// }

// TEST_CASE("two_variables") {
//     std::vector<std::string> two_variables = {"variable v1, v2;"};
//     std::vector<QueryEntity> expected_two_variables = {
//         QueryEntity(EntityType::VARIABLE, "v1"),
//         QueryEntity(EntityType::VARIABLE, "v2")};
//     std::vector<QueryEntity> actual_two_variables =
//         PQLParser::parseQueryEntities(two_variables);

//     REQUIRE(actual_two_variables.size() == expected_two_variables.size());
//     REQUIRE(actual_two_variables[0] == expected_two_variables[0]);
//     REQUIRE(actual_two_variables[1] == expected_two_variables[1]);
// }

// TEST_CASE("one_call_assign_stmt") {
//     std::vector<std::string> one_call_assign_stmt = {"call c1;", "assign a1;",
//                                                      "stmt s1;"};
//     std::vector<QueryEntity> expected_one_call_assign_stmt = {
//         QueryEntity(EntityType::CALL, "c1"),
//         QueryEntity(EntityType::ASSIGN, "a1"),
//         QueryEntity(EntityType::STMT, "s1")};

//     std::vector<QueryEntity> actual_one_call_assign_stmt =
//         PQLParser::parseQueryEntities(one_call_assign_stmt);

//     REQUIRE(actual_one_call_assign_stmt.size() ==
//             expected_one_call_assign_stmt.size());
//     REQUIRE(actual_one_call_assign_stmt[0] == expected_one_call_assign_stmt[0]);
//     REQUIRE(actual_one_call_assign_stmt[1] == expected_one_call_assign_stmt[1]);
//     REQUIRE(actual_one_call_assign_stmt[2] == expected_one_call_assign_stmt[2]);
// }

// TEST_CASE("various_call_assign_stmt") {
//     std::vector<std::string> various_call_assign_stmt = {
//         "call c1, c2;", "assign a1;", "stmt s1, s2;"};
//     std::vector<QueryEntity> expected_various_call_assign_stmt = {
//         QueryEntity(EntityType::CALL, "c1"),
//         QueryEntity(EntityType::CALL, "c2"),
//         QueryEntity(EntityType::ASSIGN, "a1"),
//         QueryEntity(EntityType::STMT, "s1"),
//         QueryEntity(EntityType::STMT, "s2")};

//     std::vector<QueryEntity> actual_various_call_assign_stmt =
//         PQLParser::parseQueryEntities(various_call_assign_stmt);

//     REQUIRE(actual_various_call_assign_stmt.size() ==
//             expected_various_call_assign_stmt.size());
//     REQUIRE(actual_various_call_assign_stmt[0] ==
//             expected_various_call_assign_stmt[0]);
//     REQUIRE(actual_various_call_assign_stmt[1] ==
//             expected_various_call_assign_stmt[1]);
//     REQUIRE(actual_various_call_assign_stmt[2] ==
//             expected_various_call_assign_stmt[2]);
//     REQUIRE(actual_various_call_assign_stmt[3] ==
//             expected_various_call_assign_stmt[3]);
//     REQUIRE(actual_various_call_assign_stmt[4] ==
//             expected_various_call_assign_stmt[4]);
// }

