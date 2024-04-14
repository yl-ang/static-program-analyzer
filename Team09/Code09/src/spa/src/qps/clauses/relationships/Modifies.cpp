#include "Modifies.h"

#include "../ClauseEvaluatorUtils.h"

Modifies::Modifies(std::shared_ptr<ClauseArgument> stmt, std::shared_ptr<ClauseArgument> var)
    : modifier(stmt), var(var) {}

bool Modifies::validateArguments() {
    if (modifier->isWildcard()) {
        return false;
    }
    if (modifier->isSynonym()) {
        std::shared_ptr<Synonym> first = std::dynamic_pointer_cast<Synonym>(modifier);
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

ClauseResult Modifies::evaluate(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    if (modifier->isSynonym() && var->isSynonym()) {
        return evaluateBothSynonyms(reader, evalDb);
    }

    if (modifier->isSynonym()) {
        return evaluateModifierSynonym(reader);
    }

    if (var->isSynonym()) {
        // Integer, Synonym
        if (modifier->isInteger()) {
            return variablesModifedByStatement(reader);
        }

        // Literal, Synonym
        if (modifier->isLiteral()) {
            return variablesModifiedByProcedure(reader);
        }
    }

    return {ClauseEvaluatorUtils::isIdentLiteral(modifier->getValue())
                ? reader.hasProcedureVariableModifiesRelationship(*modifier, *var)
                : reader.hasStatementVariableModifiesRelationship(*modifier, *var)};
}

ClauseResult Modifies::evaluateBothSynonyms(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym modifierSyn = *std::dynamic_pointer_cast<Synonym>(modifier);
    Synonym varSyn = *std::dynamic_pointer_cast<Synonym>(var);

    SynonymValues modifierValues{};
    SynonymValues varValues{};

    std::vector<Row> values{};

    bool isProcedureModifier = modifierSyn.getType() == DesignEntityType::PROCEDURE;

    for (Variable var : evalDb.getVariables(varSyn)) {
        if (isProcedureModifier) {
            std::unordered_set<Procedure> procs = reader.getModifiesProceduresByVariable(var);
            for (const Procedure& proc : procs) {
                values.push_back(Row{{modifierSyn.getName(), proc}, {varSyn.getName(), var}});
            }
        } else {
            std::vector<std::string> modifierStmts = ClauseEvaluatorUtils::filterStatementsByType(
                reader, modifierSyn.getType(), reader.getModifiesStatementsByVariable(var));
            for (const std::string& stmt : modifierStmts) {
                values.push_back(Row{{modifierSyn.getName(), stmt}, {varSyn.getName(), var}});
            }
        }
    }

    std::vector<Synonym> headers = {modifierSyn, varSyn};
    return {headers, values};
}

ClauseResult Modifies::evaluateModifierSynonym(PKBFacadeReader& reader) {
    Synonym modifierSyn = *std::dynamic_pointer_cast<Synonym>(modifier);

    std::unordered_set<Variable> vars{};
    if (var->isWildcard()) {
        vars = reader.getVariables();
    } else {
        vars.insert(var->getValue());
    }

    std::vector<Row> values{};
    if (modifierSyn.getType() == DesignEntityType::PROCEDURE) {
        for (const Variable& var : vars) {
            auto procs = reader.getModifiesProceduresByVariable(var);
            for (Procedure proc : procs) {
                values.push_back(Row{{modifierSyn.getName(), proc}});
            }
        }
    } else {
        std::unordered_set<StmtNum> allStmts{};
        for (const Variable& var : vars) {
            std::unordered_set<StmtNum> stmts = reader.getModifiesStatementsByVariable(var);
            allStmts.insert(stmts.begin(), stmts.end());
        }
        std::vector<std::string> stmts =
            ClauseEvaluatorUtils::filterStatementsByType(reader, modifierSyn.getType(), allStmts);
        for (const auto& stmt : stmts) {
            values.push_back(Row{{modifierSyn.getName(), stmt}});
        }
    }

    return {modifierSyn, values};
}

ClauseResult Modifies::variablesModifiedByProcedure(PKBFacadeReader& reader) {
    Synonym varSyn = *std::dynamic_pointer_cast<Synonym>(var);

    std::vector<Row> values{};
    for (const Variable& currVar : reader.getModifiesVariablesByProcedure(modifier->getValue())) {
        values.push_back(Row{{varSyn.getName(), currVar}});
    }

    return {varSyn, values};
}

ClauseResult Modifies::variablesModifedByStatement(PKBFacadeReader& reader) {
    Synonym varSyn = *std::dynamic_pointer_cast<Synonym>(var);

    std::vector<Row> values{};
    for (const Variable& currVar : reader.getModifiesVariablesByStatement(std::stoi(modifier->getValue()))) {
        values.push_back(Row{{varSyn.getName(), currVar}});
    }

    return {varSyn, values};
}

bool Modifies::isSimpleResult() const {
    return !modifier->isSynonym() && !var->isSynonym();
}
