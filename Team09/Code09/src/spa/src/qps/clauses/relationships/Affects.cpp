#include "Affects.h"

#include "../ClauseEvaluatorUtils.h"

Affects::Affects(ClauseArgument& affector, ClauseArgument& affected)
    : affector(affector), affected(affected) {}

bool Affects::validateArguments() {
    return checkSynonym(affector) && checkSynonym(affected);
}

/**
 * Various checks below
*/
bool Affects::checkSynonym(ClauseArgument& clauseArgument) {
    if (clauseArgument.isSynonym()) {
        Synonym& s = dynamic_cast<Synonym&>(clauseArgument);
        DesignEntityType sType = s.getType();
        if (sType != DesignEntityType::ASSIGN && sType != DesignEntityType::READ &&
            sType != DesignEntityType::CALL && sType != DesignEntityType::PRINT &&
            sType != DesignEntityType::WHILE && sType != DesignEntityType::IF &&
            sType != DesignEntityType::STMT) {
            return false;
        }
    }
    return true;
}

bool Affects::checkAssign(Synonym& synonym) {
    DesignEntityType sType = synonym.getType();
    if (sType == DesignEntityType::ASSIGN || sType == DesignEntityType::STMT) {
        return true;
    }
    return false;
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
    if (affector.isInteger() && affected.isInteger()) {
        return evaluateBothIntegers(reader);
    }
    if (affector.isWildcard() && affected.isWildcard()) {
        return evaluateBothWildcards(reader);
    }
    if ((affector.isWildcard() && affected.isInteger()) || (affector.isInteger() && affected.isWildcard())) {
        return evaluateWildcardInteger(reader);
    }

    /**
     * Returns either s1, s2, or both
    */
    if ((affector.isInteger() && affected.isSynonym()) || (affector.isSynonym() && affected.isInteger())) {
        return evaluateSynonymInteger(reader);
    }
    if ((affector.isWildcard() && affected.isSynonym()) || (affector.isSynonym() && affected.isWildcard())) {
        return evaluateSynonymWildcard(reader);
    }

    return evaluateBothSynonyms(reader);
}

// Get All Affects Relationship
AffectsSet Affects::generateAffectsRelation(PKBFacadeReader& reader) {
    // get all assign statements
    std::unordered_set<Stmt> allStmts = reader.getStmts();
    std::unordered_set<StmtNum> assignStmtSet;
    for (Stmt stmt : allStmts) {
        if (stmt.type == StatementType::ASSIGN) {
            assignStmtSet.insert(stmt.stmtNum);
        }
    }
    AffectsSet result;
    for (StmtNum assignStmt : assignStmtSet) {
        generateAffectsfromAffector(result, assignStmt, reader);
    }
    return result;
}

// Helper Affected template (1)
template <typename Func>
void processAffected(const Func& func, const StmtNum& affectedStmtNum, const PKBFacadeReader& reader) {
    auto usesVariables = reader.getUsesVariablesByStatement(affectedStmtNum);

    // IMPORTANT: For each variable used
    for (auto usesVariable : usesVariables) {
        // keep track of visited
        std::unordered_set<StmtNum> visited;
        std::vector<StmtNum> stack;

        // Place the statement itself inside stack
        stack.emplace_back(affectedStmtNum);

        // get the immendiate set of next of affectedStmtNum
        auto startingSet = reader.getNextee(affectedStmtNum);
        /**
         * For each statement stmt in startingSet, this line adds stmt to the back of the stack using emplace_back(). 
         * emplace_back() is a function that constructs an object in-place at the end of the container.
        */
        for (auto stmt : startingSet) {
            stack.emplace_back(stmt);
        }
        // get variables used by affectedStmtNum
        while (!stack.empty()) {
            // Get first element of stack and erase
            StmtNum stmtNum = stack[0];
            stack.erase(stack.begin());

            // If not visited, continue
            // insert into visited
            if (visited.find(stmtNum) != visited.end()) {
                continue;
            }
            visited.insert(stmtNum);

            // get statement type of statement
            std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
            if (!stmt.has_value()) {
                throw Exception("Statement does not have value");
            }
            StatementType stmtType = stmt.value().type;

            std::unordered_set<Variable> usesVariableOneSet = {usesVariable};
            func(affectedStmtNum, stmtNum, usesVariableOneSet, stmtType, reader, stack, visited);
        }
    }
}

// Helper Affected function (2)
void Affects::handleCommonAffectedLogic(StmtNum& stmtNum, std::unordered_set<Variable>& usesVariable,
                                        StatementType& stmtType, PKBFacadeReader& reader,
                                        std::vector<StmtNum>& stack, std::unordered_set<StmtNum>& visited) {
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
            stack.emplace_back(previousStmt);
        }
    }
}

