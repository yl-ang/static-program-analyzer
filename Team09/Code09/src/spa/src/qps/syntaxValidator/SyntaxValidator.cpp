#include "SyntaxValidator.h"

#include <qps/GrammarUtils.h>
#include <qps/ParserUtils.h>

bool SyntaxValidator::isValidSelectStatement(std::string statement) {
    int firstSpaceIndex = statement.find_first_of(" \n\t\b\r\f");
    if (firstSpaceIndex == std::string::npos) return false;
    if (statement.back() != ';') {
        std::string firstWord = statement.substr(0, firstSpaceIndex + 1);  // Select
        std::string remainingStatement = statement.substr(firstSpaceIndex + 1, std::string::npos);
        if (!isSynonym(remainingStatement)) return false;
        return true;
    }
    return false;
}

bool SyntaxValidator::isValidDeclarationStatement(std::string statement) {
    if (statement.back() != ';') return false;
    statement.pop_back();

    int firstSpaceIndex = statement.find_first_of(" \n\t\b\r\f");
    if (firstSpaceIndex == std::string::npos) return false;
    std::string firstWord = statement.substr(0, firstSpaceIndex + 1);  // design-entity
    std::string remainingStatement = statement.substr(firstSpaceIndex + 1, std::string::npos);

    std::vector<std::string> synonymList = splitByDelimiter(trim(remainingStatement), ",");
    if (synonymList.size() == 0) return false;
    for (std::string synonym : synonymList) {
        if (!isSynonym(synonym)) {
            return false;
        }
    }
    return true;
}
