#include "Wildcard.h"

bool Wildcard::isWildcard() const {
    return true;
}

std::string Wildcard::getValue() const {
    return "_";
}

bool Wildcard::operator==(const ClauseArgument& other) const {
    return other.isWildcard();
}