// Used for BOOLEAN Affected
bool Affects::isAffectsfromAffected(StmtNum& affectedStmtNum, PKBFacadeReader& reader) {
    bool result = false;
    processAffected([&](StmtNum& affectedStmtNum, StmtNum& stmtNum, std::unordered_set<Variable>& usesVariable,
                        StatementType stmtType, PKBFacadeReader& reader,
                        std::vector<StmtNum>& stack, std::unordered_set<StmtNum>& visited) {
        if (stmtType == StatementType::ASSIGN || stmtType == StatementType::READ || stmtType == StatementType::CALL) {
            auto curModifiesVariables = reader.getModifiesVariablesByStatement(stmtNum);
            if (hasCommonValue(usesVariable, curModifiesVariables)) {
                if (stmtType == StatementType::ASSIGN) {
                    result = true;
                }
                return;
            }
        }
        handleCommonAffectedLogic(stmtNum, usesVariable, stmtType, reader, stack, visited);
    }, affectedStmtNum, reader);
    return result;
}

// Used for Synonym Affected
void Affects::generateAffectsfromAffected(AffectsSet& result, StmtNum& affectedStmtNum, PKBFacadeReader& reader) {
    processAffected([&](StmtNum& affectedStmtNum, StmtNum& stmtNum, std::unordered_set<Variable>& usesVariable,
                    StatementType& stmtType, PKBFacadeReader& reader,
                    std::vector<StmtNum>& stack, std::unordered_set<StmtNum>& visited) {
        if (stmtType == StatementType::ASSIGN || stmtType == StatementType::READ || stmtType == StatementType::CALL) {
            auto curModifiesVariables = reader.getModifiesVariablesByStatement(stmtNum);
            if (hasCommonValue(usesVariable, curModifiesVariables)) {
                if (stmtType == StatementType::ASSIGN) {
                    result.insert({stmtNum, affectedStmtNum});
                }
                return;
            }
        }
        handleCommonAffectedLogic(stmtNum, usesVariable, stmtType, reader, stack, visited);
    }, affectedStmtNum, reader);
}

// Helper Affector template (1)
template <typename Func>
void processAffects(const Func& func, const StmtNum& affectorStmtNum, const PKBFacadeReader& reader) {
    // keep track of visited
    std::unordered_set<StmtNum> visited;
    std::vector<StmtNum> stack;

    // Place the statement itself inside stack
    stack.emplace_back(affectorStmtNum);

    // get the immediate set of next of affectorStmtNum
    auto startingSet = reader.getNexter(affectorStmtNum);
    for (auto stmt : startingSet) {
        stack.emplace_back(stmt);
    }
    // get variables modified by affectorStmtNum
    auto modifiedVariables = reader.getModifiesVariablesByStatement(affectorStmtNum);

    while (!stack.empty()) {
        // Get first element of stack and erase
        StmtNum stmtNum = stack[0];
        stack.erase(stack.begin());

        // If not visited, continue
        // insert into visited
        if (visited.find(stmtNum) != visited.end()) {
            continue;
        }
        visited.insert(stmtNum);

        // get statement type of statement
        std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
        if (!stmt.has_value()) {
            throw Exception("Statement does not have value");
        }
        StatementType stmtType = stmt.value().type;

        func(affectorStmtNum, stmtNum, modifiedVariables, stmtType, reader, stack, visited);
    }
}

// Helper Affector function (2)
void Affects::handleCommonAffectorLogic(StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                                        StatementType& stmtType, PKBFacadeReader& reader,
                                        std::vector<StmtNum>& stack, std::unordered_set<StmtNum>& visited) {
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
            stack.emplace_back(nextStmt);
        }
    }
}

// Used for BOOLEAN Affector
bool Affects::isAffectsfromAffector(StmtNum& affectorStmtNum, PKBFacadeReader& reader) {
    bool result = false;
    processAffects([&](StmtNum& affectorStmtNum, StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                        StatementType stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& stack,
                        std::unordered_set<StmtNum>& visited) {
        if (stmtType == StatementType::ASSIGN) {
            auto curUsedVariables = reader.getUsesVariablesByStatement(stmtNum);
            if (hasCommonValue(modifiedVariables, curUsedVariables)) {
                result = true;
            }
        }
        handleCommonAffectorLogic(stmtNum, modifiedVariables, stmtType, reader, stack, visited);
    }, affectorStmtNum, reader);
    return result;
}

// Used for Integer/Integer Affector
bool Affects::intAffectsfromAffector(StmtNum& affectorStmtNum, StmtNum& affectedStmtNum, PKBFacadeReader& reader) {
    bool result = false;
    processAffects([&](StmtNum& affectorStmtNum, StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                        StatementType stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& stack,
                        std::unordered_set<StmtNum>& visited) {
        if (stmtType == StatementType::ASSIGN) {
            auto curUsedVariables = reader.getUsesVariablesByStatement(stmtNum);
            if (hasCommonValue(modifiedVariables, curUsedVariables) && (stmtNum == affectedStmtNum)) {
                result = true;
            }
        }
        handleCommonAffectorLogic(stmtNum, modifiedVariables, stmtType, reader, stack, visited);
    }, affectorStmtNum, reader);
    return result;
}

