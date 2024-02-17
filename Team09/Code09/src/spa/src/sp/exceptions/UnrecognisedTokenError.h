#include <string>

#include "SyntaxError.h"
#include "Utils.h"
#include "sp/tokenizer/Token.h"

class UnrecognisedTokenError : public SyntaxError {
public:
    explicit UnrecognisedTokenError(LEXICAL_TOKEN_TYPE unrecognised)
        : SyntaxError(getErrorMessage(unrecognised)) {}

protected:
    std::string getErrorMessage(LEXICAL_TOKEN_TYPE unrecognised) {
        return "Unregonised token: " + getLexicalEnumString(unrecognised);
    }
};
