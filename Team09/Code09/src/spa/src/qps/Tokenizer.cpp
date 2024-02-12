#include "Tokenizer.h"

#include "ParserUtils.h"

std::vector<std::string> Tokenizer::tokenize(std::string inputQueryString) {
    std::vector<std::string> inputQueryTokens = { };
    std::string delimiter = ";";

    size_t nextDelimiterIndex = inputQueryString.find(delimiter);
    size_t offset = 0;
    size_t substringSize;
    std::string newString;

    while (nextDelimiterIndex != std::string::npos) {
        substringSize = nextDelimiterIndex - offset + 1;
        newString = inputQueryString.substr(offset, substringSize);
        inputQueryTokens.push_back(trim(newString));

        offset = nextDelimiterIndex + 1;
        nextDelimiterIndex = inputQueryString.find(delimiter, offset);
    }

    if (offset != inputQueryString.length()) {
        substringSize = inputQueryString.length() - offset + 1;
        newString = inputQueryString.substr(offset, substringSize);
        inputQueryTokens.push_back(trim(newString));
    }

    return inputQueryTokens;
}
