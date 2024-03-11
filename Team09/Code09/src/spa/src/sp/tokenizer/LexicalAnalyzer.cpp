#include "LexicalAnalyzer.h"

std::vector<BasicToken*> LexicalAnalyzer::preprocess(std::vector<std::string> strings) {
    std::vector<BasicToken*> bt;
    for (size_t i = 0; i < strings.size(); ++i) {
        std::string current = strings[i];
        std::string prev = (i > 0) ? strings[i - 1] : "";
        std::string next = (i < strings.size() - 1) ? strings[i + 1] : "";
        bt.push_back(assignType(current, prev, next));
    }
    return bt;
}

BasicToken* LexicalAnalyzer::assignType(std::string curr, std::string prev, std::string next) {
    // Integer
    if (std::all_of(curr.begin(), curr.end(), ::isdigit)) {
        if (!isValidInteger(curr)) {
            throw SyntaxError("Invalid integer!");
        }
        return new BasicToken(curr, BASIC_TOKEN_TYPE::_INTEGER);

        // Symbol
    } else if (std::all_of(curr.begin(), curr.end(), ::ispunct)) {
        if (!isValidSymbol(curr)) {
            throw SyntaxError("Invalid symbol!");
        }
        return new BasicToken(curr, BASIC_TOKEN_TYPE::SYMBOL);

        // Name
    } else if (std::all_of(curr.begin(), curr.end(), ::isalnum)) {
        if (!isValidName(curr)) {
            throw SyntaxError("Invalid name!");
        }
        // Keyword or Name
        if (std::all_of(curr.begin(), curr.end(), ::isalpha)) {
            return disambiguate(curr, prev, next);
        }
        return new BasicToken(curr, BASIC_TOKEN_TYPE::_NAME);

    } else {
        throw SyntaxError("Invalid token!");
    }
}

BasicToken* LexicalAnalyzer::disambiguate(std::string curr, std::string prev, std::string next) {
    bool isKeyword = false;
    if (KEYWORDS.find(curr) != KEYWORDS.end()) {
        // Keyword match, disambiguate by checking neighbours
        if (curr == "if" || curr == "while") {
            if (next == "(") {
                isKeyword = true;
            }
        } else if (curr == "then") {
            if (prev == ")" && next == "{") {
                isKeyword = true;
            }
        } else if (curr == "else") {
            if (prev == "}" && next == "{") {
                isKeyword = true;
            }
        } else if (curr == "procedure" || curr == "read" || curr == "print" || curr == "call") {
            if (isValidName(next)) {
                isKeyword = true;
            }
        } else {
            // Shouldn't reach here
            throw SyntaxError("Error disambiguating token!");
        }
    }
    if (isKeyword) {
        return new BasicToken(curr, BASIC_TOKEN_TYPE::KEYWORD);
    }
    // Keyword not matched, assign type NAME
    return new BasicToken(curr, BASIC_TOKEN_TYPE::_NAME);
}

bool LexicalAnalyzer::isValidInteger(std::string value) {
    // INTEGER : 0 | NZDIGIT ( DIGIT )*
    std::regex integerRegex("(0|[1-9][0-9]*)");
    return std::regex_match(value, integerRegex);
}

bool LexicalAnalyzer::isValidName(std::string value) {
    // NAME: LETTER (LETTER | DIGIT)*
    std::regex nameRegex("[a-zA-Z][a-zA-Z0-9]*");
    return std::regex_match(value, nameRegex);
}

bool LexicalAnalyzer::isValidSymbol(std::string value) {
    // SYMBOL: + | - | * | / | % | < | > | && | || | != | == | = | >= | <= | ! |
    // { | } | ( | ) | ;
    std::regex symbolRegex(R"(\+|\-|\*|\/|\%|<|>|&&|\|\||!=|==|=|>=|<=|!|\{|\}|\(|\)|;)");
    return std::regex_match(value, symbolRegex);
}