// Used for Synonym Affector
void Affects::generateAffectsfromAffector(AffectsSet& result, StmtNum& affectorStmtNum, PKBFacadeReader& reader) {
    processAffects([&](StmtNum& affectorStmtNum, StmtNum& stmtNum, std::unordered_set<Variable>& modifiedVariables,
                    StatementType& stmtType, PKBFacadeReader& reader, std::vector<StmtNum>& stack,
                    std::unordered_set<StmtNum>& visited) {
        if (stmtType == StatementType::ASSIGN) {
            auto curUsedVariables = reader.getUsesVariablesByStatement(stmtNum);
            if (hasCommonValue(modifiedVariables, curUsedVariables)) {
                result.insert({affectorStmtNum, stmtNum});
            }
        }
        handleCommonAffectorLogic(stmtNum, modifiedVariables, stmtType, reader, stack, visited);
    }, affectorStmtNum, reader);
}

/**
 * Actual Evaluation functions begin here:
*/

ClauseResult Affects::evaluateWildcardInteger(PKBFacadeReader& reader) {
    bool affectorIsInteger = affector.isInteger();
    Integer integer = affectorIsInteger ? dynamic_cast<Integer&>(affector) : dynamic_cast<Integer&>(affected);
    StmtNum stmtNum = std::stoi(integer.getValue());

    std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
    if (stmt.has_value() && (stmt.value().type == StatementType::ASSIGN)) {
        AffectsSet resultSet;

        // wildcard is affected
        if (affectorIsInteger) {
            return isAffectsfromAffector(stmtNum, reader);
        // wildcard is affector
        } else {
            return isAffectsfromAffected(stmtNum, reader);
        }
    }
    return false;
}

ClauseResult Affects::evaluateBothIntegers(PKBFacadeReader& reader) {
    Integer affectorInt = dynamic_cast<Integer&>(affector);
    Integer affectedInt = dynamic_cast<Integer&>(affected);

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
    bool affectorIsSynonym = affector.isSynonym();
    Synonym syn = affectorIsSynonym ? dynamic_cast<Synonym&>(affector) : dynamic_cast<Synonym&>(affected);

    SynonymValues values{};

    if (checkAssign(syn)) {
        AffectsSet resultSet = generateAffectsRelation(reader);

        for (std::pair<StmtNum, StmtNum> result: resultSet) {
            if (affectorIsSynonym) {
                values.push_back(std::to_string(result.first));
            } else {
                values.push_back(std::to_string(result.second));
            }
        }
    }

    return {syn, values};
}

ClauseResult Affects::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool affectorIsInteger = affector.isInteger();
    Synonym syn = affectorIsInteger ? dynamic_cast<Synonym&>(affected) : dynamic_cast<Synonym&>(affector);
    Integer integer = affectorIsInteger ? dynamic_cast<Integer&>(affector) : dynamic_cast<Integer&>(affected);
    StmtNum stmtNum = std::stoi(integer.getValue());

    SynonymValues values{};

    if (checkAssign(syn)) {
        std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
        if (stmt.has_value() && stmt.value().type == StatementType::ASSIGN) {
            AffectsSet resultSet;
            // synonym is affected
            if (affectorIsInteger) {
                generateAffectsfromAffector(resultSet, stmtNum, reader);
                for (const auto& pair : resultSet) {
                    if (pair.first == stmtNum) {
                        values.push_back(std::to_string(pair.second));
                    }
                }
            // synonym is affector
            } else {
                generateAffectsfromAffected(resultSet, stmtNum, reader);
                for (const auto& pair : resultSet) {
                    if (pair.second == stmtNum) {
                        values.push_back(std::to_string(pair.first));
                    }
                }
            }
        }
    }

    return {syn, values};
}

ClauseResult Affects::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym affectorSyn = dynamic_cast<Synonym&>(affector);
    Synonym affectedSyn = dynamic_cast<Synonym&>(affected);
    std::vector<Synonym> synonyms{affectorSyn, affectedSyn};

    SynonymValues affectorValues{};
    SynonymValues affectedValues{};

    std::vector<Synonym> headers = {affectorSyn, affectedSyn};

    if (checkAssign(affectorSyn) && checkAssign(affectedSyn)) {
        AffectsSet resultSet = generateAffectsRelation(reader);
        for (std::pair<StmtNum, StmtNum> result: resultSet) {
            affectorValues.push_back(std::to_string(result.first));
            affectedValues.push_back(std::to_string(result.second));
        }
    }

    std::vector<SynonymValues> values = {affectorValues, affectedValues};
    return {headers, values};
}

ClauseResult Affects::evaluateBothWildcards(PKBFacadeReader& reader) {
    // get all assign statements
    std::unordered_set<Stmt> allStmts = reader.getStmts();
    std::unordered_set<StmtNum> assignStmtSet;
    for (Stmt stmt : allStmts) {
        if (stmt.type == StatementType::ASSIGN) {
            assignStmtSet.insert(stmt.stmtNum);
        }
    }
    AffectsSet result;

    for (StmtNum assignStmt : assignStmtSet) {
        if (isAffectsfromAffector(assignStmt, reader)) {
            return true;
        }
    }

    return false;
}
