#pragma once
#include <string>
#include <ostream>

enum LEXICAL_TOKEN_TYPE {
  LETTER,
  DIGIT,
  NZDIGIT,
  PROCEDURE,
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

    // Overloading the equality operator (==)
    bool operator==(const Token& other) const {
        return (value == other.value && type == other.type && line_number == other.line_number);
    }

    friend std::ostream& operator<<(std::ostream& os, const Token& obj) {
            os << "{ \"value\": \"" << obj.value << "\", \n"
            << "\"type\": \"" << obj.type << "\", \n" 
            << "{ \"line_number\": \"" << obj.line_number;
            return os;
    }
};

