#include <string>
#include <vector>

#include "../GrammarUtils.h"
#include "../ParserUtils.h"
#include "../exceptions/QPSSemanticError.h"
#include "../exceptions/QPSSyntaxError.h"
#include "SynonymStore.h"

class Validator {
public:
    void validate(std::vector<std::string>);

protected:
    SynonymStore synonymStore = {};

    bool isValidSelectStatement(std::string);
    bool isValidDeclarationStatement(std::string);

    bool isValidSuchThatClause(std::string);
    bool isValidPatternClause(std::string);
};
