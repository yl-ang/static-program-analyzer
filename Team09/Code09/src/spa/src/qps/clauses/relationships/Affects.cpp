#include "Affects.h"

#include "../ClauseEvaluatorUtils.h"

Affects::Affects(ClauseArgument& affector, ClauseArgument& affected)
    : affector(affector), affected(affected) {}

bool Affects::validateArguments() {
    if (affector.isSynonym()) {
        Synonym first = dynamic_cast<Synonym&>(affector);
        if (first.getType() != DesignEntityType::ASSIGN) {
            return false;
        }
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
    /**
     * integer integer
     * wildcard integer / integer wildcard
     * wildcard wildcard
     */ 
    if (affector.isInteger() && affected.isInteger()) {
        return evaluateIntegerInteger(reader);
    }

    if ((affector.isWildcard() && affected.isInteger()) || (affector.isInteger() && affected.isWildcard())) {
        // return evaluateWildcardInteger(reader);
    }

    if (affector.isWildcard() && affected.isWildcard()) {
        // return evaluateWildcardWildcard(reader);
    }

    /**
     * Return types:
     * 
     * synonym synonym s1, s2
     * synonym wilcard s1
     * wildcard synonym s2
     * 
     * integer synonym s2
     * synonym integer s1
     * 
     * TRUE/FALSE
     * integer integer
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

/**
 * Helper function
*/
std::unordered_set<std::tuple<Variable, StmtNum>> getAssignStatements(PKBFacadeReader& reader) {
    std::unordered_set<Variable> allVar = reader.getVariables();
    std::unordered_set<std::tuple<Variable, StmtNum>> varAndAffectorStmtList{};
    for (Variable var : allVar) {
        std::unordered_set<StmtNum> allStmts = reader.getModifiesStatementsByVariable(var);
        std::unordered_set<StmtNum> assignStmts = ClauseEvaluatorUtils::filterStatementsByType(
                                                    reader, DesignEntityType::ASSIGN, allStmts);
        for (StmtNum assignStmt : assignStmts) {
            varAndAffectorStmtList.insert(std::make_tuple(var, assignStmt));
        }
    }
    return varAndAffectorStmtList;
}

/**
 * Helper function
*/
std::unordered_set<StmtNum> getNextStmtNums(
        const std::unordered_set<std::tuple<Variable, StmtNum>>& varAndAffectorStmtList,
        PKBFacadeReader& reader) {
    std::unordered_set<StmtNum> nextStmtNums{};
    for (const std::tuple<Variable, StmtNum>& varAndAffectorStmt : varAndAffectorStmtList) {
        // Get NextStar Control Flow - all StmtNums that follow that assign statement
        StmtNum affectorStmtNum = std::get<StmtNum>(varAndAffectorStmt);
        std::unordered_set<StmtNum> CFAffector = reader.getNexteeStar(affectorStmtNum);
        nextStmtNums.insert(CFAffector.begin(), CFAffector.end());
    }
    return nextStmtNums;
}

ClauseResult Affects::evaluateIntegerInteger(PKBFacadeReader& reader) {
    Integer affectorInt = dynamic_cast<Integer&>(affector);
    Integer affectedInt = dynamic_cast<Integer&>(affected);

    if (affectorInt == affectedInt) {
        return false;
    }

    StmtNum affectorStmtNum = std::stoi(affectorInt.getValue());
    StmtNum affectedStmtNum = std::stoi(affectedInt.getValue());

    bool usesModifiedVariable = false;

    /**
     * Check that affectedStatement Uses Modified variable
    */
    std::unordered_set<Variable> affectorVariables = reader.getModifiesVariablesByStatement(affectorStmtNum);
    std::unordered_set<Variable> affectedVariables = reader.getUsesVariablesByStatement(affectedStmtNum);
    for (Variable affectorVariable: affectorVariables) {
        for (Variable affectedVariable: affectedVariables) {
            if (affectorVariable == affectedVariable) {
                usesModifiedVariable = true;
            }
        }
    }

    /**
     * Check that Uses Modified variable is not Modified inbetween
    */
    std::unordered_set<StmtNum> nextStmtNums = reader.getNexteeStar(affectorStmtNum);

    /**
     * For each Nextee:
     * - for all statements, CHECK that does not modify the variable
     * -- if modified, mark notModifiedBetween as false, and exit loop
     * - else if goal statement, exit loop
    */
    bool modifiedBetween = false;
    for (StmtNum nextStmtNum : nextStmtNums) {
        if ((nextStmtNum == affectedStmtNum) || modifiedBetween) {
            break;
        } else {
            std::unordered_set<Variable> nextStmtVariables = reader.getModifiesVariablesByStatement(nextStmtNum);
            for (Variable nextStmtVariable: nextStmtVariables) {
                for (Variable affectorVariable: affectorVariables) {
                    if (nextStmtVariable == affectorVariable) {
                        modifiedBetween = true;
                        break;
                    }
                }
                if (modifiedBetween) {
                    break;
                }
            }
        }
    }

    return usesModifiedVariable && !(modifiedBetween);
}

ClauseResult Affects::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    bool affectorIsSynonym = affector.isSynonym();
    Synonym syn = affectorIsSynonym ? dynamic_cast<Synonym&>(affector) : dynamic_cast<Synonym&>(affected);

    SynonymValues values{};

    /**
     * 1. Get assign statements that modify their variables
    */
    std::unordered_set<std::tuple<Variable, StmtNum>> varAndAffectorStmtList = getAssignStatements(reader);

    /**
     * 2. For each tuple of modified variable and assign statement
    */
    std::unordered_set<StmtNum> nextStmtNums = getNextStmtNums(varAndAffectorStmtList, reader);

    /**
     * For each tuple of modified variable and assign statement:
     * For each CF next assign statement:
     * - iterate through variables used by the CF next assign statement,
     * 
     * - for all statements, CHECK that does not modify the variable
     * -- if modified, exit loop for that affecter statement
     * - else if ASSIGN statement, CHECK that currVariable equals modified variable
     * -- if yes, push back values
    */
    for (const std::tuple<Variable, StmtNum>& varAndAffectorStmt : varAndAffectorStmtList) {
        StmtNum stmtNum = std::get<StmtNum>(varAndAffectorStmt);
        Variable variable = std::get<Variable>(varAndAffectorStmt);

        bool modified = false;
        for (StmtNum nextStmtNum : nextStmtNums) {
            std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
            if (modified) {
                break;
            } else {
                // Checking if uses the affector variable
                for (Variable currVar : reader.getUsesVariablesByStatement(nextStmtNum)) {
                    if (stmt.value().type == StatementType::ASSIGN && currVar == variable) {
                        if (affectorIsSynonym) {
                            values.push_back(std::to_string(stmtNum));
                        } else {
                            values.push_back(std::to_string(nextStmtNum));
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
    }

    return {syn, values};
}

ClauseResult Affects::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool affectorIsSynonym = affector.isSynonym();
    Synonym syn = affectorIsSynonym ? dynamic_cast<Synonym&>(affector) : dynamic_cast<Synonym&>(affected);
    Integer integer = affectorIsSynonym ? dynamic_cast<Integer&>(affected) : dynamic_cast<Integer&>(affector);

    StmtNum stmtNum = std::stoi(integer.getValue());

    // Synonym Integer
    /**
     * Need to figure out the affector statements
    */
    // Integer Synonym
    /**
     * Need to figure out the affected statements
    */
}

ClauseResult Affects::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym affectorSyn = dynamic_cast<Synonym&>(affector);
    Synonym affectedSyn = dynamic_cast<Synonym&>(affected);

    std::vector<Synonym> synonyms{affectorSyn, affectedSyn};

    if (affectorSyn == affectedSyn) {
        return {synonyms, {}};
    }

    SynonymValues affectorValues{};
    SynonymValues affectedValues{};

    /**
     * 1. Get assign statements that modify their variables
     * Get statements that modify variables (LHS)
     * Then filter down to assign statements
     * Make tuple of modified variable and assign statement
    */
    std::unordered_set<std::tuple<Variable, StmtNum>> varAndAffectorStmtList = getAssignStatements(reader);

    /**
     * 2. For each tuple of modified variable and assign statement
     * - Get the control flow statements that follow it (like NextStar)
     * - Get the CF with all the statements
     * - Get separate list with assign statements
    */
    std::unordered_set<StmtNum> nextStmtNums = getNextStmtNums(varAndAffectorStmtList, reader);

    /**
     * For each tuple of modified variable and assign statement:
     * For each CF next assign statement:
     * - iterate through variables used by the CF next assign statement,
     * 
     * - if not ASSIGN statement, CHECK that does not modify the variable
     * -- if modified, exit loop for that affecter statement
     * - else if ASSIGN statement, CHECK that currVariable equals modified variable
     * -- if yes, push back values
    */
    for (const std::tuple<Variable, StmtNum>& varAndAffectorStmt : varAndAffectorStmtList) {
        StmtNum stmtNum = std::get<StmtNum>(varAndAffectorStmt);
        Variable variable = std::get<Variable>(varAndAffectorStmt);

        bool modified = false;
        for (StmtNum nextStmtNum : nextStmtNums) {
            std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
            if (modified) {
                break;
            } else {
                // Checking if uses the affector variable
                for (Variable currVar : reader.getUsesVariablesByStatement(nextStmtNum)) {
                    if (stmt.value().type == StatementType::ASSIGN && currVar == variable) {  
                        affectorValues.push_back(std::to_string(stmtNum));
                        affectedValues.push_back(std::to_string(nextStmtNum));
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
    }

    std::vector<Synonym> headers = {affectorSyn, affectedSyn};
    std::vector<SynonymValues> values = {affectorValues, affectedValues};
    return {headers, values};
}
