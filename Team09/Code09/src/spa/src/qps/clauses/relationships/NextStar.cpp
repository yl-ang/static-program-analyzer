#include "NextStar.h"

NextStar::NextStar(ClauseArgument& currentStmt, ClauseArgument& nextStmt)
    : currentStmt(currentStmt), nextStmt(nextStmt) {}

bool NextStar::isSimpleResult() const {
    return !currentStmt.isSynonym() && !nextStmt.isSynonym();
}

ClauseResult NextStar::evaluate(PKBFacadeReader& reader) {
    if (isSimpleResult()) {
        return {false};
    }

    if ((currentStmt.isSynonym() && nextStmt.isWildcard()) || (currentStmt.isWildcard() && nextStmt.isSynonym())) {
        return evaluateSynonymWildcard(reader);
    }

    if ((currentStmt.isSynonym() && nextStmt.isInteger()) || (currentStmt.isInteger() && nextStmt.isSynonym())) {
        return evaluateSynonymInteger(reader);
    }

    return evaluateBothSynonyms(reader);
}

ClauseResult NextStar::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    throw "Not implemented";
}

ClauseResult NextStar::evaluateSynonymInteger(PKBFacadeReader& reader) {
    throw "Not implemented";
}

ClauseResult NextStar::evaluateBothSynonyms(PKBFacadeReader& reader) {
    throw "Not implemented";
}
