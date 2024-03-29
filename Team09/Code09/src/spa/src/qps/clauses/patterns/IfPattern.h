#include <PKB/PKBClient/PKBFacadeReader.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class IfPattern : public Pattern {
private:
    std::shared_ptr<ClauseArgument> ifSyn;
    std::shared_ptr<ClauseArgument> firstArg;
    std::shared_ptr<ClauseArgument> secondArg;
    std::shared_ptr<ClauseArgument> thirdArg;

    ClauseResult evaluateFirstArgSyn(PKBFacadeReader&);
    ClauseResult evaluateFirstArgLiteral(PKBFacadeReader&);
    ClauseResult evaluateFirstArgWildcard(PKBFacadeReader&);

public:
    IfPattern(std::shared_ptr<ClauseArgument> ifSyn, std::vector<std::shared_ptr<ClauseArgument>> args);
    ClauseResult evaluate(PKBFacadeReader&) override;
};
