#include <PKB/PKBClient/PKBFacadeReader.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class WhilePattern : public Pattern {
private:
    ClauseArgument& whileSyn;
    ClauseArgument& firstArg;
    ClauseArgument& secondArg;

public:
    WhilePattern(ClauseArgument* whileSyn, std::vector<ClauseArgument*> args);
    ClauseResult evaluate(PKBFacadeReader&) override;
};
