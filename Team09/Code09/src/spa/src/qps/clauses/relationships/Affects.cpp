#include "Affects.h"

#include "../ClauseEvaluatorUtils.h"

Affects::Affects(std::shared_ptr<ClauseArgument> affector, std::shared_ptr<ClauseArgument> affected)
    : affector(affector), affected(affected) {}

bool Affects::validateArguments() {
    return checkSynonym(affector) && checkSynonym(affected);
}

/**
 * Various checks below
 */
bool Affects::checkSynonym(std::shared_ptr<ClauseArgument> clauseArgument) {
    if (clauseArgument->isSynonym()) {
        std::shared_ptr<Synonym> s = std::dynamic_pointer_cast<Synonym>(clauseArgument);
        DesignEntityType sType = s->getType();
        if (sType == DesignEntityType::VARIABLE || sType == DesignEntityType::PROCEDURE ||
            sType == DesignEntityType::CONSTANT) {
            return false;
        }
    }
    return true;
}

bool Affects::checkAssign(std::shared_ptr<Synonym> synonym) {
    DesignEntityType sType = synonym->getType();
    return (sType == DesignEntityType::ASSIGN || sType == DesignEntityType::STMT);
}

bool Affects::hasCommonValue(const std::unordered_set<Variable>& set1, const std::unordered_set<Variable>& set2) {
    for (const auto value : set1) {
        if (set2.find(value) != set2.end()) {
            return true;  // A common value is found
        }
    }
    return false;  // No common value found
}

ClauseResult Affects::evaluate(PKBFacadeReader& reader, EvaluationDb&) {
    /**
     * Returns TRUE/FALSE
     */
    if (affector->isInteger() && affected->isInteger()) {
        return evaluateBothIntegers(reader);
    }
    if (affector->isWildcard() && affected->isWildcard()) {
        return evaluateBothWildcards(reader);
    }
    if ((affector->isWildcard() && affected->isInteger()) || (affector->isInteger() && affected->isWildcard())) {
        return evaluateWildcardInteger(reader);
    }

    /**
     * Returns either s1, s2, or both
     */
    if ((affector->isInteger() && affected->isSynonym()) || (affector->isSynonym() && affected->isInteger())) {
        return evaluateSynonymInteger(reader);
    }
    if ((affector->isWildcard() && affected->isSynonym()) || (affector->isSynonym() && affected->isWildcard())) {
        return evaluateSynonymWildcard(reader);
    }

    return evaluateBothSynonyms(reader);
}

// Get All Affects Relationship
AffectsSet Affects::generateAffectsRelation(PKBFacadeReader& reader) {
    // get all assign statements
    std::unordered_set<Stmt> assignStmtSet = reader.getStatementsByType(StatementType::ASSIGN);
    AffectsSet result;
    for (Stmt assignStmt : assignStmtSet) {
        generateAffectsfromAffector(result, assignStmt.stmtNum, reader);
    }
    return result;
}

// Helper Affected function (2)
void Affects::handleCommonAffectedLogic(StmtNum& stmtNum, std::unordered_set<Variable>& usesVariable,
                                        StatementType& stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& queue,
                                        std::unordered_set<StmtNum>& visited) {
    // if modified, skip rest of loop
    if (stmtType == StatementType::ASSIGN || stmtType == StatementType::READ || stmtType == StatementType::CALL) {
        auto curModifiesVariables = reader.getModifiesVariablesByStatement(stmtNum);
        if (hasCommonValue(usesVariable, curModifiesVariables)) {
            return;
        }
    }

    auto previousStmtSet = reader.getNextee(stmtNum);
    for (const auto previousStmt : previousStmtSet) {
        if (visited.find(previousStmt) == visited.end()) {
            queue.emplace_back(previousStmt);
        }
    }
}

