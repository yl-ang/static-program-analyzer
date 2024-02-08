#include <string>

#include "../ParserUtils.h"

class SyntaxValidator {
public:
    bool isValidSelectStatement(std::string);
    bool isValidDeclarationStatement(std::string);
};