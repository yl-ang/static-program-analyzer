#include "SpTokenizer.h"

std::vector<Token> SpTokenizer::tokenize(std::vector<std::string> input) {
    std::vector<BasicToken*> basicTokens = lexicalAnalyzer.preprocess(input);
    std::vector<Token> tokens = assignTokens(basicTokens);
    syntaxValidator.validateSyntax(tokens);
    return tokens;
}

std::vector<Token> SpTokenizer::tokenizeForPattern(std::vector<std::string> input) {
    std::vector<BasicToken*> basicTokens = lexicalAnalyzer.preprocess(input);
    std::vector<Token> tokens = assignTokens(basicTokens);
    return tokens;
}

std::vector<Token> SpTokenizer::tokenizeWithoutValidation(std::vector<std::string> input) {
    std::vector<BasicToken*> basicTokens = lexicalAnalyzer.preprocess(input);
    std::vector<Token> tokens = assignTokens(basicTokens);
    return tokens;
}

std::vector<Token> SpTokenizer::assignTokens(std::vector<BasicToken*> input) {
    std::vector<Token> tokens;
    for (size_t i = 0; i < input.size(); i++) {
        BasicToken* curr = input[i];
        BasicToken* next = (i + 1 < input.size()) ? input[i + 1] : nullptr;
        switch (curr->type) {
        case BASIC_TOKEN_TYPE::_INTEGER:
            tokens.push_back(Token(LEXICAL_TOKEN_TYPE::INTEGER, curr->value, stmtNumCounter));
            break;
        case BASIC_TOKEN_TYPE::KEYWORD:
            // Assign specific keyword type
            tokens.push_back(assignKeyword(curr));
            break;
        case BASIC_TOKEN_TYPE::SYMBOL:
            // Assign specific symbol type
            tokens.push_back(assignSymbol(curr));
            break;
        case BASIC_TOKEN_TYPE::_NAME:
            if (noStmtNumCounter) {  // 'proc_name's have no statement number
                noStmtNumCounter--;
                tokens.push_back(Token(LEXICAL_TOKEN_TYPE::NAME, curr->value, NO_STMT_NUM));
                break;
            }
            // For assign statements, check next token is "="
            if (next != nullptr && next->value == "=") {
                stmtNumCounter++;
            }
            tokens.push_back(Token(LEXICAL_TOKEN_TYPE::NAME, curr->value, stmtNumCounter));
            break;
        }
    }
    return tokens;
}

Token SpTokenizer::assignKeyword(BasicToken* bt) {
    if (bt->value == "while") {
        stmtNumCounter++;
        return Token(LEXICAL_TOKEN_TYPE::WHILE, bt->value, stmtNumCounter);

    } else if (bt->value == "read") {
        stmtNumCounter++;
        return Token(LEXICAL_TOKEN_TYPE::READ, bt->value, stmtNumCounter);

    } else if (bt->value == "procedure") {
        // 'proc_name' and '{' after 'procedure' should not have statement
        // numbers
        noStmtNumCounter = 2;
        return Token(LEXICAL_TOKEN_TYPE::PROCEDURE, bt->value, NO_STMT_NUM);

    } else if (bt->value == "print") {
        stmtNumCounter++;
        return Token(LEXICAL_TOKEN_TYPE::PRINT, bt->value, stmtNumCounter);

    } else if (bt->value == "if") {
        stmtNumCounter++;
        return Token(LEXICAL_TOKEN_TYPE::IF, bt->value, stmtNumCounter);

    } else if (bt->value == "then") {
        return Token(LEXICAL_TOKEN_TYPE::THEN, bt->value, stmtNumCounter);

    } else if (bt->value == "else") {
        // '{' after 'else' should not have statement number
        noStmtNumCounter = 1;
        return Token(LEXICAL_TOKEN_TYPE::ELSE, bt->value, NO_STMT_NUM);

    } else if (bt->value == "call") {
        stmtNumCounter++;
        return Token(LEXICAL_TOKEN_TYPE::CALL, bt->value, stmtNumCounter);

    } else {
        throw SyntaxError("Invalid keyword!");
    }
}

Token SpTokenizer::assignSymbol(BasicToken* bt) {
    if (bt->value == ";") {
        return Token(LEXICAL_TOKEN_TYPE::SEMICOLON, bt->value, stmtNumCounter);

    } else if (bt->value == "{") {
        if (noStmtNumCounter) {  // { after 'else' or 'proc_name'
            noStmtNumCounter--;
            return Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, bt->value, NO_STMT_NUM);
        }
        return Token(LEXICAL_TOKEN_TYPE::OPEN_CURLY_BRACE, bt->value, stmtNumCounter);

    } else if (bt->value == "}") {
        // All '}'s have no statement number
        return Token(LEXICAL_TOKEN_TYPE::CLOSE_CURLY_BRACE, bt->value, NO_STMT_NUM);

    } else if (bt->value == "(") {
        return Token(LEXICAL_TOKEN_TYPE::OPEN_BRACKET, bt->value, stmtNumCounter);

    } else if (bt->value == ")") {
        return Token(LEXICAL_TOKEN_TYPE::CLOSE_BRACKET, bt->value, stmtNumCounter);

    } else if (bt->value == "=") {
        return Token(LEXICAL_TOKEN_TYPE::EQUAL, bt->value, stmtNumCounter);

    } else if (bt->value == "!") {
        return Token(LEXICAL_TOKEN_TYPE::NOT, bt->value, stmtNumCounter);

    } else if (bt->value == "&&") {
        return Token(LEXICAL_TOKEN_TYPE::ANDAND, bt->value, stmtNumCounter);

    } else if (bt->value == "||") {
        return Token(LEXICAL_TOKEN_TYPE::OR, bt->value, stmtNumCounter);

    } else if (bt->value == "<") {
        return Token(LEXICAL_TOKEN_TYPE::LESS_THAN, bt->value, stmtNumCounter);

    } else if (bt->value == "<=") {
        return Token(LEXICAL_TOKEN_TYPE::LESS_THAN_OR_EQUAL_TO, bt->value, stmtNumCounter);

    } else if (bt->value == ">") {
        return Token(LEXICAL_TOKEN_TYPE::GREATER_THAN, bt->value, stmtNumCounter);

    } else if (bt->value == ">=") {
        return Token(LEXICAL_TOKEN_TYPE::GREATER_THAN_OR_EQUAL_TO, bt->value, stmtNumCounter);

    } else if (bt->value == "==") {
        return Token(LEXICAL_TOKEN_TYPE::EQUAL_CHECK, bt->value, stmtNumCounter);

    } else if (bt->value == "!=") {
        return Token(LEXICAL_TOKEN_TYPE::NOT_EQUAL_CHECK, bt->value, stmtNumCounter);

    } else if (bt->value == "+") {
        return Token(LEXICAL_TOKEN_TYPE::ADD, bt->value, stmtNumCounter);

    } else if (bt->value == "-") {
        return Token(LEXICAL_TOKEN_TYPE::SUB, bt->value, stmtNumCounter);

    } else if (bt->value == "*") {
        return Token(LEXICAL_TOKEN_TYPE::MUL, bt->value, stmtNumCounter);

    } else if (bt->value == "/") {
        return Token(LEXICAL_TOKEN_TYPE::DIV, bt->value, stmtNumCounter);

    } else if (bt->value == "%") {
        return Token(LEXICAL_TOKEN_TYPE::MOD, bt->value, stmtNumCounter);

    } else {
        throw SyntaxError("Invalid symbol!");
    }
}
