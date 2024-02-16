#include <QPS/QpsTokenizer.h>

#include "catch.hpp"

TEST_CASE("Tokenizer Test") {
    std::vector<std::string> expectedResultList_NormalQuery = {"variable v;",
                                                               "Select v"};
    std::vector<std::string> expectedResultList_NoSemiColon = {
        "variable v Select v"};
    std::vector<std::string> expectedResultList_WithTabs = {"variable v;",
                                                            "Sel ect v"};
    std::vector<std::string> expectedResultList_EndSemiColon = {"variable v;",
                                                                "assign a;"};
    std::vector<std::string> expectedResultList_MoreThanOneSemiColon = {
        "variable v;", "assign a;", "Select v"};

    std::string inputString_NormalQuery = "variable v; Select v";
    std::string inputString_NoSemiColon = "variable v Select v";
    std::string inputString_WithTabs = "variable \tv; Sel\tect v";
    std::string inputString_EndSemiColon = "variable v; assign a;";
    std::string inputString_MoreThanOneSemiColon =
        "variable v; assign a; Select v";

    QpsTokenizer Tokenizer;

    std::vector<std::string> resultsList;

    resultsList = Tokenizer.tokenize(inputString_NormalQuery);
    REQUIRE(expectedResultList_NormalQuery == resultsList);

    resultsList = Tokenizer.tokenize(inputString_NoSemiColon);
    REQUIRE(expectedResultList_NoSemiColon == resultsList);

    resultsList = Tokenizer.tokenize(inputString_WithTabs);
    REQUIRE(expectedResultList_WithTabs == resultsList);

    resultsList = Tokenizer.tokenize(inputString_EndSemiColon);
    REQUIRE(expectedResultList_EndSemiColon == resultsList);

    resultsList = Tokenizer.tokenize(inputString_MoreThanOneSemiColon);
    REQUIRE(expectedResultList_MoreThanOneSemiColon == resultsList);
}