// Helper Affected template (1)
template <typename Func>
void processAffected(Func func, StmtNum affectedStmtNum, PKBFacadeReader reader) {
    auto usesVariables = reader.getUsesVariablesByStatement(affectedStmtNum);

    // IMPORTANT: For each variable used
    for (auto usesVariable : usesVariables) {
        // keep track of visited
        std::unordered_set<StmtNum> visited;
        std::vector<StmtNum> queue;

        // get the immendiate set of next of affectedStmtNum
        auto startingSet = reader.getNextee(affectedStmtNum);
        /**
         * For each statement stmt in startingSet, this line adds stmt to the back of the queue using emplace_back().
         * emplace_back() is a function that constructs an object in-place at the end of the container.
         */
        for (auto stmt : startingSet) {
            queue.emplace_back(stmt);
        }
        // get variables used by affectedStmtNum
        while (!queue.empty()) {
            // Get first element of queue and erase
            StmtNum stmtNum = queue[0];
            queue.erase(queue.begin());

            // If not visited, continue
            // insert into visited
            if (visited.find(stmtNum) != visited.end()) {
                continue;
            }
            visited.insert(stmtNum);

            // get statement type of statement
            std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
            if (!stmt.has_value()) {
                throw Exception("Not supposed to happen. Check to return empty value/ false.");
            }
            StatementType stmtType = stmt->type;

            std::unordered_set<Variable> usesVariableOneSet = {usesVariable};
            auto curModifiesVariables = reader.getModifiesVariablesByStatement(stmtNum);
            if (stmtType == StatementType::ASSIGN || stmtType == StatementType::READ ||
                stmtType == StatementType::CALL) {
                func(usesVariableOneSet, curModifiesVariables, stmtNum, stmtType);
            }
            Affects::handleCommonAffectedLogic(stmtNum, usesVariableOneSet, stmtType, reader, queue, visited);
        }
    }
}

// Used for BOOLEAN Affected
bool Affects::isAffectsfromAffected(StmtNum& affectedStmtNum, PKBFacadeReader& reader) {
    bool result = false;
    processAffected(
        [&](std::unordered_set<Variable>& usesVariable, std::unordered_set<Variable>& curModifiesVariables,
            StmtNum& stmtNum, StatementType stmtType) {
            result =
                result || (stmtType == StatementType::ASSIGN && hasCommonValue(usesVariable, curModifiesVariables));
            return;
        },
        affectedStmtNum, reader);
    return result;
}

// Used for Synonym Affected
void Affects::generateAffectsfromAffected(AffectsSet& result, StmtNum& affectedStmtNum, PKBFacadeReader& reader) {
    processAffected(
        [&](std::unordered_set<Variable>& usesVariable, std::unordered_set<Variable>& curModifiesVariables,
            StmtNum& stmtNum, StatementType stmtType) {
            if (hasCommonValue(usesVariable, curModifiesVariables)) {
                if (stmtType == StatementType::ASSIGN) {
                    result.insert({stmtNum, affectedStmtNum});
                }
                return;
            }
        },
        affectedStmtNum, reader);
}

// Helper Affector function (2)
void Affects::handleCommonAffectorLogic(StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                                        StatementType& stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& queue,
                                        std::unordered_set<StmtNum>& visited) {
    // if modified, skip rest of loop
    if (stmtType == StatementType::ASSIGN || stmtType == StatementType::READ || stmtType == StatementType::CALL) {
        auto curModifiesVariables = reader.getModifiesVariablesByStatement(stmtNum);
        if (hasCommonValue(modifiedVariables, curModifiesVariables)) {
            return;
        }
    }
    auto nextStmtSet = reader.getNexter(stmtNum);
    for (const auto nextStmt : nextStmtSet) {
        if (visited.find(nextStmt) == visited.end()) {
            queue.emplace_back(nextStmt);
        }
    }
}

