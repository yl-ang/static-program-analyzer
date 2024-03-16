#pragma once

#include <string>
#include <vector>

#include "qps/validator/ValidatorSynonymStore.h"

class ArgumentsValidator {
protected:
    std::vector<std::string> args;

public:
    virtual void validateSyntax() = 0;
    virtual void validateSemantic(ValidatorSynonymStore* store) = 0;
    virtual ~ArgumentsValidator(){};
};
