#include "Exception.h"

#include <utility>

Exception::Exception() = default;

Exception::Exception(std::string message) {
    errorMessage = message;
}

std::string Exception::getMessage() {
    return errorMessage;
}
