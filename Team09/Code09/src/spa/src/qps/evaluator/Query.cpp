#include "Query.h"

#include <qps/clauses/SynonymValuesRetriever.h>

#include <queue>

Query::Query(const std::vector<Synonym>& se, const std::vector<std::shared_ptr<SuchThatClause>>& stc,
             const std::vector<std::shared_ptr<PatternClause>>& pc, const std::vector<std::shared_ptr<WithClause>>& wc)
    : selectEntities(se), suchThatClauses(stc), patternClauses(pc), withClauses(wc) {}

std::vector<std::string> Query::evaluate(PKBFacadeReader& pkb) const {
    if (hasNoClauses() && isSelectBoolean()) {
        return {QPSConstants::TRUE_STRING};
    }

    if (!evaluateBooleanClauses(pkb)) {
        return getEmptyResult();
    }

    const TableManager tableManager{};

    if (buildAndJoinSelectTable(tableManager, pkb); tableManager.isEmpty()) {
        // There are no results to select at all. Return empty result.
        return getEmptyResult();
    }

    ArrangedClauses ac = arrangeClauses();
    if (evaluateAndJoinClauses(tableManager, ac.selectConnectedClauses, pkb); tableManager.isEmpty()) {
        return getEmptyResult();
    }

    if (evaluateAndJoinClauses(tableManager, ac.nonSelectConnectedClauses, pkb); tableManager.isEmpty()) {
        return getEmptyResult();
    }

    if (isSelectBoolean()) {
        return tableManager.isEmpty() ? std::vector{QPSConstants::FALSE_STRING}
                                      : std::vector{QPSConstants::TRUE_STRING};
    }

    projectAttributes(tableManager, pkb);
    return tableManager.extractResults(selectEntities);
}

void Query::projectAttributes(const TableManager& tm, PKBFacadeReader& pkb) const {
    std::vector<Synonym> synonymsWithAttributes{};

    for (Synonym syn : selectEntities) {
        if (syn.getAttr().has_value()) {
            synonymsWithAttributes.push_back(syn);
        }
    }

    tm.projectAttributes(synonymsWithAttributes, headerMatcher, projectSynonymAttributesTransformer(pkb));
}

Synonym Query::headerMatcher(const std::vector<Synonym>& synonyms, Synonym newSynonym) {
    auto it = std::find_if(synonyms.begin(), synonyms.end(), [&newSynonym](const Synonym& existingSynonym) {
        return newSynonym.equalSynonymValue(existingSynonym);
    });

    if (it == synonyms.end()) {
        throw Exception("Attempting to access attribute of non-existent synonym");
    }
    return *it;
}

ValueTransformer Query::projectSynonymAttributesTransformer(PKBFacadeReader& pkb) {
    return [&pkb](Synonym synonym, SynonymValue value) -> SynonymValue {
        const auto& attr = synonym.getAttr();

        if (!attr.has_value()) {
            return value;
        }

        switch (attr.value()) {
        case SynonymAttributeType::PROCNAME:
            if (synonym.getType() == DesignEntityType::CALL) {
                std::optional procedureName = pkb.getCallFromStmtNum(std::stoi(value));
                if (procedureName.has_value()) {
                    return procedureName.value();
                }
                throw Exception("No procedure name found for Call Statement");
            }
            return value;

        case SynonymAttributeType::VARNAME:
            if (synonym.getType() == DesignEntityType::READ) {
                const auto& variables = pkb.getModifiesVariablesByStatement(std::stoi(value));
                if (variables.size() != 1) {
                    throw Exception("None or more than 1 variable found when reading Read Statement");
                }
                const auto& variable = *variables.begin();
                return variable;
            }
            if (synonym.getType() == DesignEntityType::PRINT) {
                const auto& variables = pkb.getUsesVariablesByStatement(std::stoi(value));
                if (variables.size() != 1) {
                    throw Exception("None or more than 1 variable found when reading Print Statement");
                }
                const auto& variable = *variables.begin();
                return variable;
            }
            return value;

        case SynonymAttributeType::VALUE:
        case SynonymAttributeType::STMTNUM:
            return value;
        }

        return value;
    };
}

