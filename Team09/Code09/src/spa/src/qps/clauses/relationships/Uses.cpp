#include "Uses.h"

#include "../ClauseEvaluatorUtils.h"

Uses::Uses(ClauseArgument& user, ClauseArgument& var) : user(user), var(var) {}

bool Uses::validateArguments() {
    if (user.isWildcard()) {
        return false;
    }
    if (user.isSynonym()) {
        Synonym first = dynamic_cast<Synonym&>(user);
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

ClauseResult Uses::evaluate(PKBFacadeReader& reader) {
    if (user.isSynonym() && var.isSynonym()) {
        return evaluateBothSynonyms(reader);
    }

    if (user.isSynonym()) {
        return evaluateUserSynonym(reader);
    }

    if (var.isSynonym()) {
        // Integer, Synonym
        if (user.isInteger()) {
            return variablesUsedByStatement(reader);
        }

        // Wildcard, Synonym
        if (user.isLiteral()) {
            return variablesUsedByProcedure(reader);
        }
    }

    return {ClauseEvaluatorUtils::isIdentLiteral(user.getValue())
                ? reader.hasProcedureVariableUseRelationship(user, var)
                : reader.hasStatementVariableUseRelationship(user, var)};
}

ClauseResult Uses::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym userSyn = static_cast<Synonym&>(user);
    Synonym varSyn = static_cast<Synonym&>(var);

    SynonymValues userValues{};
    SynonymValues varValues{};

    for (Variable var : reader.getVariables()) {
        if (userSyn.getType() == DesignEntityType::PROCEDURE) {
            std::unordered_set<Procedure> procs = reader.getUsesProceduresByVariable(var);
            for (Procedure proc : procs) {
                userValues.push_back(proc);
                varValues.push_back(var);
            }
        } else {
            std::unordered_set<StmtNum> stmts = reader.getUsesStatementsByVariable(var);
            for (StmtNum currStmt : ClauseEvaluatorUtils::filterStatementsByType(reader, userSyn.getType(), stmts)) {
                userValues.push_back(std::to_string(currStmt));
                varValues.push_back(var);
            }
        }
    }

    std::vector<Synonym> headers = {userSyn, varSyn};
    std::vector<SynonymValues> values = {userValues, varValues};
    return {headers, values};
}

ClauseResult Uses::evaluateUserSynonym(PKBFacadeReader& reader) {
    Synonym userSyn = static_cast<Synonym&>(this->user);

    std::unordered_set<Variable> vars{};
    if (this->var.isWildcard()) {
        vars = reader.getVariables();
    } else {
        vars.insert(this->var.getValue());
    }

    SynonymValues values{};
    if (userSyn.getType() == DesignEntityType::PROCEDURE) {
        for (Variable var : vars) {
            std::unordered_set<Procedure> users = reader.getUsesProceduresByVariable(var);
            for (Procedure proc : users) {
                values.push_back(proc);
            }
        }
    } else {
        std::unordered_set<StmtNum> allStmts{};
        for (Variable var : vars) {
            std::unordered_set<StmtNum> users = reader.getUsesStatementsByVariable(var);
            allStmts.insert(users.begin(), users.end());
        }

        for (StmtNum stmt : ClauseEvaluatorUtils::filterStatementsByType(reader, userSyn.getType(), allStmts)) {
            values.push_back(std::to_string(stmt));
        }
    }

    return {userSyn, values};
}

ClauseResult Uses::variablesUsedByProcedure(PKBFacadeReader& reader) {
    Synonym varSyn = static_cast<Synonym&>(var);

    SynonymValues values{};
    for (Variable var : reader.getUsesVariablesByProcedure(user.getValue())) {
        values.push_back(var);
    }
    return {varSyn, values};
}

ClauseResult Uses::variablesUsedByStatement(PKBFacadeReader& reader) {
    Synonym varSyn = static_cast<Synonym&>(var);

    SynonymValues values{};
    for (Variable currVar : reader.getUsesVariablesByStatement(std::stoi(user.getValue()))) {
        values.push_back(currVar);
    }

    return {varSyn, values};
}

bool Uses::isSimpleResult() const {
    return !user.isSynonym() && !var.isSynonym();
}
