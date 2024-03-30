#include "Affects.h"

#include "../ClauseEvaluatorUtils.h"

Affects::Affects(ClauseArgument& affector, ClauseArgument& affected)
    : affector(affector), affected(affected) {}

bool Affects::validateArguments() {
    return checkSynonym(affector) && checkSynonym(affected);
}

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

bool Affects::checkAssign(ClauseArgument& clauseArgument) {
    if (clauseArgument.isSynonym()) {
        Synonym& s = dynamic_cast<Synonym&>(clauseArgument);
        DesignEntityType sType = s.getType();
        if (sType != DesignEntityType::ASSIGN && sType != DesignEntityType::STMT) {
            return false;
        }
    }
    return true;
}

/**
 * Similar logic:
 * Bothsynonyms/BothWildcards
 * SynonymInteger/WildcardInteger
 * BothIntegers
 * SynonymWildcard
*/
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

/**
 * Helper function
*/
std::unordered_set<std::pair<Variable, StmtNum>> Affects::getAssignStatements(PKBFacadeReader& reader) {
    std::unordered_set<Variable> allVar = reader.getVariables();
    std::unordered_set<std::pair<Variable, StmtNum>> varAndAffectorStmtList{};
    for (Variable var : allVar) {
        std::unordered_set<StmtNum> allStmts = reader.getModifiesStatementsByVariable(var);
        std::unordered_set<StmtNum> assignStmts = ClauseEvaluatorUtils::filterStatementsByType(
                                                    reader, DesignEntityType::ASSIGN, allStmts);
        for (StmtNum assignStmt : assignStmts) {
            varAndAffectorStmtList.insert(std::make_pair(var, assignStmt));
        }
    }
    return varAndAffectorStmtList;
}

// /**
//  * Helper function
// */
// std::unordered_set<StmtNum> Affects::getNextStmtNums(
//         const std::pair<Variable, StmtNum>& varAndAffectorStmt, PKBFacadeReader& reader) {
//     std::unordered_set<StmtNum> nextStmtNums{};
//     // Get NextStar Control Flow - all StmtNums that follow that assign statement
//     StmtNum affectorStmtNum = std::get<StmtNum>(varAndAffectorStmt);
//     std::unordered_set<StmtNum> CFAffector = reader.getNexterStar(affectorStmtNum);
//     nextStmtNums.insert(CFAffector.begin(), CFAffector.end());
//     return nextStmtNums;
// }

AffectsSet Affects::generateAffectsRelation(PKBFacadeReader& reader) {
    // get all assign statements
    std::unordered_set<Variable> allVar = reader.getVariables();
    std::unordered_set<StmtNum> allStmts;
    for (Variable var : allVar) {
        std::unordered_set<StmtNum> modifiesStmts = reader.getModifiesStatementsByVariable(var);
        allStmts.insert(modifiesStmts.begin(), modifiesStmts.end());
    }
    std::unordered_set<StmtNum> assignStmtSet = ClauseEvaluatorUtils::filterStatementsByType(
                                                reader, DesignEntityType::ASSIGN, allStmts);
    AffectsSet result;

    for (StmtNum assignStmt : assignStmtSet) {
        generateAffectsfromAffector(result, assignStmt, reader);
    }

    return result;
}

/**
 * Get Affects Relationship from Affector Statement
*/
void Affects::generateAffectsfromAffector(AffectsSet& result, StmtNum& affectorStmtNum, PKBFacadeReader& reader) {
    // keep track of visited
    std::unordered_set<StmtNum> visited;
    std::vector<StmtNum> stack;

    // get the immendiate set of next of affectorStmtNum
    auto startingSet = reader.getNexter(affectorStmtNum);
    /**
     * For each statement stmt in startingSet, this line adds stmt to the back of the stack using emplace_back(). 
     * emplace_back() is a function that constructs an object in-place at the end of the container.
    */
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
            throw QPSSemanticError();
        }
        StatementType stmtType = stmt.value().type;

        if (stmtType == StatementType::ASSIGN) {
            auto curUsedVariables = reader.getUsesVariablesByStatement(stmtNum);
            if (hasCommonValue(modifiedVariables, curUsedVariables)) {
                result.insert({affectorStmtNum, stmtNum});
            }
        }

        // if modified, skip rest of loop
        if (stmtType == StatementType::ASSIGN || stmtType == StatementType::READ || stmtType == StatementType::CALL) {
            auto curModifiesVariables = reader.getModifiesVariablesByStatement(stmtNum);
            if (hasCommonValue(modifiedVariables, curModifiesVariables)) {
                continue;
            }
        }

        auto nextStmtSet = reader.getNexter(stmtNum);

        for (const auto nextStmt : nextStmtSet) {
            if (visited.find(nextStmt) == visited.end()) {
                stack.emplace_back(nextStmt);
            }
        }
    }
}

