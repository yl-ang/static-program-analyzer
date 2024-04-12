#include <PKB/PKBClient/PKBFacadeReader.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class WhilePattern : public Pattern {
private:
    std::shared_ptr<ClauseArgument> whileSyn;
    std::vector<std::shared_ptr<ClauseArgument>> arguments;

    ClauseResult evaluateFirstArgSyn(PKBFacadeReader&, EvaluationDb& evalDb);
    ClauseResult evaluateFirstArgLiteral(PKBFacadeReader&, EvaluationDb& evalDb);
    ClauseResult evaluateFirstArgWildcard(PKBFacadeReader&, EvaluationDb& evalDb);

public:
    WhilePattern(std::shared_ptr<ClauseArgument> whileSyn, std::vector<std::shared_ptr<ClauseArgument>> args);
    ClauseResult evaluate(PKBFacadeReader&, EvaluationDb&) override;
    bool validateArguments() override;
};
