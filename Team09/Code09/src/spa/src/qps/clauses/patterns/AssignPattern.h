#include <PKB/PKBClient/PKBFacadeReader.h>
#include <qps/GrammarUtils.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class AssignPattern : public Pattern {
private:
    std::shared_ptr<ClauseArgument> assignSyn;
    std::vector<std::shared_ptr<ClauseArgument>> arguments;

    ClauseResult evaluateFirstArgSyn(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);
    ClauseResult evaluateNoArgsSyns(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);

public:
    AssignPattern(std::shared_ptr<ClauseArgument> assignSyn, std::vector<std::shared_ptr<ClauseArgument>> args);
    ClauseResult evaluate(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>&) override;
    bool validateArguments() override;
};
