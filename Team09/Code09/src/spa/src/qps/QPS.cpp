#include "QPS.h"

std::vector<std::string> QPS::processQueries(std::string inputString) {
    try {
        std::vector<std::string> tokenizedInput = tokenizer.tokenize(inputString);
        Query parsedInput = parser.parse(tokenizedInput);
        return parsedInput.evaluate(pkbReader);
    }
    catch (Exception e) {
        return std::vector<std::string> {e.getMessage()};
    }
}
