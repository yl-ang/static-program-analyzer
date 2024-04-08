#pragma once
#include <string>
#include <unordered_map>

#include "sp/tokenizer/Token.h"
const char UNIDENTIFIED_LEXICAL_STRING[] = "unknown";
const std::unordered_map<LEXICAL_TOKEN_TYPE, std::string> LEXICAL_ENUM_STRINGS = {{LETTER, "letter"},
                                                                                  {DIGIT, "digit"},
                                                                                  {NZDIGIT, "nzdigit"},
                                                                                  {PROCEDURE, "procedure"},
                                                                                  {NAME, "name"},
                                                                                  {INTEGER, "integer"},
                                                                                  {WHILE, "while"},
                                                                                  {READ, "read"},
                                                                                  {PRINT, "print"},
                                                                                  {CALL, "call"},
                                                                                  {IF, "if"},
                                                                                  {SEMICOLON, ";"},
                                                                                  {OPEN_CURLY_BRACE, "{"},
                                                                                  {CLOSE_CURLY_BRACE, "}"},
                                                                                  {OPEN_BRACKET, "("},
                                                                                  {CLOSE_BRACKET, ")"},
                                                                                  {THEN, "then"},
                                                                                  {ELSE, "else"},
                                                                                  {EQUAL, "="},
                                                                                  {NOT, "!"},
                                                                                  {ANDAND, "&&"},
                                                                                  {OR, "||"},
                                                                                  {LESS_THAN, "<"},
                                                                                  {LESS_THAN_OR_EQUAL_TO, "<="},
                                                                                  {GREATER_THAN, ">"},
                                                                                  {GREATER_THAN_OR_EQUAL_TO, ">="},
                                                                                  {EQUAL_CHECK, "=="},
                                                                                  {NOT_EQUAL_CHECK, "!="},
                                                                                  {ADD, "add"},
                                                                                  {SUB, "sub"},
                                                                                  {MUL, "mul"},
                                                                                  {DIV, "div"},
                                                                                  {MOD, "mod"}};
std::string getLexicalEnumString(LEXICAL_TOKEN_TYPE type);
bool isExactMatch(std::string s1, std::string s2);
bool isPartialMatch(std::string s1, std::string s2);
