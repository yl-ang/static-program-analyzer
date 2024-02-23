#include "Wildcard.h"

bool Wildcard::isWildcard() const {
    return true;
}

std::string Wildcard::getValue() const {
    return "_";
}

bool Wildcard::operator==(const ClauseArgument& other) const {
    if (const Wildcard* ptr = dynamic_cast<const Wildcard*>(&other)) {
        return true;
    }
    return false;
}
