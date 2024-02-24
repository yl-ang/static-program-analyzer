#include "PatternClause.h"
#include <iostream>
using namespace std;

PatternClause::PatternClause(const ClauseArgument* assign, const ClauseArgument* f, const ClauseArgument* s)
    : assignSynonym(assign), firstArg(f), secondArg(s) {}

ClauseType PatternClause::getType() {
    return ClauseType::PATTERN;
}

bool PatternClause::equals(const QueryClause& other) {
    if (const PatternClause* ptr = dynamic_cast<const PatternClause*>(&other)) {
        return (*firstArg).getValue() == (*ptr->firstArg).getValue()
            && (*firstArg).getClauseType() == (*ptr->firstArg).getClauseType() 
            && (*secondArg).getValue() == (*ptr->secondArg).getValue()
            && (*secondArg).getClauseType() == (*ptr->secondArg).getClauseType()
            && (*assignSynonym).getValue() == (*ptr->assignSynonym).getValue()
            && (*assignSynonym).getClauseType() == (*ptr->assignSynonym).getClauseType();
    }
    return false;
}

void PatternClause::print() {
    cout << assignSynonym->isSynonym() << assignSynonym->getValue() << firstArg->getValue() << secondArg->isExpressionSpec() << secondArg->getValue() << "\n";
}

// Table PatternClause::evaluate(const PKBFacadeReader& pkb) {
//     // TODO(Ezekiel): implement evaluate

//     return Table();
// }
