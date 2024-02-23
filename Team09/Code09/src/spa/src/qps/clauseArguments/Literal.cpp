#include "Literal.h"

Literal::Literal(const std::string& v) : value(v) {}

std::string Literal::getValue() const {
    return value;
}

bool Literal::isLiteral() const {
    return true;
}

bool Literal::operator==(const ClauseArgument& other) const {
    if (const Literal* ptr = dynamic_cast<const Literal*>(&other)) {
        return this->getValue() == other.getValue();
    }
    return false;
}
