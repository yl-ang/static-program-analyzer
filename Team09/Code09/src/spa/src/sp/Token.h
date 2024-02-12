#pragma once
#include <string>

enum LEXICAL_TOKEN_TYPE {
  LETTER,
  DIGIT,
  NZDIGIT,
  NAME,
  INTEGER,
  WHILE,
  READ,
  PRINT,
  CALL,
  IF,
  SEMICOLON,
  OPEN_CURLY_BRACE,
  CLOSE_CURLY_BRACE,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  THEN,
  ELSE,
  EQUAL,
  NOT,
  ANDAND,
  OR,
  LESS_THAN,
  LESS_THAN_OR_EQUAL_TO,
  GREATER_THAN,
  GREATER_THAN_OR_EQUAL_TO,
  EQUAL_CHECK,
  NOT_EQUAL_CHECK,
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  PROC,
};

class Token {
 public:
  LEXICAL_TOKEN_TYPE type;
  std::string value;
  int line_number;

  Token(LEXICAL_TOKEN_TYPE token_type, std::string token_value,
        int token_line_number) {
    type = token_type;
    value = token_value;
    line_number = token_line_number;
  }
};
