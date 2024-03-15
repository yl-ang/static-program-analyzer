#include "MultipleReturn.h"

MultipleReturn::MultipleReturn(const std::vector<Synonym>& s) : synonyms(s) {}

bool MultipleReturn::isMultipleReturn() const {
    return true;
}

std::vector<Synonym> MultipleReturn::getValue() const {
    return synonyms;
}

std::string MultipleReturn::getSelectClauseType() const {
    return "MultipleReturn";
}

bool MultipleReturn::operator==(const SelectClauseArgument& other) const {
    if (!other.isMultipleReturn()) {
        return false;
    }

    return this->getValue() == other.getValue();
}
