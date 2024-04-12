#include "IfPattern.h"

IfPattern::IfPattern(std::shared_ptr<ClauseArgument> ifSyn, std::vector<std::shared_ptr<ClauseArgument>> args)
    : ifSyn(ifSyn), arguments(args) {}

ClauseResult IfPattern::evaluate(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
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

ClauseResult IfPattern::evaluateFirstArgSyn(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    Synonym iSyn = *std::dynamic_pointer_cast<Synonym>(ifSyn);
    Synonym fSyn = *std::dynamic_pointer_cast<Synonym>(arguments[0]);

    std::unordered_set<StmtNum> ifStmts = evalDb->getStmts(iSyn);
    std::unordered_set<Variable> allVars = evalDb->getVariables(fSyn);

    std::vector<std::string> stmtNumbers = {};
    std::vector<std::string> synValues = {};

    for (const StmtNum& stmtNum : ifStmts) {
        for (const Variable& var : allVars) {
            if (reader.hasIfPattern(stmtNum, var)) {
                // keep track of syn and stmt
                stmtNumbers.push_back(std::to_string(stmtNum));
                synValues.push_back(var);
            }
        }
    }

    std::vector<Synonym> returnSyn = {iSyn, fSyn};
    std::vector<SynonymValues> returnSynValues = {stmtNumbers, synValues};
    return {returnSyn, returnSynValues};
}

ClauseResult IfPattern::evaluateFirstArgLiteral(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    Synonym iSyn = *std::dynamic_pointer_cast<Synonym>(ifSyn);

    std::unordered_set<StmtNum> ifStmts = evalDb->getStmts(iSyn);

    std::vector<std::string> stmtNumbers = {};

    for (const StmtNum& stmtNum : ifStmts) {
        if (reader.hasIfPattern(stmtNum, arguments[0]->getValue())) {
            stmtNumbers.push_back(std::to_string(stmtNum));
        }
    }

    return {iSyn, stmtNumbers};
}

ClauseResult IfPattern::evaluateFirstArgWildcard(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    Synonym iSyn = *std::dynamic_pointer_cast<Synonym>(ifSyn);

    std::unordered_set<StmtNum> ifStmts = evalDb->getStmts(iSyn);
    std::unordered_set<Variable> allVars = reader.getVariables();

    std::vector<std::string> stmtNumbers = {};

    for (const StmtNum& stmtNum : ifStmts) {
        for (const Variable& var : allVars) {
            if (reader.hasIfPattern(stmtNum, var)) {
                stmtNumbers.push_back(std::to_string(stmtNum));
            }
        }
    }

    return {iSyn, stmtNumbers};
}
