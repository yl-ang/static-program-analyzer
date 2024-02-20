#pragma once
#include <string>

#include "SyntaxError.h"
#include "sp/Utils.h"
#include "sp/tokenizer/Token.h"

class LexicalSyntaxError : public SyntaxError {
public:
    LexicalSyntaxError(LEXICAL_TOKEN_TYPE expected, LEXICAL_TOKEN_TYPE received)
        : SyntaxError(getErrorMessage(expected, received)) {}

protected:
    std::string getErrorMessage(LEXICAL_TOKEN_TYPE expected,
                                LEXICAL_TOKEN_TYPE received) {
        return "Expected " + getLexicalEnumString(expected) +
               " but received: " + getLexicalEnumString(received);
    }
};
