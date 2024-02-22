#include "Wildcard.h"

bool Wildcard::isWildcard() const {
    return true;
}

std::string Wildcard::getValue() const {
    return "_";
}
