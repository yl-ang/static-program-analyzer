#include <PKB/PKBClient/PKBFacadeReader.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class IfPattern : public Pattern {
private:
    ClauseArgument& ifSyn;
    ClauseArgument& firstArg;
    ClauseArgument& secondArg;
    ClauseArgument& thirdArg;

public:
    IfPattern(ClauseArgument* ifSyn, std::vector<ClauseArgument*> args);
    ClauseResult evaluate(PKBFacadeReader&) override;
};
