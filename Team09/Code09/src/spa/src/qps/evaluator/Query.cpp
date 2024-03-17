#include "Query.h"

#include <queue>

Query::Query(const std::vector<Synonym>& se, const std::vector<SuchThatClause>& stc,
             const std::vector<PatternClause>& pc)
    : selectEntities(se), suchThatClauses(stc), patternClauses(pc) {}

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

    return tableManager.extractResults(selectEntities);
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
            ClauseResult res = clause->evaluate(pkb);
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
    for (SuchThatClause stc : suchThatClauses) {
        if (stc.isBooleanResult() && !stc.evaluate(pkb).getBoolean()) {
            return false;
        }
    }
    return true;
}

std::vector<QueryClausePtr> Query::getNonBooleanClauses() const {
    std::vector<QueryClausePtr> nonBooleanClauses{};

    // Convert patternClauses to shared_ptr
    for (PatternClause pc : patternClauses) {
        nonBooleanClauses.push_back(std::make_shared<PatternClause>(pc));
    }

    // Convert suchThatClauses to shared_ptr if they are not boolean result
    for (SuchThatClause stc : suchThatClauses) {
        if (!stc.isBooleanResult()) {
            nonBooleanClauses.push_back(std::make_shared<SuchThatClause>(stc));
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

void Query::buildAndJoinSelectTable(const TableManager& tm, const PKBFacadeReader& pkb) const {
    if (selectEntities.empty()) {
        return;
    }

    for (Synonym entity : selectEntities) {
        ColumnData row{};

        switch (entity.getType()) {
        case DesignEntityType::VARIABLE:
            for (std::string var : pkb.getVariables()) {
                row.push_back(var);
            }
            break;
        case DesignEntityType::CONSTANT:
            for (std::string con : pkb.getConstants()) {
                row.push_back(con);
            }
            break;
        case DesignEntityType::PROCEDURE:
            for (std::string prod : pkb.getProcedures()) {
                row.push_back(prod);
            }
            break;
        case DesignEntityType::STMT:
            for (Stmt stmt : pkb.getStmts()) {
                row.push_back(std::to_string(stmt.stmtNum));
            }
            break;
        case DesignEntityType::READ:
        case DesignEntityType::ASSIGN:
        case DesignEntityType::CALL:
        case DesignEntityType::PRINT:
        case DesignEntityType::WHILE:
        case DesignEntityType::IF:
            for (Stmt stmt : pkb.getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[entity.getType()])) {
                row.push_back(std::to_string(stmt.stmtNum));
            }
            break;
        }
        tm.join(ClauseResult{entity, row});
    }
}
