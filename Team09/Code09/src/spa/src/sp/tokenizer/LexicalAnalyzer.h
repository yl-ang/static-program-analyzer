#pragma once

#include <regex>
#include <string>
#include <vector>

#include "../exceptions/SyntaxError.h"
#include "BasicToken.h"

/**
 * @brief Does preliminary syntax checks for disallowed lexical tokens and
 * assigns intermediate types to each string before tokenizing. Mainly to
 * disambiguate between strings that have the same value but different type such
 * as the keyword 'procedure' and the var_name 'procedure'.
 *
 */
class LexicalAnalyzer {
private:
    static bool isValidInteger(std::string value);
    static bool isValidName(std::string value);
    static bool isValidSymbol(std::string value);

public:
    /**
     * @brief Driver for the lexical analyzer.
     */
    std::vector<BasicToken> preprocess(std::vector<std::string> strings);

    /**
     * @brief Assigns intermediate types to tokens (symbol / name/ integer/ keyword).
     */
    static BasicToken assignType(std::string curr, std::string prev, std::string next);

    /**
     * @brief Disambiguates between names and keywords.
     */
    static BasicToken disambiguate(std::string curr, std::string prev, std::string next);
};
