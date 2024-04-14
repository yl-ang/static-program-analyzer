#include "IfPattern.h"

IfPattern::IfPattern(std::shared_ptr<ClauseArgument> ifSyn, std::vector<std::shared_ptr<ClauseArgument>> args)
    : ifSyn(ifSyn), arguments(args) {}

ClauseResult IfPattern::evaluate(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    if (arguments[0]->isSynonym()) {
        return evaluateFirstArgSyn(reader, evalDb);
    } else if (arguments[0]->isLiteral()) {
        return evaluateFirstArgLiteral(reader, evalDb);
    } else {
        return evaluateFirstArgWildcard(reader, evalDb);
    }
}

bool IfPattern::validateArguments() {
    if (arguments.size() != 3) {
        return false;
    }
    if (!arguments[1]->isWildcard() || !arguments[2]->isWildcard()) {
        return false;
    }
    return true;
}

ClauseResult IfPattern::evaluateFirstArgSyn(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym iSyn = *std::dynamic_pointer_cast<Synonym>(ifSyn);
    Synonym fSyn = *std::dynamic_pointer_cast<Synonym>(arguments[0]);

    std::unordered_set<StmtNum> ifStmts = evalDb.getStmts(iSyn);
    std::unordered_set<Variable> allVars = evalDb.getVariables(fSyn);

    std::vector<Row> results{};

    for (const StmtNum& stmtNum : ifStmts) {
        for (const Variable& var : allVars) {
            if (reader.hasIfPattern(stmtNum, var)) {
                // keep track of syn and stmt
                results.push_back(Row{{iSyn.getName(), std::to_string(stmtNum)}, {fSyn.getName(), var}});
            }
        }
    }

    std::vector<Synonym> returnSyn = {iSyn, fSyn};
    return {returnSyn, results};
}

ClauseResult IfPattern::evaluateFirstArgLiteral(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym iSyn = *std::dynamic_pointer_cast<Synonym>(ifSyn);

    std::unordered_set<StmtNum> ifStmts = evalDb.getStmts(iSyn);

    std::vector<Row> stmtNumbers = {};

    for (const StmtNum& stmtNum : ifStmts) {
        if (reader.hasIfPattern(stmtNum, arguments[0]->getValue())) {
            stmtNumbers.push_back(Row{{iSyn.getName(), std::to_string(stmtNum)}});
        }
    }

    return {iSyn, stmtNumbers};
}

ClauseResult IfPattern::evaluateFirstArgWildcard(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym iSyn = *std::dynamic_pointer_cast<Synonym>(ifSyn);

    std::unordered_set<StmtNum> ifStmts = evalDb.getStmts(iSyn);
    std::unordered_set<Variable> allVars = reader.getVariables();

    std::vector<Row> stmtNumbers = {};

    for (const StmtNum& stmtNum : ifStmts) {
        for (const Variable& var : allVars) {
            if (reader.hasIfPattern(stmtNum, var)) {
                stmtNumbers.push_back(Row{{iSyn.getName(), std::to_string(stmtNum)}});
            }
        }
    }

    return {iSyn, stmtNumbers};
}
