#include "Modifies.h"

#include "../ClauseEvaluatorUtils.h"

Modifies::Modifies(ClauseArgument& stmt, ClauseArgument& var) : modifier(stmt), var(var) {}

bool Modifies::validateArguments() {
    if (modifier.isWildcard()) {
        return false;
    }
    if (modifier.isSynonym()) {
        Synonym first = dynamic_cast<Synonym&>(modifier);
        if (first.getType() == DesignEntityType::VARIABLE || first.getType() == DesignEntityType::CONSTANT) {
            return false;
        }
    }
    if (var.isSynonym()) {
        Synonym second = dynamic_cast<Synonym&>(var);
        if (second.getType() != DesignEntityType::VARIABLE) {
            return false;
        }
    }
    return true;
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

    return {ClauseEvaluatorUtils::isIdentLiteral(modifier.getValue())
                ? reader.hasProcedureVariableModifiesRelationship(modifier, var)
                : reader.hasStatementVariableModifiesRelationship(modifier, var)};
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
            std::vector<std::string> modifierStmts = ClauseEvaluatorUtils::filterStatementsByType(
                reader, modifierSyn.getType(), reader.getModifiesStatementsByVariable(var));
            modifierValues.reserve(modifierValues.size() + modifierStmts.size());
            modifierValues.insert(modifierValues.end(), modifierStmts.begin(), modifierStmts.end());

            varValues.reserve(varValues.size() + modifierStmts.size());
            varValues.insert(varValues.end(), modifierStmts.size(), var);
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
        std::vector<std::string> stmts =
            ClauseEvaluatorUtils::filterStatementsByType(reader, modifierSyn.getType(), allStmts);
        values.reserve(stmts.size());
        values.insert(values.end(), stmts.begin(), stmts.end());
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

bool Modifies::isSimpleResult() const {
    return !modifier.isSynonym() && !var.isSynonym();
}
