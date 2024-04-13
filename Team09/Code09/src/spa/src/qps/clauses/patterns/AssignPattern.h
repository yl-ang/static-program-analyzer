#include <pkb/PKBClient/PKBFacadeReader.h>
#include <qps/GrammarUtils.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class AssignPattern : public Pattern {
private:
    std::shared_ptr<ClauseArgument> assignSyn;
    std::vector<std::shared_ptr<ClauseArgument>> arguments;

    ClauseResult evaluateFirstArgSyn(PKBFacadeReader&, EvaluationDb& evalDb);
    ClauseResult evaluateNoArgsSyns(PKBFacadeReader&, EvaluationDb& evalDb);

public:
    AssignPattern(std::shared_ptr<ClauseArgument> assignSyn, std::vector<std::shared_ptr<ClauseArgument>> args);
    ClauseResult evaluate(PKBFacadeReader&, EvaluationDb&) override;
    bool validateArguments() override;
};
