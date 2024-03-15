#include "Uses.h"

Uses::Uses(ClauseArgument& user, ClauseArgument& var) : user(user), var(var) {}

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

    return {reader.hasStatementVariableUseRelationship(user, var) ||
            reader.hasProcedureVariableModifiesRelationship(user, var)};
}

ClauseResult Uses::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym userSyn = static_cast<Synonym&>(user);
    Synonym varSyn = static_cast<Synonym&>(var);

    SynonymValues userValues{};
    SynonymValues varValues{};

    for (Variable var : reader.getVariables()) {
        std::unordered_set<StmtNum> users = reader.getUsesStatementsByVariable(var);
        for (StmtNum curruser : filterStatementsByType(reader, userSyn.getType(), users)) {
            userValues.push_back(std::to_string(curruser));
            varValues.push_back(var);
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

        for (StmtNum stmt : filterStatementsByType(reader, userSyn.getType(), allStmts)) {
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

std::unordered_set<StmtNum> Uses::filterStatementsByType(PKBFacadeReader& reader, DesignEntityType type,
                                                         std::unordered_set<StmtNum> users) {
    StatementType mappedStmtType = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[type];

    if (type == DesignEntityType::STMT) {
        return users;
    }

    std::unordered_set<StmtNum> filteredSet{};
    for (StmtNum curruser : users) {
        if (reader.getStatementByStmtNum(curruser)->type == mappedStmtType) {
            filteredSet.insert(curruser);
        }
    }
    return filteredSet;
}

bool Uses::isSimpleResult() const {
    return !user.isSynonym() && !var.isSynonym();
}
