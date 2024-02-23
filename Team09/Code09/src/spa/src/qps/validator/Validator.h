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

    void validateSelectStatement(const std::string& statement);
    void validateDeclarationStatement(const std::string& statement);

    void validateSuchThatClause(const std::string& suchThatClause);
    void validatePatternClause(const std::string& patternClause);
};
