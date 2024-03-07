#include "Next.h"

Next::Next(ClauseArgument& currentStmt, ClauseArgument& nextStmt) : currentStmt(currentStmt), nextStmt(nextStmt) {}

bool Next::isSimpleResult() const {
    return !currentStmt.isSynonym() && !nextStmt.isSynonym();
}

ClauseResult Next::evaluate(PKBFacadeReader& reader) {
    if (isSimpleResult()) {
        // return {reader.hasNextRelationship(currentStmt, nextStmt)};
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
    return {false};
}

ClauseResult Next::evaluateSynonymInteger(PKBFacadeReader& reader) {
    return {false};
}

ClauseResult Next::evaluateBothSynonyms(PKBFacadeReader& reader) {
    return {false};
}
