#include <pkb/PKBClient/PKBFacadeReader.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class IfPattern : public Pattern {
private:
    std::shared_ptr<ClauseArgument> ifSyn;
    std::vector<std::shared_ptr<ClauseArgument>> arguments;

    ClauseResult evaluateFirstArgSyn(PKBFacadeReader&, EvaluationDb& evalDb);
    ClauseResult evaluateFirstArgLiteral(PKBFacadeReader&, EvaluationDb& evalDb);
    ClauseResult evaluateFirstArgWildcard(PKBFacadeReader&, EvaluationDb& evalDb);

public:
    IfPattern(std::shared_ptr<ClauseArgument> ifSyn, std::vector<std::shared_ptr<ClauseArgument>> args);
    ClauseResult evaluate(PKBFacadeReader&, EvaluationDb&) override;
    bool validateArguments() override;
};
