#pragma once

#include <string>
#include <vector>
#include "Token.h"
#include "BasicToken.h"
#include "LexicalAnalyzer.h"
#include "../exceptions/SyntaxError.h"


class Tokenizer {
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
