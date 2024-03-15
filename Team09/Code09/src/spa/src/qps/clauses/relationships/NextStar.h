#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "Relationship.h"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseResult.h"

class NextStar : public Relationship {
private:
    ClauseArgument& currentStmt;
    ClauseArgument& nextStmt;

    /**
     * Check if the result is a simple boolean result.
     * It is a simple boolean result if both arguments are not synonyms.
     * @return true if the result is a simple boolean result, false otherwise.
     */
    bool isSimpleResult() const;

    ClauseResult evaluateSynonymInteger(PKBFacadeReader&);
    ClauseResult evaluateSynonymWildcard(PKBFacadeReader&);
    ClauseResult evaluateBothSynonyms(PKBFacadeReader&);

public:
    NextStar(ClauseArgument&, ClauseArgument&);

    ClauseResult evaluate(PKBFacadeReader&) override;
    void checkSemantic() override;
};
