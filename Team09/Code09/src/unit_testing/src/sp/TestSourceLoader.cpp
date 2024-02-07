#include "catch.hpp"
#include "sp/SourceLoader.h"

using namespace std;  // NOLINT

TEST_CASE("Source Loader Tests") {
  SourceLoader sourceLoader;

  SECTION("strips single spaces correctly") {
    std::vector<std::string> expectedResultList_NormalQuery = {
        "procedure", "a", "{", "\n", "k", "=", "1;", "\n", "}"};
    std::string inputString_NormalSimple = "procedure a { \n k = 1; \n }";
    REQUIRE(expectedResultList_NormalQuery ==
            sourceLoader.LoadSIMPLE(inputString_NormalSimple));
  }

  SECTION("strips multiple spaces correctly") {
    std::vector<std::string> expectedResultList_NormalQuery = {
        "procedure", "a", "{", "\n", "k", "=", "1;", "\n", "}"};
    std::string inputString_NormalSimple =
        "procedure a { \n k       =         1; \n }";
    REQUIRE(expectedResultList_NormalQuery ==
            sourceLoader.LoadSIMPLE(inputString_NormalSimple));
  }

  SECTION("strips multiple lines correctly") {
    std::vector<std::string> expectedResultList_NormalQuery = {
        "procedure", "a", "{", "\n", "k",  "=", "1;",
        "\n",        "a", "=", "2;", "\n", "}"};
    std::string inputString_NormalSimple =
        "procedure a { \n k = 1; \n a = 2; \n  }";
    REQUIRE(expectedResultList_NormalQuery ==
            sourceLoader.LoadSIMPLE(inputString_NormalSimple));
  }
}