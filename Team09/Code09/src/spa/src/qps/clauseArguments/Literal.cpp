#include "Literal.h"

#include "qps/QPSConstants.h"

Literal::Literal(const std::string& v) : value(v) {}

std::string Literal::getValue() const {
    return value;
}

bool Literal::isLiteral() const {
    return true;
}

std::string Literal::getClauseType() const {
    return QPSConstants::LITERAL_CLAUSE_TYPE;
}

bool Literal::operator==(const ClauseArgument& other) const {
    if (!other.isLiteral()) {
        return false;
    }

    return this->getValue() == other.getValue();
}
