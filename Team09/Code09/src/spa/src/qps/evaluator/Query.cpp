#include "Query.h"

#include <qps/clauses/SynonymValuesRetriever.h>
#include <qps/clauses/with/AttributeCollector.h>

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
    return [&pkb](const Synonym& synonym, const SynonymValue& value) -> SynonymValue {
        return AttributeCollector::collect(pkb, synonym, value);
    };
}

bool Query::hasNoClauses() const {
    return suchThatClauses.empty() && patternClauses.empty() && withClauses.empty();
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
    for (auto wc : withClauses) {
        if (wc->isBooleanResult() && !wc->runEvaluation(pkb).getBoolean()) {
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

    for (auto wc : withClauses) {
        if (!wc->isBooleanResult()) {
            nonBooleanClauses.push_back(wc);
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
            selectEntitiesWithoutAttributes.push_back(syn.getWithoutAttribute());
        } else {
            selectEntitiesWithoutAttributes.push_back(syn);
        }
    }

    const ClauseResult cr{SynonymValuesRetriever::retrieveAsClauseResult(pkb, selectEntitiesWithoutAttributes)};
    tm.join(cr);
}
