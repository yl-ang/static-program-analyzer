#pragma once

#include <string>

#include "sp/Utils.h"
#include "sp/exceptions/semantic/SemanticError.h"

class NotCallingProcedureError : public SemanticError {
public:
    explicit NotCallingProcedureError(std::string name) : SemanticError(getErrorMessage(name)) {}

protected:
    std::string getErrorMessage(std::string name) {
        return "Attempted to call: " + name + ", which is not a procedure.";
    }
};
