#include "Wildcard.h"

#include "qps/QPSConstants.h"

bool Wildcard::isWildcard() const {
    return true;
}

std::string Wildcard::getValue() const {
    return QPSConstants::WILDCARD_VALUE;
}

std::string Wildcard::getClauseType() const {
    return QPSConstants::WILDCARD_CLAUSE_TYPE;
}

bool Wildcard::operator==(const ClauseArgument& other) const {
    return other.isWildcard();
}
