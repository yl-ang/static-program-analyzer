#include "SuchThatClause.h"

// TODO(Ezekiel): add validations for arguments
SuchThatClause::SuchThatClause(const SuchThatClauseType& t, const QueryEntity& f, const QueryEntity& s)
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

Table SuchThatClause::evaluate(const PKBFacadeReader& reader) {
    // TODO(Ezekiel): implement evaluate

    return Table();
}
