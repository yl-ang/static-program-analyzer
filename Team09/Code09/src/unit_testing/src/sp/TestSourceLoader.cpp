#include <iostream>  // std::cout
#include <sstream>   // std::stringstream, std::stringbuf
#include <string>    // std::string

#include "catch.hpp"
#include "sp/SourceLoader.h"

using namespace std;  // NOLINT

TEST_CASE("Source Loader Tests") {
  SourceLoader sourceLoader;

  SECTION("strips single spaces correctly") {
    std::vector<std::string> expectedResultList = {"procedure", "a", "{", "k",
                                                   "=",         "1", ";", "}"};
    std::string inputString = "procedure a { \n k = 1; \n }";
    std::stringstream ss;
    ss.str(inputString);
    REQUIRE(expectedResultList == sourceLoader.loadSimple(ss));
  }

  SECTION("strips multiple spaces correctly") {
    std::vector<std::string> expectedResultList = {"procedure", "a", "{", "k",
                                                   "=",         "1", ";", "}"};
    std::string inputString = "procedure a { \n k       =         1; \n }";
    std::stringstream ss;
    ss.str(inputString);
    REQUIRE(expectedResultList == sourceLoader.loadSimple(ss));
  }

  SECTION("strips multiple lines correctly") {
    std::vector<std::string> expectedResultList = {
        "procedure", "a", "{", "k", "=", "1", ";", "a", "=", "2", ";", "}"};
    std::string inputString = "procedure a { \n k = 1; \n a = 2; \n  }";
    std::stringstream ss;
    ss.str(inputString);
    REQUIRE(expectedResultList == sourceLoader.loadSimple(ss));
  }

  SECTION("loads single line procedure correctly") {
    std::vector<std::string> expectedResultList = {"procedure", "a", "{", "k",
                                                   "=",         "1", ";", "}"};
    std::string inputString = "procedure a {  k = 1; }";
    std::stringstream ss;
    ss.str(inputString);
    REQUIRE(expectedResultList == sourceLoader.loadSimple(ss));
  }

  SECTION("loads un-spaced operations correctly") {
    std::vector<std::string> expectedResultList = {"procedure", "a", "{", "k",
                                                   "=",         "1", ";", "}"};
    std::string inputString = "procedure a {  \n k=1; \n }";
    std::stringstream ss;
    ss.str(inputString);
    REQUIRE(expectedResultList == sourceLoader.loadSimple(ss));
  }
}
