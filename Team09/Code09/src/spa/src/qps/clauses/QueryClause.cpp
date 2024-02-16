#include "QueryClause.h"

#include <iostream>
#include <string>
#include <vector>

SelectClause::SelectClause(const std::string& a) : arg(a) {}

ClauseType SelectClause::getType() const {
    return ClauseType::SELECT;
}

std::string SelectClause::getArgument() const {
    return arg;
}

bool SelectClause::equals(const QueryClause& other) const {
    if (const SelectClause* ptr = dynamic_cast<const SelectClause*>(&other)) {
        return arg == ptr->arg;
    }
    return false;
}
