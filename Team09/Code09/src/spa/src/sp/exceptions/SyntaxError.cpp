#include "SyntaxError.h"

SyntaxError::SyntaxError(const std::string& msg) : errMsg(msg) {}

const char* SyntaxError::what() const noexcept {
    return errMsg.c_str();
}
