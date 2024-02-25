#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class Modifies : public Relationship {
private:
    ClauseArgument& stmt;
    ClauseArgument& var;

    /**
     * Check if the result is a simple boolean result.
     * It is a simple boolean result if both arguments are not synonyms.
     * @return true if the result is a simple boolean result, false otherwise.
     */
    bool isSimpleResult() const;

    ClauseResult statementsModifyLiteral(PKBFacadeReader&);
    ClauseResult allModifiedVariables(PKBFacadeReader&);
    ClauseResult allStmtsThatModifySomeVar(PKBFacadeReader&);
    ClauseResult variablesModifedByStatement(PKBFacadeReader&);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&);
    std::unordered_set<StmtNum> filterStatementsByType(PKBFacadeReader&, DesignEntityType, std::unordered_set<StmtNum>);

public:
    Modifies(ClauseArgument&, ClauseArgument&);

    ClauseResult evaluate(PKBFacadeReader&) override;
};
