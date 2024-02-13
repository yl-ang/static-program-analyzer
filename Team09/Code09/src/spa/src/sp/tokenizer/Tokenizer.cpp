#include "Tokenizer.h"

/*
Takes in the source code as a vector of strings and turns into a vector of
Tokens.
*/
std::vector<Token> SpTokenizer::tokenize(std::vector<std::string> input) {
  std::vector<BasicToken*> basicTokens = lexicalAnalyzer.preprocess(input);
  std::vector<Token> tokens = assignTokens(basicTokens);
  return tokens;
}

/*
Assign specific token types and statement numbers to each token.
Procedure definitions, else keywords on a line and close brackets are assigned
statement number -1.
*/
std::vector<Token> SpTokenizer::assignTokens(std::vector<BasicToken*> input) {
  std::vector<Token> tokens;
  for (size_t i = 0; i < input.size(); i++) {
    BasicToken* curr = input[i];
    BasicToken* next = (i + 1 < input.size()) ? input[i + 1] : nullptr;
    switch (curr->type) {
      case BASIC_TOKEN_TYPE::_INTEGER:
        tokens.push_back(
            Token(LEXICAL_TOKEN_TYPE::INTEGER, curr->value, stmtNum));
        break;
      case BASIC_TOKEN_TYPE::KEYWORD:
        // Assign specific keyword type
        tokens.push_back(assignKeyword(curr));
        break;

      case BASIC_TOKEN_TYPE::_NAME:
        // For assign statements, check next token is "="
        if (next != nullptr && next->value == "=") {
          stmtNum++;
        }
        if (noStmtNumCountdown) {
          // proc_name
          noStmtNumCountdown--;
          tokens.push_back(
              Token(LEXICAL_TOKEN_TYPE::NAME, curr->value, NO_STMT_NUM));
        } else {
          tokens.push_back(
              Token(LEXICAL_TOKEN_TYPE::NAME, curr->value, stmtNum));
        }
        break;

      case BASIC_TOKEN_TYPE::SYMBOL:
        // Assign specific symbol type
        tokens.push_back(assignSymbol(curr));
        break;
    }
  }
  return tokens;
}

Token SpTokenizer::assignKeyword(BasicToken* bt) {
  if (bt->value == "while") {
    stmtNum++;
    return Token(LEXICAL_TOKEN_TYPE::WHILE, bt->value, stmtNum);

  } else if (bt->value == "read") {
    stmtNum++;
    return Token(LEXICAL_TOKEN_TYPE::READ, bt->value, stmtNum);

  } else if (bt->value == "procedure") {
    // proc_name { after procedure should not have statement numbers
    noStmtNumCountdown = 2;
    return Token(LEXICAL_TOKEN_TYPE::PROCEDURE, bt->value, NO_STMT_NUM);

  } else if (bt->value == "print") {
    stmtNum++;
    return Token(LEXICAL_TOKEN_TYPE::PRINT, bt->value, stmtNum);

  } else if (bt->value == "if") {
    stmtNum++;
    return Token(LEXICAL_TOKEN_TYPE::IF, bt->value, stmtNum);

  } else if (bt->value == "then") {
    return Token(LEXICAL_TOKEN_TYPE::THEN, bt->value, stmtNum);

  } else if (bt->value == "else") {
    // { after else should not have statement number
    noStmtNumCountdown = 1;
    return Token(LEXICAL_TOKEN_TYPE::ELSE, bt->value, NO_STMT_NUM);

  } else {
    throw SyntaxError("Invalid keyword!");
  }
}

Token SpTokenizer::assignSymbol(BasicToken* bt) {
  if (bt->value == ";") {
    return Token(LEXICAL_TOKEN_TYPE::SEMICOLON, bt->value, stmtNum);

  } else if (bt->value == "{") {
    if (noStmtNumCountdown) {
      noStmtNumCountdown--;
      return Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, bt->value,
                   NO_STMT_NUM);

    } else {
      return Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, bt->value, stmtNum);
    }

  } else if (bt->value == "}") {
    return Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, bt->value, NO_STMT_NUM);

  } else if (bt->value == "(") {
    return Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, bt->value, stmtNum);

  } else if (bt->value == ")") {
    return Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, bt->value, stmtNum);

  } else if (bt->value == "=") {
    return Token(LEXICAL_TOKEN_TYPE::EQUAL, bt->value, stmtNum);

  } else if (bt->value == "!") {
    return Token(LEXICAL_TOKEN_TYPE::NOT, bt->value, stmtNum);

  } else if (bt->value == "&&") {
    return Token(LEXICAL_TOKEN_TYPE::ANDAND, bt->value, stmtNum);

  } else if (bt->value == "||") {
    return Token(LEXICAL_TOKEN_TYPE::OR, bt->value, stmtNum);

  } else if (bt->value == "<") {
    return Token(LEXICAL_TOKEN_TYPE::LESS_THAN, bt->value, stmtNum);

  } else if (bt->value == "<=") {
    return Token(LEXICAL_TOKEN_TYPE::LESS_THAN_OR_EQUAL_TO, bt->value, stmtNum);

  } else if (bt->value == ">") {
    return Token(LEXICAL_TOKEN_TYPE::GREATER_THAN, bt->value, stmtNum);

  } else if (bt->value == ">=") {
    return Token(LEXICAL_TOKEN_TYPE::GREATER_THAN_OR_EQUAL_TO, bt->value,
                 stmtNum);

  } else if (bt->value == "==") {
    return Token(LEXICAL_TOKEN_TYPE::EQUAL_CHECK, bt->value, stmtNum);

  } else if (bt->value == "!=") {
    return Token(LEXICAL_TOKEN_TYPE::NOT_EQUAL_CHECK, bt->value, stmtNum);

  } else if (bt->value == "+") {
    return Token(LEXICAL_TOKEN_TYPE::ADD, bt->value, stmtNum);

  } else if (bt->value == "-") {
    return Token(LEXICAL_TOKEN_TYPE::SUB, bt->value, stmtNum);

  } else if (bt->value == "*") {
    return Token(LEXICAL_TOKEN_TYPE::MUL, bt->value, stmtNum);

  } else if (bt->value == "/") {
    return Token(LEXICAL_TOKEN_TYPE::DIV, bt->value, stmtNum);

  } else if (bt->value == "%") {
    return Token(LEXICAL_TOKEN_TYPE::MOD, bt->value, stmtNum);

  } else {
    throw SyntaxError("Invalid symbol!");
  }
}
