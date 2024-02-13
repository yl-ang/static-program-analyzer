#pragma once

#include <string>
#include <vector>

#include "../exceptions/SyntaxError.h"
#include "BasicToken.h"
#include "LexicalAnalyzer.h"
#include "Token.h"

class SpTokenizer {
 private:
  LexicalAnalyzer lexicalAnalyzer;
  int stmtNum = 0;
  const int NO_STMT_NUM = -1;
  int noStmtNumCountdown = 0;

 public:
  std::vector<Token> tokenize(std::vector<std::string>);
  std::vector<Token> assignTokens(std::vector<BasicToken*> input);
  Token assignKeyword(BasicToken* bt);
  Token assignSymbol(BasicToken* bt);
};
