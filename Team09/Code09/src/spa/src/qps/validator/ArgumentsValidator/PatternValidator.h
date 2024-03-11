#include <string>
#include <vector>

#include "ArgumentsValidator.h"
#include "qps/GrammarUtils.h"
#include "qps/exceptions/QPSSemanticError.h"
#include "qps/exceptions/QPSSyntaxError.h"

class PatternValidator : public ArgumentsValidator {
private:
    std::string syn;

public:
    PatternValidator(const std::string& syn, const std::vector<std::string>& args);
    virtual void validateSyntax() override;
    virtual void validateSemantic(SynonymStore* store) override;
};