bool Affects::hasCommonValue(const std::unordered_set<Variable>& set1, 
                            const std::unordered_set<Variable>& set2) {
    for (const auto value : set1) {
        if (set2.find(value) != set2.end()) {
            return true; // A common value is found
        }
    }
    return false; // No common value found
}

ClauseResult Affects::evaluateWildcardInteger(PKBFacadeReader& reader) {
    bool affectorIsInteger = affector.isInteger();
    Integer integer = affectorIsInteger ? dynamic_cast<Integer&>(affector) : dynamic_cast<Integer&>(affected);
    StmtNum stmtNum = std::stoi(integer.getValue());

    std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
    if (!stmt.has_value() || (stmt.value().type != StatementType::ASSIGN)) {
        return false;
    }

    // wildcard is affected
    if (affectorIsInteger) {
        AffectsSet resultSet;
        generateAffectsfromAffector(resultSet, stmtNum, reader);
        return !resultSet.empty();
    // wildcard is affector
    } else {
        AffectsSet resultSet = generateAffectsRelation(reader);
        for (const auto& pair : resultSet) {
            if (pair.second == stmtNum) {
                return true;
            }
        }
        return false;
    }
}

ClauseResult Affects::evaluateBothIntegers(PKBFacadeReader& reader) {
    Integer affectorInt = dynamic_cast<Integer&>(affector);
    Integer affectedInt = dynamic_cast<Integer&>(affected);

    StmtNum affectorStmtNum = std::stoi(affectorInt.getValue());
    StmtNum affectedStmtNum = std::stoi(affectedInt.getValue());

    std::optional<Stmt> affectorStmt = reader.getStatementByStmtNum(affectorStmtNum);
    std::optional<Stmt> affectedStmt = reader.getStatementByStmtNum(affectedStmtNum);
    if (!affectorStmt.has_value() || !affectedStmt.has_value() ||
            (affectorStmt.value().type != StatementType::ASSIGN) ||
            (affectedStmt.value().type != StatementType::ASSIGN)) {
        return false;
    }

    AffectsSet resultSet;
    generateAffectsfromAffector(resultSet, affectorStmtNum, reader);

    for (const auto& pair : resultSet) {
        if (pair.second == affectedStmtNum) {
            return true;
        }
    }
    return false;
}

ClauseResult Affects::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    bool affectorIsSynonym = affector.isSynonym();
    Synonym syn = affectorIsSynonym ? dynamic_cast<Synonym&>(affector) : dynamic_cast<Synonym&>(affected);

    SynonymValues values{};

    AffectsSet resultSet = generateAffectsRelation(reader);

    for (std::pair<StmtNum, StmtNum> result: resultSet) {
        if (affectorIsSynonym) {
            values.push_back(std::to_string(result.first));
        } else {
            values.push_back(std::to_string(result.second));
        }     
    }

    return {syn, values};
}

