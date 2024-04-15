#include "WhilePattern.h"

WhilePattern::WhilePattern(std::shared_ptr<ClauseArgument> whileSyn, std::vector<std::shared_ptr<ClauseArgument>> args)
    : whileSyn(whileSyn), arguments(args) {}

ClauseResult WhilePattern::evaluate(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    if (arguments[0]->isSynonym()) {
        return evaluateFirstArgSyn(reader, evalDb);
    } else if (arguments[0]->isLiteral()) {
        return evaluateFirstArgLiteral(reader, evalDb);
    } else {
        return evaluateFirstArgWildcard(reader, evalDb);
    }
}

bool WhilePattern::validateArguments() {
    if (arguments.size() != 2) {
        return false;
    }
    if (!arguments[1]->isWildcard()) {
        return false;
    }
    return true;
}

ClauseResult WhilePattern::evaluateFirstArgSyn(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym wSyn = *std::dynamic_pointer_cast<Synonym>(whileSyn);
    Synonym fSyn = *std::dynamic_pointer_cast<Synonym>(arguments[0]);

    std::unordered_set<StmtNum> whileStmts = evalDb.getStmts(wSyn);
    std::unordered_set<Variable> allVars = evalDb.getVariables(fSyn);

    std::vector<Row> results{};

    for (const StmtNum& stmtNum : whileStmts) {
        for (Variable var : allVars) {
            if (reader.hasWhilePattern(stmtNum, var)) {
                // keep track of syn and stmt
                results.push_back(Row{{wSyn.getName(), std::to_string(stmtNum)}, {fSyn.getName(), var}});
            }
        }
    }

    std::vector<Synonym> returnSyn = {wSyn, fSyn};
    return {returnSyn, results};
}

ClauseResult WhilePattern::evaluateFirstArgLiteral(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym wSyn = *std::dynamic_pointer_cast<Synonym>(whileSyn);

    std::unordered_set<StmtNum> whileStmts = evalDb.getStmts(wSyn);

    std::vector<Row> stmtNumbers = {};

    for (const StmtNum& stmtNum : whileStmts) {
        if (reader.hasWhilePattern(stmtNum, arguments[0]->getValue())) {
            stmtNumbers.push_back(Row{{wSyn.getName(), std::to_string(stmtNum)}});
        }
    }

    return {wSyn, stmtNumbers};
}

ClauseResult WhilePattern::evaluateFirstArgWildcard(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym wSyn = *std::dynamic_pointer_cast<Synonym>(whileSyn);

    std::unordered_set<StmtNum> whileStmts = evalDb.getStmts(wSyn);
    std::unordered_set<Variable> allVars = reader.getVariables();

    std::vector<Row> stmtNumbers = {};

    for (const StmtNum& stmtNum : whileStmts) {
        for (Variable var : allVars) {
            if (reader.hasWhilePattern(stmtNum, var)) {
                stmtNumbers.push_back(Row{{wSyn.getName(), std::to_string(stmtNum)}});
            }
        }
    }

    return {wSyn, stmtNumbers};
}
