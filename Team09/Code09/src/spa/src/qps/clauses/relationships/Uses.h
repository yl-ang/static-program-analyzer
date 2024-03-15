#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Uses : public Relationship {
private:
    ClauseArgument& user;
    ClauseArgument& var;

    /**
     * Check if the result is a simple boolean result.
     * It is a simple boolean result if both arguments are not synonyms.
     * @return true if the result is a simple boolean result, false otherwise.
     */
    bool isSimpleResult() const;

    ClauseResult evaluateUserSynonym(PKBFacadeReader&);
    ClauseResult variablesUsedByProcedure(PKBFacadeReader&);
    ClauseResult variablesUsedByStatement(PKBFacadeReader&);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&);
    std::unordered_set<StmtNum> filterStatementsByType(PKBFacadeReader&, DesignEntityType, std::unordered_set<StmtNum>);

public:
    Uses(ClauseArgument&, ClauseArgument&);

    ClauseResult evaluate(PKBFacadeReader&) override;
    void checkSemantic() override;
};