ClauseResult Affects::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool affectorIsInteger = affector.isInteger();
    Synonym syn = affectorIsInteger ? dynamic_cast<Synonym&>(affected) : dynamic_cast<Synonym&>(affector);
    Integer integer = affectorIsInteger ? dynamic_cast<Integer&>(affector) : dynamic_cast<Integer&>(affected);
    StmtNum stmtNum = std::stoi(integer.getValue());

    if (!checkAssign(syn)) {
        return {syn, {}};
    }

    std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
    if (!stmt.has_value() || stmt.value().type != StatementType::ASSIGN) {
        return {syn, {}};
    }

    SynonymValues values{};

    /**
     * Get control flow from established statement number
    */
    std::unordered_set<StmtNum> nextStmtNums =
        affectorIsInteger ? reader.getNexterStar(stmtNum) : reader.getNexteeStar(stmtNum);

    /**
     * If !affectorIsSynonym:
     * - start from affector statement and work downwards
     * Else:
     * - start from affected statement and work upwards
    */
    if (affectorIsInteger) {
        std::unordered_set<Variable> modifiedVariables = reader.getModifiesVariablesByStatement(stmtNum);
        for (Variable modifiedVariable : modifiedVariables) {
            bool modified = false;
            for (StmtNum nextStmtNum : nextStmtNums) {
                if (modified) {
                    break;
                }
                std::optional<Stmt> nextStmt = reader.getStatementByStmtNum(nextStmtNum);
                // Checking if uses the affector variable
                for (Variable currVar : reader.getUsesVariablesByStatement(nextStmtNum)) {
                    if (nextStmt.has_value() && nextStmt.value().type == StatementType::ASSIGN && currVar == modifiedVariable) {
                        values.push_back(std::to_string(nextStmtNum));
                    }
                }
                // Checking if modifies the affector variable AND not nextStmtNum
                for (Variable currVar : reader.getModifiesVariablesByStatement(nextStmtNum)) {
                    if (currVar == modifiedVariable) {
                        modified = true;
                        break;  // Break out of inner loop for current affector statement
                    }
                }
            }
        }
    } else {
        std::unordered_set<Variable> usesVariables = reader.getUsesVariablesByStatement(stmtNum);
        for (Variable usesVariable : usesVariables) {
            bool modified = false;
            for (StmtNum nextStmtNum : nextStmtNums) {
                if (modified) {
                    break;
                }
                std::optional<Stmt> nextStmt = reader.getStatementByStmtNum(nextStmtNum);
                // Checking if modified affected variable
                for (Variable modifiedVar : reader.getModifiesVariablesByStatement(nextStmtNum)) {
                    if (modifiedVar == usesVariable) {
                        // If statement assign then mark as value element
                        if (nextStmt.has_value() && nextStmt.value().type == StatementType::ASSIGN) {
                            values.push_back(std::to_string(nextStmtNum));
                        }
                        modified = true;
                        break;
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

    AffectsSet resultSet = generateAffectsRelation(reader);
    
    for (std::pair<StmtNum, StmtNum> result: resultSet) {
        affectorValues.push_back(std::to_string(result.first));
        affectedValues.push_back(std::to_string(result.second));
    }

    std::vector<SynonymValues> values = {affectorValues, affectedValues};
    return {headers, values};
}

ClauseResult Affects::evaluateBothWildcards(PKBFacadeReader& reader) {
    std::unordered_set<std::pair<Variable, StmtNum>> varAndAffectorStmtList = getAssignStatements(reader);
    

    for (const std::pair<Variable, StmtNum>& varAndAffectorStmt : varAndAffectorStmtList) {
        std::unordered_set<StmtNum> nextStmtNums = getNextStmtNums(varAndAffectorStmt, reader);
        StmtNum stmtNum = std::get<StmtNum>(varAndAffectorStmt);
        Variable variable = std::get<Variable>(varAndAffectorStmt);
        bool modified = false;
        for (StmtNum nextStmtNum : nextStmtNums) {
            std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
            if (modified) {
                break;
            }
            // Checking if uses the affector variable
            for (Variable currVar : reader.getUsesVariablesByStatement(nextStmtNum)) {
                if (!stmt.has_value()) {
                    return false;
                } else { 
                    if (stmt.value().type == StatementType::ASSIGN && currVar == variable) {
                        return true;
                    }
                }
            }
            // Checking if modifies the affector variable AND not nextStmtNum
            for (Variable currVar : reader.getModifiesVariablesByStatement(nextStmtNum)) {
                if (currVar == variable) {
                    modified = true;
                    break;  // Break out of inner loop for current affector statement
                }
            }
        }
    }
    return false;
}
