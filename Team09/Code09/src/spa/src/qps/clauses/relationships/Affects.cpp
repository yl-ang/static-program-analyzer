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
    if (isSimpleResult()) {
        return {reader.hasParentRelationship(affector, affected)};
    }

    /**
     * Return types:
     * 
     * synonym synonym s1, s2
     * 
     * integer synonym s2
     * wildcard synonym s2
     * synonym integer s1
     * synonym wilcard s1
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

ClauseResult Affects::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    // bool affectorIsSynonym = affector.isSynonym();
    // Synonym syn = affectorIsSynonym ? dynamic_cast<Synonym&>(affector) : dynamic_cast<Synonym&>(affected);

    // std::unordered_set<Stmt> allStmts{};
    // allStmts = reader.getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[DesignEntityType::ASSIGN]);

    // std::unordered_set<StmtNum> uniqueValues{};

    // for (Stmt stmt : allStmts) {
    //     StmtNum stmtNum = stmt.stmtNum;
    //     if (affectorIsSynonym) {
            
    //     } else {
            
    //     }
    // }
}

ClauseResult Affects::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool affectorIsSynonym = affector.isSynonym();
    Synonym syn = affectorIsSynonym ? dynamic_cast<Synonym&>(affector) : dynamic_cast<Synonym&>(affected);
    Integer stmtNum = affectorIsSynonym ? dynamic_cast<Integer&>(affected) : dynamic_cast<Integer&>(affector);

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
     * 
     * Get statements that modify variables (LHS)
     * Then filter down to assign statements
     * Make tuple of modified variable and assign statement
    */
    std::unordered_set<Variable> allVar = reader.getVariables();
    std::unordered_set<std::tuple<Variable, StmtNum>> varAndAffectorStmtList{};
    for (Variable var : allVar) {
        std::unordered_set<StmtNum> allStmts = reader.getModifiesStatementsByVariable(var);
        std::unordered_set<StmtNum> assignStmts = ClauseEvaluatorUtils::filterStatementsByType(
                                                reader, affectorSyn.getType(), allStmts);
        for (StmtNum assignStmt : assignStmts) {
            varAndAffectorStmtList.insert(std::make_tuple(var, assignStmt));
        }
    }

    /**
     * 2. For each tuple of modified variable and assign statement
     * - Get the control flow statements that follow it (like NextStar)
     * - Get the CF with all the statements
     * - Get separate list with assign statements
    */
    std::unordered_set<StmtNum> nextStmtNums{};
    for (const std::tuple<Variable, StmtNum>& varAndAffectorStmt : varAndAffectorStmtList) {
        // Get NextStar Control Flow - all StmtNums that follow that assign statement
        StmtNum affectorStmtNum = std::get<StmtNum>(varAndAffectorStmt);
        std::unordered_set<StmtNum> CFAffector = reader.getNexteeStar(affectorStmtNum);
        nextStmtNums.insert(CFAffector.begin(), CFAffector.end());
    }

    /**
     * For each tuple of modified variable and assign statement:
     * For each CF next assign statement:
     * - iterate through variables used by the CF next assign statement,
     * 
     * - if not ASSIGN statement, CHECK that does not modify the variable
     * if modified, exit loop for that affecter statement
     * 
     * - else if ASSIGN statement, CHECK that currVariable equals modified variable
     * if yes, push back values
    */
    for (const std::tuple<Variable, StmtNum>& varAndAffectorStmt : varAndAffectorStmtList) {
        StmtNum stmtNum = std::get<StmtNum>(varAndAffectorStmt);
        Variable variable = std::get<Variable>(varAndAffectorStmt);

        for (StmtNum nextStmtNum : nextStmtNums) {
            std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
            // Checking if modifies the affector variable
            for (Variable currVar : reader.getModifiesVariablesByStatement(nextStmtNum)) {
                if (currVar == variable) {
                    break;  // Break out of inner loop for current affector statement
                }
            }
            // Checking if uses the affector variable
            for (Variable currVar : reader.getUsesVariablesByStatement(nextStmtNum)) {
                if (currVar == variable) {  
                    affectorValues.push_back(std::to_string(stmtNum));
                    affectedValues.push_back(std::to_string(nextStmtNum));
                }
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
