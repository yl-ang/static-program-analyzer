#include "Modifies.h"

Modifies::Modifies(ClauseArgument& stmt, ClauseArgument& var) : modifier(stmt), var(var) {}

ClauseResult Modifies::evaluate(PKBFacadeReader& reader) {
    if (modifier.isSynonym() && var.isSynonym()) {
        return evaluateBothSynonyms(reader);
    }

    if (modifier.isSynonym()) {
        return evaluateModifierSynonym(reader);
    }

    if (var.isSynonym()) {
        // Integer, Synonym
        if (modifier.isInteger()) {
            return variablesModifedByStatement(reader);
        }

        // Wildcard, Synonym
        if (modifier.isWildcard()) {
            return allModifiedVariables(reader);
        }
    }

    return {reader.hasStatementVariableModifiesRelationship(modifier, var) ||
            reader.hasProcedureVariableModifiesRelationship(modifier, var)};
}

ClauseResult Modifies::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym stmtSyn = static_cast<Synonym&>(modifier);
    Synonym varSyn = static_cast<Synonym&>(var);

    SynonymValues stmtValues{};
    SynonymValues varValues{};

    for (Variable var : reader.getVariables()) {
        std::unordered_set<StmtNum> stmts = reader.getModifiesStatementsByVariable(var);
        for (StmtNum currStmt : filterStatementsByType(reader, stmtSyn.getType(), stmts)) {
            stmtValues.push_back(std::to_string(currStmt));
            varValues.push_back(var);
        }
    }

    std::vector<Synonym> headers = {stmtSyn, varSyn};
    std::vector<SynonymValues> values = {stmtValues, varValues};
    return {headers, values};
}

ClauseResult Modifies::evaluateModifierSynonym(PKBFacadeReader& reader) {
    Synonym stmtSyn = static_cast<Synonym&>(modifier);

    std::unordered_set<Variable> vars{};
    if (var.isWildcard()) {
        vars = reader.getVariables();
    } else {
        vars.insert(var.getValue());
    }

    SynonymValues values{};
    if (stmtSyn.getType() == DesignEntityType::PROCEDURE) {
        std::unordered_set<Procedure> allProcs{};
        for (Variable var : vars) {
            auto procs = reader.getModifiesProceduresByVariable(var);
            allProcs.insert(procs.begin(), procs.end());
        }

        values.insert(values.end(), allProcs.begin(), allProcs.end());
    } else {
        std::unordered_set<StmtNum> allStmts{};
        for (Variable var : vars) {
            std::unordered_set<StmtNum> stmts = reader.getModifiesStatementsByVariable(var);
            allStmts.insert(stmts.begin(), stmts.end());
        }

        for (StmtNum currStmt : filterStatementsByType(reader, stmtSyn.getType(), allStmts)) {
            values.push_back(std::to_string(currStmt));
        }
    }

    return {stmtSyn, values};
}

ClauseResult Modifies::allModifiedVariables(PKBFacadeReader& reader) {
    Synonym varSyn = static_cast<Synonym&>(var);

    SynonymValues values{};
    for (Variable currVar : reader.getVariables()) {
        if (!reader.getModifiesStatementsByVariable(currVar).empty()) {
            values.push_back(currVar);
        }
    }

    return {varSyn, values};
}

ClauseResult Modifies::variablesModifedByStatement(PKBFacadeReader& reader) {
    Integer stmtInt = static_cast<Integer&>(modifier);
    Synonym varSyn = static_cast<Synonym&>(var);

    SynonymValues values{};
    for (Variable currVar : reader.getModifiesVariablesByStatement(std::stoi(stmtInt.getValue()))) {
        values.push_back(currVar);
    }

    return {varSyn, values};
}

std::unordered_set<StmtNum> Modifies::filterStatementsByType(PKBFacadeReader& reader, DesignEntityType type,
                                                             std::unordered_set<StmtNum> stmts) {
    StatementType mappedStmtType = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[type];

    if (type == DesignEntityType::STMT) {
        return stmts;
    }

    std::unordered_set<StmtNum> filteredSet{};
    for (StmtNum currStmt : stmts) {
        if (reader.getStatementByStmtNum(currStmt)->type == mappedStmtType) {
            filteredSet.insert(currStmt);
        }
    }
    return filteredSet;
}

bool Modifies::isSimpleResult() const {
    return !modifier.isSynonym() && !var.isSynonym();
}
