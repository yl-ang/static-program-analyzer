#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Uses : public Relationship {
private:
    std::shared_ptr<ClauseArgument> user;
    std::shared_ptr<ClauseArgument> var;

    /**
     * Check if the result is a simple boolean result.
     * It is a simple boolean result if both arguments are not synonyms.
     * @return true if the result is a simple boolean result, false otherwise.
     */
    bool isSimpleResult() const;

    ClauseResult evaluateUserSynonym(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);
    ClauseResult variablesUsedByProcedure(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);
    ClauseResult variablesUsedByStatement(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);

public:
    Uses(std::shared_ptr<ClauseArgument>, std::shared_ptr<ClauseArgument>);

    ClauseResult evaluate(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>&) override;
    bool validateArguments() override;
};
