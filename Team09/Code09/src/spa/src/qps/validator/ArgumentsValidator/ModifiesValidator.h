#include <string>
#include <vector>

#include "ArgumentsValidator.h"
#include "qps/GrammarUtils.h"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"

class ModifiesValidator : public ArgumentsValidator {
public:
    ModifiesValidator(const std::vector<std::string>& args);
    ModifiesValidator() = default;
    virtual void validateSyntax() override;
    virtual void validateSemantic(ValidatorSynonymStore* store) override;
};
