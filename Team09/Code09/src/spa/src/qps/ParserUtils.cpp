#include "ParserUtils.h"

std::string replaceAllWhitespaces(std::string str) {
    std::string replacedString;

    std::regex whitespacePattern = std::regex("[" + WHITESPACES + "]");
    std::regex consecutiveSpacePattern = std::regex(SPACE + "+");

    replacedString = std::regex_replace(str, whitespacePattern, SPACE);
    replacedString = std::regex_replace(replacedString, consecutiveSpacePattern, SPACE);

    return replacedString;
}

std::vector<std::string> splitByDelimiter(const std::string str, const std::string delimiter) {
    size_t nextDelimiterIndex = str.find(delimiter);
    size_t offset = 0;

    std::string substring;
    size_t substringSize;

    std::vector<std::string> splitList;
    while (nextDelimiterIndex != std::string::npos) {
        substringSize = nextDelimiterIndex - offset;
        substring = str.substr(offset, substringSize);
        splitList.push_back(trim(substring));

        offset = nextDelimiterIndex + 1;
        nextDelimiterIndex = str.find(delimiter, offset);
    }

    if (offset != str.length()) {
        substringSize = str.length() - offset;
        substring = str.substr(offset, substringSize);
        splitList.push_back(trim(substring));
    }

    return splitList;
}

std::vector<std::string> stringToWordList(std::string string) {
    std::vector<std::string> wordList;
    std::stringstream ss(string);
    std::string word;
    while (ss >> word) {
        wordList.push_back(word);
    }
    return wordList;
}

std::string trim(const std::string str) {
    size_t start = str.find_first_not_of(WHITESPACES);
    std::string trimmedString;
    if (start != std::string::npos) {
        trimmedString = str.substr(start);
    }

    size_t end = trimmedString.find_last_not_of(WHITESPACES);
    if (end != std::string::npos) {
        trimmedString = trimmedString.substr(0, end + 1);
    }

    return trimmedString;
}

// str = "such that Follows(1, 2) pattern a("_", "_")"
// return {0, 24}
std::vector<std::string> getAllClauses(const std::string& str) {
    std::vector<std::string> clauseStarts = {"such that", "pattern"};

    std::vector<size_t> allClausesIndices = {};
    for (std::string clauseString : clauseStarts) {
        std::vector<size_t> clauseIndices = getClauseIndices(str, clauseString);
        allClausesIndices.insert(std::end(allClausesIndices), std::begin(clauseIndices), std::end(clauseIndices));
    }

    if (allClausesIndices.size() == 0) {
        return {};
    }

    std::sort(allClausesIndices.begin(), allClausesIndices.end());

    std::vector<std::string> clauses = {};
    std::string clause;
    for (int i = 0; i < allClausesIndices.size() - 1; i++) {
        clause = str.substr(allClausesIndices.at(i), allClausesIndices.at(i + 1));
        clauses.push_back(clause);
    }
    clause = str.substr(allClausesIndices.at(allClausesIndices.size() - 1), std::string::npos);
    clauses.push_back(clause);

    return clauses;
}

std::vector<size_t> getClauseIndices(const std::string& str, std::string clause) {
    size_t offset = 0;
    size_t clauseIndex = str.find(clause, offset);

    // no such clause found
    if (clauseIndex == std::string::npos) {
        return {};
    }

    std::vector<size_t> clausePositionList = {};
    while (clauseIndex != std::string::npos) {
        clausePositionList.push_back(clauseIndex);

        offset = clauseIndex + 1;
        clauseIndex = str.find(clause, offset);
    }

    return clausePositionList;
}
