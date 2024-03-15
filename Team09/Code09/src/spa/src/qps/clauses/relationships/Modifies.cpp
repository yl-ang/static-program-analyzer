#include "Modifies.h"

Modifies::Modifies(ClauseArgument& stmt, ClauseArgument& var) : modifier(stmt), var(var) {}

void Modifies::checkSemantic() {
    if (stmt.isWildcard()) {
        throw QPSSemanticError();
    }
    if (stmt.isSynonym()) {
        Synonym first = dynamic_cast<Synonym&>(stmt);
        if (first.getType() == DesignEntityType::VARIABLE || first.getType() == DesignEntityType::CONSTANT) {
            throw QPSSemanticError();
        }
    }
    if (var.isSynonym()) {
        Synonym second = dynamic_cast<Synonym&>(var);
        if (second.getType() != DesignEntityType::VARIABLE) {
            throw QPSSemanticError();
        }
    }
}

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

        // Literal, Synonym
        if (modifier.isLiteral()) {
            return variablesModifiedByProcedure(reader);
        }
    }

    return {reader.hasStatementVariableModifiesRelationship(modifier, var) ||
            reader.hasProcedureVariableModifiesRelationship(modifier, var)};
}

ClauseResult Modifies::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym modifierSyn = static_cast<Synonym&>(modifier);
    Synonym varSyn = static_cast<Synonym&>(var);

    SynonymValues modifierValues{};
    SynonymValues varValues{};

    for (Variable var : reader.getVariables()) {
        if (modifierSyn.getType() == DesignEntityType::PROCEDURE) {
            std::unordered_set<Procedure> procs = reader.getModifiesProceduresByVariable(var);
            for (Procedure proc : procs) {
                modifierValues.push_back(proc);
                varValues.push_back(var);
            }
        } else {
            std::unordered_set<StmtNum> stmts = reader.getModifiesStatementsByVariable(var);
            for (StmtNum currStmt : filterStatementsByType(reader, modifierSyn.getType(), stmts)) {
                modifierValues.push_back(std::to_string(currStmt));
                varValues.push_back(var);
            }
        }
    }

    std::vector<Synonym> headers = {modifierSyn, varSyn};
    std::vector<SynonymValues> values = {modifierValues, varValues};
    return {headers, values};
}

ClauseResult Modifies::evaluateModifierSynonym(PKBFacadeReader& reader) {
    Synonym modifierSyn = static_cast<Synonym&>(modifier);

    std::unordered_set<Variable> vars{};
    if (var.isWildcard()) {
        vars = reader.getVariables();
    } else {
        vars.insert(var.getValue());
    }

    SynonymValues values{};
    if (modifierSyn.getType() == DesignEntityType::PROCEDURE) {
        for (Variable var : vars) {
            auto procs = reader.getModifiesProceduresByVariable(var);
            for (Procedure proc : procs) {
                values.push_back(proc);
            }
        }
    } else {
        std::unordered_set<StmtNum> allStmts{};
        for (Variable var : vars) {
            std::unordered_set<StmtNum> stmts = reader.getModifiesStatementsByVariable(var);
            allStmts.insert(stmts.begin(), stmts.end());
        }

        for (StmtNum currStmt : filterStatementsByType(reader, modifierSyn.getType(), allStmts)) {
            values.push_back(std::to_string(currStmt));
        }
    }

    return {modifierSyn, values};
}

ClauseResult Modifies::variablesModifiedByProcedure(PKBFacadeReader& reader) {
    Synonym varSyn = static_cast<Synonym&>(var);

    SynonymValues values{};
    for (Variable currVar : reader.getModifiesVariablesByProcedure(modifier.getValue())) {
        values.push_back(currVar);
    }

    return {varSyn, values};
}

ClauseResult Modifies::variablesModifedByStatement(PKBFacadeReader& reader) {
    Synonym varSyn = static_cast<Synonym&>(var);

    SynonymValues values{};
    for (Variable currVar : reader.getModifiesVariablesByStatement(std::stoi(modifier.getValue()))) {
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