// Helper Affector template (1)
template <typename Func>
void processAffects(Func func, StmtNum affectorStmtNum, PKBFacadeReader reader) {
    // keep track of visited
    std::unordered_set<StmtNum> visited;
    std::vector<StmtNum> queue;

    // get the immediate set of next of affectorStmtNum
    auto startingSet = reader.getNexter(affectorStmtNum);
    for (auto stmt : startingSet) {
        queue.emplace_back(stmt);
    }
    // get variables modified by affectorStmtNum
    auto modifiedVariables = reader.getModifiesVariablesByStatement(affectorStmtNum);

    while (!queue.empty()) {
        // Get first element of queue and erase
        StmtNum stmtNum = queue[0];
        queue.erase(queue.begin());

        // If not visited, continue
        // insert into visited
        if (visited.find(stmtNum) != visited.end()) {
            continue;
        }
        visited.insert(stmtNum);

        // get statement type of statement
        std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
        if (!stmt.has_value()) {
            throw Exception("Not supposed to happen. Check to return empty value/ false.");
        }
        StatementType stmtType = stmt.value().type;

        // func(affectorStmtNum, stmtNum, modifiedVariables, stmtType, reader, queue, visited);
        if (stmtType == StatementType::ASSIGN) {
            auto curUsedVariables = reader.getUsesVariablesByStatement(stmtNum);
            func(modifiedVariables, curUsedVariables, stmtNum);
        }
        Affects::handleCommonAffectorLogic(stmtNum, modifiedVariables, stmtType, reader, queue, visited);
    }
}

// Used for BOOLEAN Affector
bool Affects::isAffectsfromAffector(StmtNum& affectorStmtNum, PKBFacadeReader& reader) {
    bool result = false;
    processAffects([&](std::unordered_set<Variable>& modifiedVariables, std::unordered_set<Variable>& curUsedVariables,
                       StmtNum& stmtNum) { result = result || hasCommonValue(modifiedVariables, curUsedVariables); },
                   affectorStmtNum, reader);
    return result;
}

// Used for Integer/Integer Affector
bool Affects::intAffectsfromAffector(StmtNum& affectorStmtNum, StmtNum& affectedStmtNum, PKBFacadeReader& reader) {
    bool result = false;
    processAffects(
        [&](std::unordered_set<Variable>& modifiedVariables, std::unordered_set<Variable>& curUsedVariables,
            StmtNum& stmtNum) {
            result = result || (hasCommonValue(modifiedVariables, curUsedVariables) && (stmtNum == affectedStmtNum));
        },
        affectorStmtNum, reader);
    return result;
}

// Used for Synonym Affector
void Affects::generateAffectsfromAffector(AffectsSet& result, StmtNum& affectorStmtNum, PKBFacadeReader& reader) {
    processAffects(
        [&](std::unordered_set<Variable>& modifiedVariables, std::unordered_set<Variable>& curUsedVariables,
            StmtNum& stmtNum) {
            if (hasCommonValue(modifiedVariables, curUsedVariables)) {
                result.insert({affectorStmtNum, stmtNum});
            }
        },
        affectorStmtNum, reader);
}

/**
 * Actual Evaluation functions begin here:
 */

ClauseResult Affects::evaluateWildcardInteger(PKBFacadeReader& reader) {
    bool affectorIsInteger = affector->isInteger();
    std::shared_ptr<ClauseArgument> integer = affectorIsInteger ? affector : affected;
    StmtNum stmtNum = std::stoi(integer->getValue());

    std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
    if (!stmt.has_value() || stmt.value().type != StatementType::ASSIGN) {
        return false;
    }

    AffectsSet resultSet;
    // wildcard is affected
    if (affectorIsInteger) {
        return isAffectsfromAffector(stmtNum, reader);
        // wildcard is affector
    } else {
        return isAffectsfromAffected(stmtNum, reader);
    }
}

ClauseResult Affects::evaluateBothIntegers(PKBFacadeReader& reader) {
    Integer affectorInt = *std::dynamic_pointer_cast<Integer>(affector);
    Integer affectedInt = *std::dynamic_pointer_cast<Integer>(affected);

    StmtNum affectorStmtNum = std::stoi(affectorInt.getValue());
    StmtNum affectedStmtNum = std::stoi(affectedInt.getValue());

    std::optional<Stmt> affectorStmt = reader.getStatementByStmtNum(affectorStmtNum);
    std::optional<Stmt> affectedStmt = reader.getStatementByStmtNum(affectedStmtNum);

    if (affectorStmt.has_value() && affectedStmt.has_value() && (affectorStmt.value().type == StatementType::ASSIGN) &&
        (affectedStmt.value().type == StatementType::ASSIGN)) {
        return intAffectsfromAffector(affectorStmtNum, affectedStmtNum, reader);
    }
    return false;
}

