#include <string>
#include <vector>

#include "ArgumentsValidator/ArgumentsValidator.h"
#include "ArgumentsValidator/FollowsValidator.h"
#include "ArgumentsValidator/ModifiesValidator.h"
#include "ArgumentsValidator/ParentValidator.h"
#include "ArgumentsValidator/UsesValidator.h"
#include "SynonymStore.h"
#include "qps/GrammarUtils.h"
#include "qps/ParserUtils.h"
#include "qps/QPSConstants.h"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"

class Validator {
public:
    void validate(std::vector<std::string>);

protected:
    SynonymStore synonymStore = {};

    void validateDeclarationStatement(const std::string& statement);
    void validateSelectStatement(const std::string& statement);
    void validateReturnClause(const std::string& returnClause);

    void validateSuchThatClause(const std::string& suchThatClause);
    void validateRelRef(const std::string& relRef);
    ArgumentsValidator* buildArgValidator(const std::string& relRefString, const std::vector<std::string>& arguments);

    void validatePatternClause(const std::string& patternClause);
};
