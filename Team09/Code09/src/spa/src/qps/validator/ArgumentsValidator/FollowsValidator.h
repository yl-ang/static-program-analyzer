#include <string>
#include <vector>

#include "ArgumentsValidator.h"
#include "qps/GrammarUtils.h"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"

class FollowsValidator : public ArgumentsValidator {
public:
    FollowsValidator(const std::vector<std::string>& args);
    FollowsValidator() = default;
    virtual void validateSyntax() override;
    virtual void validateSemantic(ValidatorSynonymStore* store) override;
};
