#include "SemanticError.h"

SemanticError::SemanticError() {
    Exception("Semantic Error has occurred!");
}

SemanticError::SemanticError(std::string message) {
    Exception(message);
}
