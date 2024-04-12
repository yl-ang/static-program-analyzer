#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Modifies : public Relationship {
private:
    std::shared_ptr<ClauseArgument> modifier;
    std::shared_ptr<ClauseArgument> var;

    /**
     * Check if the result is a simple boolean result.
     * It is a simple boolean result if both arguments are not synonyms.
     * @return true if the result is a simple boolean result, false otherwise.
     */
    bool isSimpleResult() const;

    ClauseResult evaluateModifierSynonym(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);
    ClauseResult variablesModifedByStatement(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);
    ClauseResult variablesModifiedByProcedure(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>& evalDb);

public:
    Modifies(std::shared_ptr<ClauseArgument>, std::shared_ptr<ClauseArgument>);

    ClauseResult evaluate(PKBFacadeReader&, const std::shared_ptr<EvaluationDb>&) override;
    bool validateArguments() override;
};
