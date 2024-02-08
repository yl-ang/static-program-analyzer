#include "GrammarUtils.h"

bool isIdent(std::string str) {
    return std::regex_match(str, std::regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

bool isName(std::string str) {
    return isIdent(str);
}

bool isInteger(std::string str) {
    return std::regex_match(str, std::regex("^(?:0|[1-9][0-9]*)$"));
}

bool isSynonym(std::string str) {
    return isIdent(str);
}

bool isWildcard(std::string str) {
    return std::regex_match(str, std::regex("^_$"));
}

bool isQuotedIdent(std::string str) {
    return std::regex_match(str, std::regex("^\"[a-zA-Z][a-zA-Z0-9]*\"$"));
}

bool isStmtRef(std::string str) {
    return isSynonym(str) || isWildcard(str) || isInteger(str);
}

bool isEntRef(std::string str) {
    return isSynonym(str) || isWildcard(str) || isQuotedIdent(str);
}
