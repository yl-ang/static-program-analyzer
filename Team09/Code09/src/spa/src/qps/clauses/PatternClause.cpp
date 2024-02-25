#include "PatternClause.h"

#include "qps/ParserUtils.h"

PatternClause::PatternClause(const ClauseArgument* assign, const ClauseArgument* f, const ClauseArgument* s)
    : assignSynonym(assign), firstArg(f), secondArg(s) {}

ClauseType PatternClause::getType() const {
    return ClauseType::PATTERN;
}

bool PatternClause::equals(const QueryClause& other) const {
    if (const PatternClause* ptr = dynamic_cast<const PatternClause*>(&other)) {
        return (*firstArg).getValue() == (*ptr->firstArg).getValue() &&
               (*firstArg).getClauseType() == (*ptr->firstArg).getClauseType() &&
               (*secondArg).getValue() == (*ptr->secondArg).getValue() &&
               (*secondArg).getClauseType() == (*ptr->secondArg).getClauseType() &&
               removeAllWhitespaces((*assignSynonym).getValue()) ==
                   removeAllWhitespaces((*ptr->assignSynonym).getValue()) &&
               (*assignSynonym).getClauseType() == (*ptr->assignSynonym).getClauseType();
    }
    return false;
}

ClauseResult PatternClause::evaluate(PKBFacadeReader& pkb) {
    // TODO(Ezekiel): implement evaluate

    return {false};
}
