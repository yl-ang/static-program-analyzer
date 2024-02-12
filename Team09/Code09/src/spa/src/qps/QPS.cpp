#include "QPS.h"

std::vector<std::string> QPS::processQueries(std::string inputString) {
    try {
        std::string tokenizedInput = tokenizer.tokenize(inputString);
        ParsedQuery parsedInput = parser.parse(tokenizedInput);
    }
    catch (Exception e) {
        return std::vector<std::string> {e.getMessage()};
    }
}
