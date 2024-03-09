#include <string>
#include <vector>

#include "ArgumentsValidator.h"
#include "qps/GrammarUtils.h"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"

class UsesValidator : public ArgumentsValidator {
public:
    UsesValidator(const std::vector<std::string>& args);
    UsesValidator() = default;
    virtual void validateSyntax() override;
    virtual void validateSemantic(SynonymStore* store) override;
};
