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
    // if (isSimpleResult()) {
    //     return {reader.hasParentRelationship(parent, child)};
    // }
    /**
     * synonym synonym
     * synonym integer / integer synonym 
     * wildcard synonym / synonym wilcard
     * 
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

ClauseResult Affects::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    bool affectorIsSynonym = affector.isSynonym();
    Synonym syn = affectorIsSynonym ? dynamic_cast<Synonym&>(affector) : dynamic_cast<Synonym&>(affected);

    std::unordered_set<Stmt> allStmts{};
    allStmts = reader.getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityType::ASSIGN]);

    std::unordered_set<StmtNum> uniqueValues{};

    for (Stmt stmt : allStmts) {
        StmtNum stmtNum = stmt.stmtNum;
        if (affectorIsSynonym) {
            
        } else {
            
        }
    }
}

ClauseResult Affects::evaluateSynonymInteger(PKBFacadeReader& reader) {}

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
     * Original plan:
     * 1. Get the variable modified by the statement
     * 2. See if this variable is modified by any statement below, following control flow
     * 3. Stop immediately after you hit one that modifies the variable 
     * (as other following statements would break no modifies inbetween rule)
    */

    /**
     * 1. Get assign statements that modify their variables
     * 
     * Get statements that modify variables (LHS)
     * Then filter down to assign statements
     * Make tuple of modified variable and assign statement
    */
    std::unordered_set<Variable> vars = reader.getVariables();
    std::unordered_set<std::tuple<Variable, StmtNum>> varAndAssignStmtList{};
    for (Variable var : vars) {
        std::unordered_set<StmtNum> stmts = reader.getModifiesStatementsByVariable(var);
        std::unordered_set<StmtNum> assignStmts = ClauseEvaluatorUtils::filterStatementsByType(
                                                reader, affectorSyn.getType(), stmts);
        for (StmtNum stmt : assignStmts) {
            varAndAssignStmtList.insert(std::make_tuple(var, stmt));
        }
    }

    /**
     * 2. For each tuple of modified variable and assign statement
     * - Get the control flow statements that follow it (like NextStar)
     * - Filter out the assign statements
    */
    for (const std::tuple<Variable, StmtNum>& varAndAssignStmt : varAndAssignStmtList) {
        // Get NextStar Control Flow - all StmtNums that follow that assign statement
        Variable variable = std::get<Variable>(varAndAssignStmt);
        StmtNum stmtNum = std::get<StmtNum>(varAndAssignStmt);
        std::unordered_set<StmtNum> nextStmtNums = reader.getNexteeStar(stmtNum);
        std::unordered_set<StmtNum> nextAssignStmtNums{};
        // Filtering out Assign Statements
        for (StmtNum nextStmtNum : nextStmtNums) {
            std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
            if (stmt.value().type == DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityType::ASSIGN]) {
                nextAssignStmtNums.insert(nextStmtNum);
            }
        }

        /**
         * For each tuple of modified variable and assign statement:
         * [Still in above loop]
         * 
         * For each CF next assign statement:
         * - iterate through variables used by the CF next assign statement,
         *   until you hit the first that satisifies below condition:
         * - if currVariable equals modified variable, push back values and exit loops
         * - Then exit the loop for THIS assign statement
        */
        bool found = false;
        for (StmtNum nextAssignStmtNum : nextAssignStmtNums) {
            for (Variable currVar : reader.getUsesVariablesByStatement(nextAssignStmtNum)) {
                if (currVar == variable) {
                    affectorValues.push_back(std::to_string(stmtNum));
                    affectedValues.push_back(std::to_string(nextAssignStmtNum));
                    found = true;
                    break;  // Break out of inner loop
                }
            }
            if (found) {
                break;  // Break out of outer loop
            }
        }
    }

    std::vector<Synonym> headers = {affectorSyn, affectedSyn};
    std::vector<SynonymValues> values = {affectorValues, affectedValues};
    return {headers, values};
}

bool Affects::isSimpleResult() const {
    // Takes care of Integer/Wildcard, Wildcard/Wildcard
    return !affector.isSynonym() && !affected.isSynonym();
}
