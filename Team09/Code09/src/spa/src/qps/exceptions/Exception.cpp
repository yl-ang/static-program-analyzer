#include "Exception.h"

#include <utility>

Exception::Exception(std::string message) {
    this->errorMessage = message;
}

std::string Exception::getMessage() {
    return this->errorMessage;
}
