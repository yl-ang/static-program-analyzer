#include "NextStar.h"

NextStar::NextStar(ClauseArgument& currentStmt, ClauseArgument& nextStmt)
    : currentStmt(currentStmt), nextStmt(nextStmt) {}

bool NextStar::isSimpleResult() const {
    return !currentStmt.isSynonym() && !nextStmt.isSynonym();
}

void NextStar::checkSemantic() {
    if (currentStmt.isSynonym()) {
        Synonym first = dynamic_cast<Synonym&>(currentStmt);
        if (first.getType() == DesignEntityType::VARIABLE || first.getType() == DesignEntityType::CONSTANT ||
            first.getType() == DesignEntityType::PROCEDURE) {
            throw QPSSemanticError();
        }
    }
    if (nextStmt.isSynonym()) {
        Synonym second = dynamic_cast<Synonym&>(nextStmt);
        if (second.getType() == DesignEntityType::VARIABLE || second.getType() == DesignEntityType::CONSTANT ||
            second.getType() == DesignEntityType::PROCEDURE) {
            throw QPSSemanticError();
        }
    }
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
