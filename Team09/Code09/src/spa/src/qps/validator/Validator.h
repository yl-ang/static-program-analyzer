#include <string>
#include <vector>

#include "../exceptions/*"
#include "../GrammarUtils.h"
#include "../ParserUtils.h"

class Validator {
public:
    void validate(std::vector<std::string>);

private:
    bool isValidSelectStatement(std::string);
    bool isValidDeclarationStatement(std::string);

    bool isValidSuchThatClause(std::string);
    bool isValidPatternClause(std::string);
};
