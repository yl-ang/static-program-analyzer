#include <iostream>
#include <sstream>

#include "catch.hpp"
#include "sp/SourceLoader.h"

using namespace std;  // NOLINT

TEST_CASE("Create Symbol string") {
  SourceLoader sourceLoader;

  SECTION("Valid single symbol string created successfully") {
    std::string inputString = "<";
    std::stringstream stringStream(inputString);

    REQUIRE(sourceLoader.createSymbolString(stringStream) == inputString);
  }

  SECTION("Invalid second symbol string returns first symbol string") {
    std::string inputString = "<@";
    std::stringstream stringStream(inputString);

    REQUIRE(sourceLoader.createSymbolString(stringStream) == "<");
  }

  SECTION("Valid double symbol string created successfully") {
    std::string inputString = "<=";
    std::stringstream stringStream(inputString);

    REQUIRE(sourceLoader.createSymbolString(stringStream) == inputString);
  }
}

TEST_CASE("Valid Character symbols") {
  SourceLoader sourceLoader;

  SECTION("Valid character symbol returns true") {
    char inputString = '<';
    REQUIRE(sourceLoader.isCharSymbol(inputString) == true);
  }

  SECTION("Invalid character symbol returns false") {
    char inputString = '@';
    REQUIRE(sourceLoader.isCharSymbol(inputString) == false);
  }
}

TEST_CASE("Valid Symbol pair") {
  SourceLoader sourceLoader;

  SECTION("Valid key and invalid value returns false") {
    char key = '<';
    char value = '@';
    REQUIRE(sourceLoader.isValidSymbolPair(key, value) == false);
  }

  SECTION("Valid key and invalid value returns true") {
    char key = '<';
    char value = '=';
    REQUIRE(sourceLoader.isValidSymbolPair(key, value) == true);
  }
  SECTION("Invalid key and invalid value returns false") {
    char key = '^';
    char value = '@';
    REQUIRE(sourceLoader.isValidSymbolPair(key, value) == false);
  }
  SECTION("Invalid key and valid value returns false") {
    char key = '@';
    char value = '=';
    REQUIRE(sourceLoader.isValidSymbolPair(key, value) == false);
  }
}
