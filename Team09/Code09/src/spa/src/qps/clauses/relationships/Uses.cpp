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

    std::vector<Row> values{};

    for (const Variable& var : evalDb.getVariables(varSyn)) {
        if (userSyn.getType() == DesignEntityType::PROCEDURE) {
            std::unordered_set<Procedure> procs = reader.getUsesProceduresByVariable(var);
            for (Procedure proc : procs) {
                values.push_back(Row{{userSyn.getName(), proc}, {varSyn.getName(), var}});
            }
        } else {
            std::vector<std::string> userStmts = ClauseEvaluatorUtils::filterStatementsByType(
                reader, userSyn.getType(), reader.getUsesStatementsByVariable(var));

            for (const auto& stmt : userStmts) {
                values.push_back(Row{{userSyn.getName(), stmt}, {varSyn.getName(), var}});
            }
        }
    }

    std::vector<Synonym> headers = {userSyn, varSyn};
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

    std::vector<Row> values{};
    if (userSyn.getType() == DesignEntityType::PROCEDURE) {
        for (Variable var : vars) {
            std::unordered_set<Procedure> users = reader.getUsesProceduresByVariable(var);
            for (const auto& proc : users) {
                values.push_back(Row{{userSyn.getName(), proc}});
            }
        }
    } else {
        std::unordered_set<StmtNum> allStmts{};
        for (const Variable& var : vars) {
            std::unordered_set<StmtNum> users = reader.getUsesStatementsByVariable(var);
            allStmts.insert(users.begin(), users.end());
        }

        std::vector<std::string> stmts =
            ClauseEvaluatorUtils::filterStatementsByType(reader, userSyn.getType(), allStmts);
        for (const auto& stmt : stmts) {
            values.push_back(Row{{userSyn.getName(), stmt}});
        }
    }

    return {userSyn, values};
}

ClauseResult Uses::variablesUsedByProcedure(PKBFacadeReader& reader) {
    Synonym varSyn = *std::dynamic_pointer_cast<Synonym>(var);

    std::vector<Row> values{};
    for (const Variable& var : reader.getUsesVariablesByProcedure(user->getValue())) {
        values.push_back(Row{{varSyn.getName(), var}});
    }
    return {varSyn, values};
}

ClauseResult Uses::variablesUsedByStatement(PKBFacadeReader& reader) {
    Synonym varSyn = *std::dynamic_pointer_cast<Synonym>(var);

    std::vector<Row> values{};
    for (Variable currVar : reader.getUsesVariablesByStatement(std::stoi(user->getValue()))) {
        values.push_back(Row{{varSyn.getName(), currVar}});
    }

    return {varSyn, values};
}

bool Uses::isSimpleResult() const {
    return !user->isSynonym() && !var->isSynonym();
}
