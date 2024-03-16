#pragma once

#include <string>

#include "sp/Utils.h"
#include "sp/exceptions/semantic/SemanticError.h"

class NoRecursiveCallsAllowedError : public SemanticError {
public:
    explicit NoRecursiveCallsAllowedError(std::string name) : SemanticError(getErrorMessage(name)) {}

protected:
    std::string getErrorMessage(std::string name) {
        return "Procedure with name: " + name + " called within " + name;
    }
};
