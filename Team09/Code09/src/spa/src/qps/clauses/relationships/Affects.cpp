#include "Affects.h"

#include "../ClauseEvaluatorUtils.h"

Affects::Affects(ClauseArgument& affector, ClauseArgument& affected)
    : affector(affector), affected(affected) {}

bool Affects::validateArguments() {
    if (!(affector.isSynonym() && affector.isInteger() && affector.isWildcard())) {
        return false;
    }
    if (affector.isSynonym()) {
        Synonym first = dynamic_cast<Synonym&>(affector);
        if (first.getType() != DesignEntityType::ASSIGN) {
            return false;
        }
    }
    if (!(affected.isSynonym() && affected.isInteger() && affected.isWildcard())) {
        return false;
    }
    if (affected.isSynonym()) {
        Synonym second = dynamic_cast<Synonym&>(affected);
        if (second.getType() != DesignEntityType::ASSIGN) {
            return false;
        }
    }
    return true;
}

ClauseResult Affects::evaluate(PKBFacadeReader& reader) {
    // if (isSimpleResult()) {
    //     return {reader.hasParentRelationship(parent, child)};
    // }
    /**
     * synonym synonym
     * synonym integer / integer synonym 
     * wildcard synonym / synonym wilcard
     * integer integer TRUE/FALSE
     * wildcard integer / integer wildcard 
     * wildcard wildcard
    */

    if ((affector.isInteger() && affected.isSynonym()) || (affector.isSynonym() && affected.isInteger())) {
        return evaluateSynonymInteger(reader);
    }

    if ((affector.isWildcard() && affected.isSynonym()) || (affector.isSynonym() && affected.isWildcard())) {
        return evaluateSynonymWildcard(reader);
    }

    return evaluateBothSynonyms(reader);
}

// ClauseResult Affects::evaluateBothSynonyms(PKBFacadeReader& reader) {
    
// }

// ClauseResult Affects::evaluateUserSynonym(PKBFacadeReader& reader) {
    
// }

// ClauseResult Affects::variablesUsedByProcedure(PKBFacadeReader& reader) {
    
// }

// ClauseResult Affects::variablesUsedByStatement(PKBFacadeReader& reader) {
    
// }

bool Affects::isSimpleResult() const {
    return !affector.isSynonym() && !affected.isSynonym();
}
