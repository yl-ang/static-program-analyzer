#include "NextStar.h"

NextStar::NextStar(ClauseArgument& currentStmt, ClauseArgument& nextStmt)
    : currentStmt(currentStmt), nextStmt(nextStmt) {}

bool Next::isSimpleResult() const {
    return !currentStmt.isSynonym() && !nextStmt.isSynonym();
}

ClauseResult Next::evaluate(PKBFacadeReader& reader) {
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

ClauseResult Next::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    return {};
}

ClauseResult Next::evaluateSynonymInteger(PKBFacadeReader& reader) {
    return {};
}

ClauseResult Next::evaluateBothSynonyms(PKBFacadeReader& reader) {
    return {};
}
