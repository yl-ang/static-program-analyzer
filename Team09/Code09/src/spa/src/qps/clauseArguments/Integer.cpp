#include "Integer.h"

Integer::Integer(const int& v) : value(v) {}

int Integer::getValue() const {
    return value;
}

bool Integer::operator==(const Integer& other) const {
    return other.value == value;
}
