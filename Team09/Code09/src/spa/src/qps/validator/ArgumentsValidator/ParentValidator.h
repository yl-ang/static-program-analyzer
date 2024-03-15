#include <string>
#include <vector>

#include "ArgumentsValidator.h"
#include "qps/GrammarUtils.h"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"

class ParentValidator : public ArgumentsValidator {
public:
    ParentValidator(const std::vector<std::string>& args);
    ParentValidator() = default;
    virtual void validateSyntax() override;
    virtual void validateSemantic(ValidatorSynonymStore* store) override;
};
