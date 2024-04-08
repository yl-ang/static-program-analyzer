#include "Integer.h"

#include "qps/QPSConstants.h"

Integer::Integer(const std::string& v) : value(v) {}

std::string Integer::getValue() const {
    return value;
}

bool Integer::isInteger() const {
    return true;
}

std::string Integer::getClauseType() const {
    return QPSConstants::INTEGER_CLAUSE_TYPE;
}

bool Integer::operator==(const ClauseArgument& other) const {
    if (!other.isInteger()) {
        return false;
    }
    return this->getValue() == other.getValue();
}
