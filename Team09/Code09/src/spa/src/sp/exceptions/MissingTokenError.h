#pragma once

#include <string>

#include "SyntaxError.h"
#include "sp/Utils.h"
#include "sp/tokenizer/Token.h"

class MissingTokenError : public SyntaxError {
public:
    explicit MissingTokenError(LEXICAL_TOKEN_TYPE expected)
        : SyntaxError(getErrorMessage(expected)) {}

protected:
    std::string getErrorMessage(LEXICAL_TOKEN_TYPE expected) {
        return "Expected " + getLexicalEnumString(expected) + " not found.";
    }
};
