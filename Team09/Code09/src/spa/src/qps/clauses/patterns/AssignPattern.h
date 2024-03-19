#include <PKB/PKBClient/PKBFacadeReader.h>
#include <qps/GrammarUtils.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class AssignPattern : public Pattern {
private:
    ClauseArgument& assignSyn;
    ClauseArgument& firstArg;
    ClauseArgument& secondArg;

    ClauseResult evaluateFirstArgSyn(PKBFacadeReader&);
    ClauseResult evaluateNoArgsSyns(PKBFacadeReader&);

public:
    AssignPattern(ClauseArgument* assignSyn, std::vector<ClauseArgument*> args);
    ClauseResult evaluate(PKBFacadeReader&) override;
};
