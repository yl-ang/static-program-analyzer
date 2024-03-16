#include <string>
#include <vector>

#include "ArgumentsValidator/ArgumentsValidator.h"
#include "ArgumentsValidator/FollowsValidator.h"
#include "ArgumentsValidator/ModifiesValidator.h"
#include "ArgumentsValidator/NextValidator.h"
#include "ArgumentsValidator/ParentValidator.h"
#include "ArgumentsValidator/PatternValidator.h"
#include "ArgumentsValidator/UsesValidator.h"
#include "ValidatorSynonymStore.h"
#include "qps/GrammarUtils.h"
#include "qps/ParserUtils.h"
#include "qps/QPSConstants.h"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"

class Validator {
public:
    void validate(std::vector<std::string>);

protected:
    ValidatorSynonymStore synonymStore = {};

    void validateDeclarationStatement(const std::string& statement);
    void validateSelectStatement(const std::string& statement);
    void validateResultClause(const std::string& resultClause);

    void validateSuchThatClause(const std::string& suchThatClause);
    void validateRelRef(const std::string& relRef);

    void validatePatternClause(const std::string& patternClause);
    void validatePattern(const std::string& pattern);

    std::unique_ptr<ArgumentsValidator> buildArgValidator(const std::string& relRefString,
                                                          const std::vector<std::string>& arguments);
    std::unique_ptr<ArgumentsValidator> buildPatternValidator(const std::string& relRefString,
                                                              const std::vector<std::string>& arguments);
};
