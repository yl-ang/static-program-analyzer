#include "Literal.h"

Literal::Literal(const std::string& v) : value(v) {}

std::string Literal::getValue() const {
    return value;
}

bool Literal::isLiteral() const {
    return true;
}

bool Literal::operator==(const ClauseArgument& other) const {
    if (!other.isLiteral()) {
        return false;
    }

    return this->getValue() == other.getValue();
}
