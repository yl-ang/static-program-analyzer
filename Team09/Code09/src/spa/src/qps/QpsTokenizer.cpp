#include "QpsTokenizer.h"

#include "ParserUtils.h"

std::vector<std::string> QpsTokenizer::tokenize(std::string inputQueryString) {
    std::string cleanInputString = replaceAllExtraWhitespaces(inputQueryString);

    std::vector<std::string> inputQueryTokens = {};
    std::string delimiter = ";";

    size_t nextDelimiterIndex = cleanInputString.find(delimiter);
    size_t offset = 0;
    size_t substringSize;
    std::string newString;

    while (nextDelimiterIndex != std::string::npos) {
        substringSize = nextDelimiterIndex - offset + 1;
        newString = cleanInputString.substr(offset, substringSize);
        inputQueryTokens.push_back(trim(newString));

        offset = nextDelimiterIndex + 1;
        nextDelimiterIndex = cleanInputString.find(delimiter, offset);
    }

    if (offset != cleanInputString.length()) {
        substringSize = cleanInputString.length() - offset + 1;
        newString = cleanInputString.substr(offset, substringSize);
        inputQueryTokens.push_back(trim(newString));
    }

    return inputQueryTokens;
}
