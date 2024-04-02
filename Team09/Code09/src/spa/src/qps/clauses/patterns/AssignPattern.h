#include <PKB/PKBClient/PKBFacadeReader.h>
#include <qps/GrammarUtils.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class AssignPattern : public Pattern {
private:
    std::shared_ptr<ClauseArgument> assignSyn;
    std::shared_ptr<ClauseArgument> firstArg;
    std::shared_ptr<ClauseArgument> secondArg;

    ClauseResult evaluateFirstArgSyn(PKBFacadeReader&);
    ClauseResult evaluateNoArgsSyns(PKBFacadeReader&);

public:
    AssignPattern(std::shared_ptr<ClauseArgument> assignSyn, std::vector<std::shared_ptr<ClauseArgument>> args);
    ClauseResult evaluate(PKBFacadeReader&) override;
};
