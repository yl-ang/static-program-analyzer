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
        Synonym s = *std::dynamic_pointer_cast<Synonym>(clauseArgument);
        DesignEntityType sType = s.getType();
        if (sType == DesignEntityType::VARIABLE || sType == DesignEntityType::PROCEDURE ||
            sType == DesignEntityType::CONSTANT) {
            return false;
        }
    }
    return true;
}

bool Affects::checkAssign(Synonym& synonym) {
    DesignEntityType sType = synonym.getType();
    return (sType == DesignEntityType::ASSIGN || sType == DesignEntityType::STMT);
}

bool Affects::hasCommonValue(const std::unordered_set<Variable>& set1,
                            const std::unordered_set<Variable>& set2) {
    for (const auto value : set1) {
        if (set2.find(value) != set2.end()) {
            return true;  // A common value is found
        }
    }
    return false;  // No common value found
}

ClauseResult Affects::evaluate(PKBFacadeReader& reader) {
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

// Helper Affected template (1)
template <typename Func>
void processAffected(Func func, StmtNum affectedStmtNum, PKBFacadeReader reader) {
    auto usesVariables = reader.getUsesVariablesByStatement(affectedStmtNum);

    // IMPORTANT: For each variable used
    for (auto usesVariable : usesVariables) {
        // keep track of visited
        std::unordered_set<StmtNum> visited;
        std::vector<StmtNum> queue;

        // Place the statement itself inside queue
        queue.emplace_back(affectedStmtNum);

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
                throw Exception(
                    "Not supposed to happen. Check to return empty value/ false.");
            }
            StatementType stmtType = stmt->type;

            std::unordered_set<Variable> usesVariableOneSet = {usesVariable};
            func(affectedStmtNum, stmtNum, usesVariableOneSet, stmtType, reader, queue, visited);
        }
    }
}

// Helper Affected function (2)
void Affects::handleCommonAffectedLogic(StmtNum& stmtNum, std::unordered_set<Variable>& usesVariable,
                                        StatementType& stmtType, PKBFacadeReader& reader,
                                        std::vector<StmtNum>& queue, std::unordered_set<StmtNum>& visited) {
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

// Used for BOOLEAN Affected
bool Affects::isAffectsfromAffected(StmtNum& affectedStmtNum, PKBFacadeReader& reader) {
    bool result = false;
    processAffected([&](StmtNum& affectedStmtNum, StmtNum& stmtNum, std::unordered_set<Variable>& usesVariable,
                        StatementType stmtType, PKBFacadeReader& reader,
                        std::vector<StmtNum>& queue, std::unordered_set<StmtNum>& visited) {
        auto curModifiesVariables = reader.getModifiesVariablesByStatement(stmtNum);
        if (stmtType == StatementType::ASSIGN) {
            if (hasCommonValue(usesVariable, curModifiesVariables)) {
                result = true;
            }
        }
        if (stmtType == StatementType::ASSIGN || stmtType == StatementType::READ || stmtType == StatementType::CALL) {
            if (hasCommonValue(usesVariable, curModifiesVariables)) {
                return;
            }
        }
        handleCommonAffectedLogic(stmtNum, usesVariable, stmtType, reader, queue, visited);
    }, affectedStmtNum, reader);
    return result;
}

// Used for Synonym Affected
void Affects::generateAffectsfromAffected(AffectsSet& result, StmtNum& affectedStmtNum, PKBFacadeReader& reader) {
    processAffected([&](StmtNum& affectedStmtNum, StmtNum& stmtNum, std::unordered_set<Variable>& usesVariable,
                    StatementType& stmtType, PKBFacadeReader& reader,
                    std::vector<StmtNum>& stack, std::unordered_set<StmtNum>& visited) {
        auto curModifiesVariables = reader.getModifiesVariablesByStatement(stmtNum);
        if (stmtType == StatementType::ASSIGN) {
            if (hasCommonValue(usesVariable, curModifiesVariables)) {
                result.insert({stmtNum, affectedStmtNum});
            }
        }
        if (stmtType == StatementType::ASSIGN || stmtType == StatementType::READ || stmtType == StatementType::CALL) {
            if (hasCommonValue(usesVariable, curModifiesVariables)) {
                return;
            }
        }
        handleCommonAffectedLogic(stmtNum, usesVariable, stmtType, reader, stack, visited);
    }, affectedStmtNum, reader);
}

// Helper Affector template (1)
template <typename Func>
void processAffects(Func func, StmtNum affectorStmtNum, PKBFacadeReader reader) {
    // keep track of visited
    std::unordered_set<StmtNum> visited;
    std::vector<StmtNum> queue;

    // Place the statement itself inside queue
    queue.emplace_back(affectorStmtNum);

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
            throw Exception(
                "Not supposed to happen. Check to return empty value/ false.");
        }
        StatementType stmtType = stmt.value().type;

        func(affectorStmtNum, stmtNum, modifiedVariables, stmtType, reader, queue, visited);
    }
}

