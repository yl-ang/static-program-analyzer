#include "Wildcard.h"

bool Wildcard::isWildcard() const {
    return true;
}

std::string Wildcard::getValue() const {
    return "_";
}

std::string Wildcard::getClauseType() const {
    return "Wildcard";
}

bool Wildcard::operator==(const ClauseArgument& other) const {
    return other.isWildcard();
}
