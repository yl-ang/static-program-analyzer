#include "Uses.h"

#include "../ClauseEvaluatorUtils.h"

Uses::Uses(std::shared_ptr<ClauseArgument> user, std::shared_ptr<ClauseArgument> var) : user(user), var(var) {}

bool Uses::validateArguments() {
    if (user->isWildcard()) {
        return false;
    }
    if (user->isSynonym()) {
        std::shared_ptr<Synonym> first = std::dynamic_pointer_cast<Synonym>(user);
        if (first->getType() == DesignEntityType::VARIABLE || first->getType() == DesignEntityType::CONSTANT) {
            return false;
        }
    }
    if (var->isSynonym()) {
        std::shared_ptr<Synonym> second = std::dynamic_pointer_cast<Synonym>(var);
        if (second->getType() != DesignEntityType::VARIABLE) {
            return false;
        }
    }
    return true;
}

ClauseResult Uses::evaluate(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    if (user->isSynonym() && var->isSynonym()) {
        return evaluateBothSynonyms(reader, evalDb);
    }

    if (user->isSynonym()) {
        return evaluateUserSynonym(reader);
    }

    if (var->isSynonym()) {
        // Integer, Synonym
        if (user->isInteger()) {
            return variablesUsedByStatement(reader);
        }

        // Wildcard, Synonym
        if (user->isLiteral()) {
            return variablesUsedByProcedure(reader);
        }
    }

    return {ClauseEvaluatorUtils::isIdentLiteral(user->getValue())
                ? reader.hasProcedureVariableUseRelationship(*user, *var)
                : reader.hasStatementVariableUseRelationship(*user, *var)};
}

ClauseResult Uses::evaluateBothSynonyms(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym userSyn = *std::dynamic_pointer_cast<Synonym>(user);
    Synonym varSyn = *std::dynamic_pointer_cast<Synonym>(var);

    SynonymValues userValues{};
    SynonymValues varValues{};

    for (const Variable& var : evalDb.getVariables(varSyn)) {
        if (userSyn.getType() == DesignEntityType::PROCEDURE) {
            std::unordered_set<Procedure> procs = reader.getUsesProceduresByVariable(var);
            for (Procedure proc : procs) {
                userValues.push_back(proc);
                varValues.push_back(var);
            }
        } else {
            std::vector<std::string> userStmts = ClauseEvaluatorUtils::filterStatementsByType(
                reader, userSyn.getType(), reader.getUsesStatementsByVariable(var));
            userValues.reserve(userValues.size() + userStmts.size());
            userValues.insert(userValues.end(), userStmts.begin(), userStmts.end());

            varValues.reserve(varValues.size() + userStmts.size());
            varValues.insert(varValues.end(), userStmts.size(), var);
        }
    }

    std::vector<Synonym> headers = {userSyn, varSyn};
    std::vector<SynonymValues> values = {userValues, varValues};
    return {headers, values};
}

ClauseResult Uses::evaluateUserSynonym(PKBFacadeReader& reader) {
    Synonym userSyn = *std::dynamic_pointer_cast<Synonym>(this->user);

    std::unordered_set<Variable> vars{};
    if (this->var->isWildcard()) {
        vars = reader.getVariables();
    } else {
        vars.insert(this->var->getValue());
    }

    SynonymValues values{};
    if (userSyn.getType() == DesignEntityType::PROCEDURE) {
        for (Variable var : vars) {
            std::unordered_set<Procedure> users = reader.getUsesProceduresByVariable(var);
            values.reserve(users.size());
            values.insert(values.end(), users.begin(), users.end());
        }
    } else {
        std::unordered_set<StmtNum> allStmts{};
        for (Variable var : vars) {
            std::unordered_set<StmtNum> users = reader.getUsesStatementsByVariable(var);
            allStmts.reserve(users.size());
            allStmts.insert(users.begin(), users.end());
        }

        std::vector<std::string> stmts =
            ClauseEvaluatorUtils::filterStatementsByType(reader, userSyn.getType(), allStmts);
        values.reserve(stmts.size());
        values.insert(values.end(), stmts.begin(), stmts.end());
    }

    return {userSyn, values};
}

ClauseResult Uses::variablesUsedByProcedure(PKBFacadeReader& reader) {
    Synonym varSyn = *std::dynamic_pointer_cast<Synonym>(var);

    SynonymValues values{};
    for (Variable var : reader.getUsesVariablesByProcedure(user->getValue())) {
        values.push_back(var);
    }
    return {varSyn, values};
}

ClauseResult Uses::variablesUsedByStatement(PKBFacadeReader& reader) {
    Synonym varSyn = *std::dynamic_pointer_cast<Synonym>(var);

    SynonymValues values{};
    for (Variable currVar : reader.getUsesVariablesByStatement(std::stoi(user->getValue()))) {
        values.push_back(currVar);
    }

    return {varSyn, values};
}

bool Uses::isSimpleResult() const {
    return !user->isSynonym() && !var->isSynonym();
}
