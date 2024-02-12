#include <iostream>

#include "catch.hpp"
#include "sp/Token.h"
#include "sp/ast/Ast.h"

using namespace std;  // NOLINT

void printVector(const std::vector<std::string>& vec) {
  std::cout << "[ ";
  for (const auto& element : vec) {
    std::cout << element << " ";
  }
  std::cout << "]" << std::endl;
}

std::vector<std::vector<std::string>> createStringArray(
    std::vector<std::vector<Token>> nested_tokens) {
  std::vector<std::vector<std::string>> result = {};
  for (auto token_array : nested_tokens) {
    std::vector<std::string> tmp = {};
    for (auto token : token_array) {
      tmp.push_back(token.value);
    }
    result.push_back(tmp);
  }
  return result;
}

TEST_CASE("AST Utils Tests") {
  AST ast;

  SECTION("Splits multiple procedures correctly") {
    // we are not interested in the types here so we just use a dummy type
    // similarly for line number.
    std::vector<Token> inputTokenArray = {
        Token(LEXICAL_TOKEN_TYPE::PROC, "procedure", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
        Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
        Token(LEXICAL_TOKEN_TYPE::LETTER, "k", 1),
        Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
        Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 1),
        Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
        Token(LEXICAL_TOKEN_TYPE::PROC, "procedure", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "b", 0),
        Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
        Token(LEXICAL_TOKEN_TYPE::LETTER, "c", 1),
        Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
        Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 1),
        Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1)};
    std::vector<std::vector<std::string>> expectedResultList = {
        {"procedure", "a", "{", "k", "=", "1", ";", "}"},
        {"procedure", "b", "{", "c", "=", "1", ";", "}"}};

    std::vector<std::vector<std::string>> result =
        createStringArray(ast.splitByProcedure(inputTokenArray));
    // verify that the length is correct first
    REQUIRE(expectedResultList.size() == result.size());
    // verify content of tokens
    for (int i = 0; i < expectedResultList.size(); i++) {
      // verify internal lengths are the same
      REQUIRE(expectedResultList[i].size() == result[i].size());
      for (int j = 0; j < expectedResultList[i].size(); j++) {
        REQUIRE(expectedResultList[i][j] == result[i][j]);
      }
    }
  }

  SECTION("Splits single procedures correctly") {
    // we are not interested in the types here so we just use a dummy type
    // similarly for line number.
    std::vector<Token> inputTokenArray = {
        Token(LEXICAL_TOKEN_TYPE::PROC, "procedure", 0),
        Token(LEXICAL_TOKEN_TYPE::NAME, "a", 0),
        Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, "{", 0),
        Token(LEXICAL_TOKEN_TYPE::LETTER, "k", 1),
        Token(LEXICAL_TOKEN_TYPE::EQUAL, "=", 1),
        Token(LEXICAL_TOKEN_TYPE::INTEGER, "1", 1),
        Token(LEXICAL_TOKEN_TYPE::SEMICOLON, ";", 1),
        Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, "}", 1),
    };
    std::vector<std::vector<std::string>> expectedResultList = {
        {"procedure", "a", "{", "k", "=", "1", ";", "}"},
    };

    std::vector<std::vector<std::string>> result =
        createStringArray(ast.splitByProcedure(inputTokenArray));
    // verify that the length is correct first
    REQUIRE(expectedResultList.size() == result.size());
    // verify content of tokens
    for (int i = 0; i < expectedResultList.size(); i++) {
      // verify internal lengths are the same
      REQUIRE(expectedResultList[i].size() == result[i].size());
      for (int j = 0; j < expectedResultList[i].size(); j++) {
        REQUIRE(expectedResultList[i][j] == result[i][j]);
      }
    }
  }
}
