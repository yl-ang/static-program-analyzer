#include <PKB/PKBClient/PKBFacadeReader.h>
#include <qps/clauses/QueryClause.h>

#include <vector>

#include "Pattern.h"

class WhilePattern : public Pattern {
private:
    std::shared_ptr<ClauseArgument> whileSyn;
    std::vector<std::shared_ptr<ClauseArgument>> arguments;

    ClauseResult evaluateFirstArgSyn(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);
    ClauseResult evaluateFirstArgLiteral(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);
    ClauseResult evaluateFirstArgWildcard(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);

public:
    WhilePattern(std::shared_ptr<ClauseArgument> whileSyn, std::vector<std::shared_ptr<ClauseArgument>> args);
    ClauseResult evaluate(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>&) override;
    bool validateArguments() override;
};
