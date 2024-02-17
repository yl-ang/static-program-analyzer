#include "SyntaxError.h"

SyntaxError::SyntaxError() {
    Exception("Syntax Error has occurred!");
}

SyntaxError::SyntaxError(std::string message) {
    Exception(message);
}
