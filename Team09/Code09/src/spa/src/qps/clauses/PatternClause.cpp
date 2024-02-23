#include "PatternClause.h"

PatternClause::PatternClause(const ClauseArgument* assign, const ClauseArgument* f, const ClauseArgument* s)
    : assignSynonym(assign), firstArg(f), secondArg(s) {}

ClauseType PatternClause::getType() {
    return ClauseType::PATTERN;
}

bool PatternClause::equals(const QueryClause& other) {
    if (const PatternClause* ptr = dynamic_cast<const PatternClause*>(&other)) {
        return *(firstArg) == *(ptr->firstArg) 
            && *(secondArg) == *(ptr->secondArg) 
            && *(assignSynonym) == *(ptr->assignSynonym);
    }
    return false;
}

// Table PatternClause::evaluate(const PKBFacadeReader& pkb) {
//     // TODO(Ezekiel): implement evaluate

//     return Table();
// }
