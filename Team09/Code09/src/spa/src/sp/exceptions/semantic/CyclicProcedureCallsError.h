#pragma once

#include <string>

#include "sp/Utils.h"
#include "sp/exceptions/semantic/SemanticError.h"

class CyclicProcedureCallsError : public SemanticError {
public:
    explicit CyclicProcedureCallsError() : SemanticError(getErrorMessage()) {}

protected:
    std::string getErrorMessage() {
        return "Cyclic procedure calls detected.";
    }
};
