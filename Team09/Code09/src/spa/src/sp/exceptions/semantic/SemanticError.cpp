#include "SemanticError.h"

SemanticError::SemanticError(const std::string& msg) : errMsg(msg) {}

const char* SemanticError::what() const noexcept {
    return errMsg.c_str();
}
