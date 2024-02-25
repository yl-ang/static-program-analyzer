#include "Integer.h"

Integer::Integer(const std::string& v) : value(v) {}

std::string Integer::getValue() const {
    return value;
}

bool Integer::isInteger() const {
    return true;
}

std::string Integer::getClauseType() const {
    return "Integer";
}

bool Integer::operator==(const ClauseArgument& other) const {
    if (!other.isInteger()) {
        return false;
    }
    return this->getValue() == other.getValue();
}