// Helper Affector function (2)
void Affects::handleCommonAffectorLogic(StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                                        StatementType& stmtType, PKBFacadeReader& reader,
                                        std::vector<StmtNum>& queue, std::unordered_set<StmtNum>& visited) {
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

// Used for BOOLEAN Affector
bool Affects::isAffectsfromAffector(StmtNum& affectorStmtNum, PKBFacadeReader& reader) {
    bool result = false;
    processAffects([&](StmtNum& affectorStmtNum, StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                        StatementType stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& queue,
                        std::unordered_set<StmtNum>& visited) {
        if (stmtType == StatementType::ASSIGN) {
            auto curUsedVariables = reader.getUsesVariablesByStatement(stmtNum);
            if (hasCommonValue(modifiedVariables, curUsedVariables)) {
                result = true;
            }
        }
        handleCommonAffectorLogic(stmtNum, modifiedVariables, stmtType, reader, queue, visited);
    }, affectorStmtNum, reader);
    return result;
}

// Used for Integer/Integer Affector
bool Affects::intAffectsfromAffector(StmtNum& affectorStmtNum, StmtNum& affectedStmtNum, PKBFacadeReader& reader) {
    bool result = false;
    processAffects([&](StmtNum& affectorStmtNum, StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                        StatementType stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& queue,
                        std::unordered_set<StmtNum>& visited) {
        if (stmtType == StatementType::ASSIGN) {
            auto curUsedVariables = reader.getUsesVariablesByStatement(stmtNum);
            if (hasCommonValue(modifiedVariables, curUsedVariables) && (stmtNum == affectedStmtNum)) {
                result = true;
            }
        }
        handleCommonAffectorLogic(stmtNum, modifiedVariables, stmtType, reader, queue, visited);
    }, affectorStmtNum, reader);
    return result;
}

// Used for Synonym Affector
void Affects::generateAffectsfromAffector(AffectsSet& result, StmtNum& affectorStmtNum, PKBFacadeReader& reader) {
    processAffects([&](StmtNum& affectorStmtNum, StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                    StatementType& stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& queue,
                    std::unordered_set<StmtNum>& visited) {
        if (stmtType == StatementType::ASSIGN) {
            auto curUsedVariables = reader.getUsesVariablesByStatement(stmtNum);
            if (hasCommonValue(modifiedVariables, curUsedVariables)) {
                result.insert({affectorStmtNum, stmtNum});
            }
        }
        handleCommonAffectorLogic(stmtNum, modifiedVariables, stmtType, reader, queue, visited);
    }, affectorStmtNum, reader);
}

/**
 * Actual Evaluation functions begin here:
*/

ClauseResult Affects::evaluateWildcardInteger(PKBFacadeReader& reader) {
    bool affectorIsInteger = affector->isInteger();
    Integer integer = *std::dynamic_pointer_cast<Integer>(affectorIsInteger ? affector : affected);
    StmtNum stmtNum = std::stoi(integer.getValue());

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

    if (affectorStmt.has_value() && affectedStmt.has_value() &&
        (affectorStmt.value().type == StatementType::ASSIGN) &&
        (affectedStmt.value().type == StatementType::ASSIGN)) {
        return intAffectsfromAffector(affectorStmtNum, affectedStmtNum, reader);
    }
    return false;
}

ClauseResult Affects::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    bool affectorIsSynonym = affector->isSynonym();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(affectorIsSynonym ? affector : affected);

    std::unordered_set<StmtNum> stmtNumValues;

    if (checkAssign(syn)) {
        AffectsSet resultSet = generateAffectsRelation(reader);

        for (std::pair<StmtNum, StmtNum> result: resultSet) {
            if (affectorIsSynonym) {
                stmtNumValues.insert(result.first);
            } else {
                stmtNumValues.insert(result.second);
            }
        }
    }

    SynonymValues values;
    for (int stmtNumValue : stmtNumValues) {
        values.push_back(std::to_string(stmtNumValue));
    }
    return {syn, values};
}

ClauseResult Affects::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool affectorIsInteger = affector->isInteger();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(affectorIsInteger ? affected : affector);
    Integer integer = *std::dynamic_pointer_cast<Integer>(affectorIsInteger ? affector : affected);
    StmtNum stmtNum = std::stoi(integer.getValue());

    std::unordered_set<StmtNum> stmtNumValues;

    if (!checkAssign(syn)) {
        return {syn, {}};
    }
    std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
    if (!stmt.has_value() || stmt.value().type != StatementType::ASSIGN) {
        return  {syn, {}};
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

    SynonymValues values;
    for (int stmtNumValue : stmtNumValues) {
        values.push_back(std::to_string(stmtNumValue));
    }
    return {syn, values};
}

ClauseResult Affects::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym affectorSyn = *std::dynamic_pointer_cast<Synonym>(affector);
    Synonym affectedSyn = *std::dynamic_pointer_cast<Synonym>(affected);
    std::vector<Synonym> headers = {affectorSyn, affectedSyn};

    SynonymValues affectorValues;
    SynonymValues affectedValues;

    if (!checkAssign(affectorSyn) || !checkAssign(affectedSyn)) {
        return {headers, {{}, {}}};
    }
    AffectsSet resultSet = generateAffectsRelation(reader);
    for (std::pair<StmtNum, StmtNum> result: resultSet) {
        affectorValues.push_back(std::to_string(result.first));
        affectedValues.push_back(std::to_string(result.second));
    }

    std::vector<SynonymValues> values = {affectorValues, affectedValues};
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
