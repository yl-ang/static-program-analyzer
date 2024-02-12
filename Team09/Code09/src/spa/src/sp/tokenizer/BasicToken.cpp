#include "BasicToken.h"

BasicToken::BasicToken(std::string value, BASIC_TOKEN_TYPE type) {
    this->type = type;
    this->value = value;
}
