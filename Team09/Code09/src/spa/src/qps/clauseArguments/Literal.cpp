#include "Literal.h"

Literal::Literal(const std::string& v) : value(v) {}

std::string Literal::getValue() const {
    return value;
}

bool Literal::isLiteral() const {
    return true;
}

bool Literal::operator==(const Literal& other) const {
    return value == other.value;
}
