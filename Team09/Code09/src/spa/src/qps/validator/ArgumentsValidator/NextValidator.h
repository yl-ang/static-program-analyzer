#include <string>
#include <vector>

#include "ArgumentsValidator.h"
#include "qps/GrammarUtils.h"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"

class NextValidator : public ArgumentsValidator {
public:
    NextValidator(const std::vector<std::string>& args);
    NextValidator() = default;
    virtual void validateSyntax() override;
    virtual void validateSemantic(SynonymStore* store) override;
};
