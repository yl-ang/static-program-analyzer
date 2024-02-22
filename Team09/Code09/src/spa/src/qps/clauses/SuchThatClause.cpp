#include "SuchThatClause.h"

#include <iostream>

// TODO(Ezekiel): add validations for arguments
SuchThatClause::SuchThatClause(const SuchThatClauseType& t, const Synonym& f, const Synonym& s)
    : type(t), firstArg(f), secondArg(s) {}

ClauseType SuchThatClause::getType() const {
    return ClauseType::SUCH_THAT;
}

bool SuchThatClause::equals(const QueryClause& other) const {
    if (const SuchThatClause* ptr = dynamic_cast<const SuchThatClause*>(&other)) {
        return type == ptr->type && firstArg == ptr->firstArg && secondArg == ptr->secondArg;
    }
    return false;
}

SuchThatClauseType SuchThatClause::determineType(const std::string type) {
    SuchThatClauseType suchThatClauseType;
    if (type == "Follows") {
        suchThatClauseType = SuchThatClauseType::FOLLOWS;
    } else if (type == "Follows*") {
        suchThatClauseType = SuchThatClauseType::FOLLOWS_STAR;
    } else if (type == "Parent") {
        suchThatClauseType = SuchThatClauseType::PARENT;
    } else if (type == "Parent*") {
        suchThatClauseType = SuchThatClauseType::PARENT_STAR;
    } else {
        std::cout << "suchThatClauseType is not found in valid types: " << type << "\n";
        exit(1);
    }
    return suchThatClauseType;
}

Table SuchThatClause::evaluate(const PKBFacadeReader& reader) {
    // TODO(Ezekiel): implement evaluate

    return Table();
}
