#pragma once

#include <string>

#include "sp/Utils.h"
#include "sp/exceptions/semantic/SemanticError.h"

class DuplicateProcedureError : public SemanticError {
public:
    explicit DuplicateProcedureError(std::string name) : SemanticError(getErrorMessage(name)) {}

protected:
    std::string getErrorMessage(std::string name) {
        return "Procedure with name: " + name + " declared twice";
    }
};
