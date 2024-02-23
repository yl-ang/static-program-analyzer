#include "Integer.h"

Integer::Integer(const std::string& v) : value(v) {}

std::string Integer::getValue() const {
    return value;
}

bool Integer::isInteger() const {
    return true;
}

bool Integer::operator==(const Integer& other) const {
    return other.value == value;
}
