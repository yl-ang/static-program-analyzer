#include "PatternClause.h"

PatternClause::PatternClause(const QueryEntity& assign, const QueryEntity& f,
                             const QueryEntity& s)
    : assignSynonym(assign), firstArg(f), secondArg(s) {}

ClauseType PatternClause::getType() const {
    return ClauseType::PATTERN;
}

bool PatternClause::equals(const QueryClause&) const {
    if (const PatternClause* ptr = dynamic_cast<const PatternClause*>(&other)) {
        return firstArg == ptr->firstArg && secondArg == ptr->secondArg &&
               assignSynonym == ptr->assignSynonym;
    }
    return false;
}

Table PatternClause::evaluate(const PKBFacadeReader&) {
    // TODO(Ezekiel): implement evaluate

    return Table();
}