ClauseResult Affects::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    bool affectorIsSynonym = affector->isSynonym();
    std::shared_ptr<Synonym> syn = std::dynamic_pointer_cast<Synonym>(affectorIsSynonym ? affector : affected);

    std::unordered_set<StmtNum> stmtNumValues;

    if (checkAssign(syn)) {
        AffectsSet resultSet = generateAffectsRelation(reader);

        for (std::pair<StmtNum, StmtNum> result : resultSet) {
            if (affectorIsSynonym) {
                stmtNumValues.insert(result.first);
            } else {
                stmtNumValues.insert(result.second);
            }
        }
    }

    std::vector<Row> values;
    for (int stmtNumValue : stmtNumValues) {
        values.push_back(Row{{syn->getName(), std::to_string(stmtNumValue)}});
    }

    return {*syn, values};
}

ClauseResult Affects::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool affectorIsInteger = affector->isInteger();
    std::shared_ptr<Synonym> syn = std::dynamic_pointer_cast<Synonym>(affectorIsInteger ? affected : affector);
    std::shared_ptr<ClauseArgument> integer = affectorIsInteger ? affector : affected;
    StmtNum stmtNum = std::stoi(integer->getValue());

    std::unordered_set<StmtNum> stmtNumValues;

    if (!checkAssign(syn)) {
        return {*syn, {}};
    }
    std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
    if (!stmt.has_value() || stmt.value().type != StatementType::ASSIGN) {
        return {*syn, {}};
    }
    AffectsSet resultSet;
    // synonym is affected
    if (affectorIsInteger) {
        generateAffectsfromAffector(resultSet, stmtNum, reader);
        for (const auto& pair : resultSet) {
            if (pair.first == stmtNum) {
                stmtNumValues.insert(pair.second);
            }
        }
        // synonym is affector
    } else {
        generateAffectsfromAffected(resultSet, stmtNum, reader);
        for (const auto& pair : resultSet) {
            if (pair.second == stmtNum) {
                stmtNumValues.insert(pair.first);
            }
        }
    }

    std::vector<Row> values;
    for (int stmtNumValue : stmtNumValues) {
        values.push_back(Row{{syn->getName(), std::to_string(stmtNumValue)}});
    }
    return {*syn, values};
}

ClauseResult Affects::evaluateBothSynonyms(PKBFacadeReader& reader) {
    std::shared_ptr<Synonym> affectorSyn = std::dynamic_pointer_cast<Synonym>(affector);
    std::shared_ptr<Synonym> affectedSyn = std::dynamic_pointer_cast<Synonym>(affected);
    std::vector<Synonym> headers = {*affectorSyn, *affectedSyn};

    std::vector<Row> values{};

    if (!checkAssign(affectorSyn) || !checkAssign(affectedSyn)) {
        return {headers, {}};
    }
    AffectsSet resultSet = generateAffectsRelation(reader);
    for (std::pair<StmtNum, StmtNum> result : resultSet) {
        // account for if affectorSyn is same syn as affectedSyn
        if (*affectorSyn == *affectedSyn) {
            if (result.first == result.second) {
                values.push_back(Row{{affectorSyn->getName(), std::to_string(result.first)},
                                     {affectedSyn->getName(), std::to_string(result.second)}});
            }
        } else {
            values.push_back(Row{{affectorSyn->getName(), std::to_string(result.first)},
                                 {affectedSyn->getName(), std::to_string(result.second)}});
        }
    }

    return {headers, values};
}

ClauseResult Affects::evaluateBothWildcards(PKBFacadeReader& reader) {
    // get all assign statements
    std::unordered_set<Stmt> assignStmtSet = reader.getStatementsByType(StatementType::ASSIGN);
    AffectsSet result;

    for (Stmt assignStmt : assignStmtSet) {
        if (isAffectsfromAffector(assignStmt.stmtNum, reader)) {
            return true;
        }
    }

    return false;
}