bool Query::hasNoClauses() const {
    return suchThatClauses.empty() && patternClauses.empty();
}

bool Query::isSelectBoolean() const {
    return selectEntities.empty();
}

std::vector<std::string> Query::getEmptyResult() const {
    return this->isSelectBoolean() ? std::vector{QPSConstants::FALSE_STRING} : std::vector<std::string>{};
}

bool Query::evaluateAndJoinClauses(const TableManager& tm,
                                   const std::vector<std::vector<QueryClausePtr>>& connectedClausesList,
                                   PKBFacadeReader& pkb) {
    for (const std::vector<QueryClausePtr>& connectedClauses : connectedClausesList) {
        for (QueryClausePtr clause : connectedClauses) {
            ClauseResult res = clause->runEvaluation(pkb);
            tm.join(res);
        }
        if (tm.isEmpty()) {
            // If the table is empty, we can stop evaluating the clauses
            return false;
        }
    }
    return true;
}

// TODO(Ezekiel): Write the actual algorithm to split into connected synonyms for optimization.
std::vector<std::vector<QueryClausePtr>> Query::splitIntoConnectedSynonyms() const {
    std::vector<std::vector<QueryClausePtr>> res{};
    for (QueryClausePtr c : getNonBooleanClauses()) {
        std::vector<QueryClausePtr> connectedClauses{c};
        res.push_back(connectedClauses);
    }
    return res;
}

ArrangedClauses Query::arrangeClauses() const {
    std::vector<std::vector<QueryClausePtr>> selectConnectedClauses{};
    std::vector<std::vector<QueryClausePtr>> nonSelectConnectedClauses{};

    for (std::vector<QueryClausePtr> connectedClauses : splitIntoConnectedSynonyms()) {
        bool hasSelectSynoyms = false;
        for (QueryClausePtr clause : connectedClauses) {
            if (containsSelectSynonyms(clause)) {
                hasSelectSynoyms = true;
                break;
            }
        }
        hasSelectSynoyms ? selectConnectedClauses.push_back(connectedClauses)
                         : nonSelectConnectedClauses.push_back(connectedClauses);
    }

    return ArrangedClauses{selectConnectedClauses, nonSelectConnectedClauses};
}

bool Query::evaluateBooleanClauses(PKBFacadeReader& pkb) const {
    for (auto stc : suchThatClauses) {
        if (stc->isBooleanResult() && !stc->runEvaluation(pkb).getBoolean()) {
            return false;
        }
    }
    return true;
}

std::vector<QueryClausePtr> Query::getNonBooleanClauses() const {
    std::vector<QueryClausePtr> nonBooleanClauses{};

    // Convert patternClauses to shared_ptr
    for (auto pc : patternClauses) {
        nonBooleanClauses.push_back(pc);
    }

    // Convert suchThatClauses to shared_ptr if they are not boolean result
    for (auto stc : suchThatClauses) {
        if (!stc->isBooleanResult()) {
            nonBooleanClauses.push_back(stc);
        }
    }
    return nonBooleanClauses;
}

bool Query::containsSelectSynonyms(QueryClausePtr clause) const {
    for (Synonym selectSyn : selectEntities) {
        if (clause->containsSynonym(selectSyn)) {
            return true;
        }
    }
    return false;
}

void Query::buildAndJoinSelectTable(const TableManager& tm, PKBFacadeReader& pkb) const {
    std::vector<Synonym> selectEntitiesWithoutAttributes{};

    for (Synonym syn : selectEntities) {
        if (syn.getAttr().has_value()) {
            selectEntitiesWithoutAttributes.push_back(Synonym{syn.getType(), syn.getName()});
        } else {
            selectEntitiesWithoutAttributes.push_back(syn);
        }
    }

    const ClauseResult cr{SynonymValuesRetriever::retrieveAsClauseResult(pkb, selectEntitiesWithoutAttributes)};
    tm.join(cr);
}
