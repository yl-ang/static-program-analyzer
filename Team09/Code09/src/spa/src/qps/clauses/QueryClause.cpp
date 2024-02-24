#include "QueryClause.h"

#include <iostream>
#include <string>
#include <vector>

SelectClause::SelectClause(const std::string& a) : arg(a) {}

ClauseType SelectClause::getType() {
    return ClauseType::SELECT;
}

std::string SelectClause::getArgument() {
    return arg;
}

bool SelectClause::equals(const QueryClause& other) {
    if (const SelectClause* ptr = dynamic_cast<const SelectClause*>(&other)) {
        return arg == ptr->arg;
    }
    return false;
}

// Table SelectClause::evaluate(const PKBFacadeReader& reader) {
//    // TODO(Ezekiel): eventually deprecate selectClause because we don't need it
//    return {};
// }
