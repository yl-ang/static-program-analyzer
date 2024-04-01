#include <PKB/PKBClient/PKBFacadeReader.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class IfPattern : public Pattern {
private:
    ClauseArgument& ifSyn;
    std::vector<ClauseArgument*> arguments;

    ClauseResult evaluateFirstArgSyn(PKBFacadeReader&);
    ClauseResult evaluateFirstArgLiteral(PKBFacadeReader&);
    ClauseResult evaluateFirstArgWildcard(PKBFacadeReader&);

public:
    IfPattern(ClauseArgument* ifSyn, std::vector<ClauseArgument*> args);
    ClauseResult evaluate(PKBFacadeReader&) override;
    bool validateArguments() override;
};
