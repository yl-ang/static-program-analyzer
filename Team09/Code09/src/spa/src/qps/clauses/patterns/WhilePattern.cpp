#include "WhilePattern.h"

WhilePattern::WhilePattern(std::shared_ptr<ClauseArgument> whileSyn, std::vector<std::shared_ptr<ClauseArgument>> args)
    : whileSyn(whileSyn), firstArg(args[0]), secondArg(args[1]) {}

ClauseResult WhilePattern::evaluate(PKBFacadeReader& reader) {
    if (firstArg->isSynonym()) {
        return evaluateFirstArgSyn(reader);
    } else if (firstArg->isLiteral()) {
        return evaluateFirstArgLiteral(reader);
    } else {
        return evaluateFirstArgWildcard(reader);
    }
}

ClauseResult WhilePattern::evaluateFirstArgSyn(PKBFacadeReader& reader) {
    Synonym wSyn = *std::dynamic_pointer_cast<Synonym>(whileSyn);
    Synonym fSyn = *std::dynamic_pointer_cast<Synonym>(firstArg);

    std::unordered_set<Stmt> whileStmts = reader.getStatementsByType(StatementType::WHILE);
    std::unordered_set<Variable> allVars = reader.getVariables();

    std::vector<std::string> stmtNumbers = {};
    std::vector<std::string> synValues = {};

    for (Stmt stmt : whileStmts) {
        for (Variable var : allVars) {
            if (reader.hasWhilePattern(stmt.stmtNum, var)) {
                // keep track of syn and stmt
                stmtNumbers.push_back(std::to_string(stmt.stmtNum));
                synValues.push_back(var);
            }
        }
    }

    std::vector<Synonym> returnSyn = {wSyn, fSyn};
    std::vector<SynonymValues> returnSynValues = {stmtNumbers, synValues};
    return {returnSyn, returnSynValues};
}

ClauseResult WhilePattern::evaluateFirstArgLiteral(PKBFacadeReader& reader) {
    Synonym wSyn = *std::dynamic_pointer_cast<Synonym>(whileSyn);

    std::unordered_set<Stmt> whileStmts = reader.getStatementsByType(StatementType::WHILE);

    std::vector<std::string> stmtNumbers = {};

    for (Stmt stmt : whileStmts) {
        if (reader.hasWhilePattern(stmt.stmtNum, firstArg->getValue())) {
            stmtNumbers.push_back(std::to_string(stmt.stmtNum));
        }
    }

    return {wSyn, stmtNumbers};
}

ClauseResult WhilePattern::evaluateFirstArgWildcard(PKBFacadeReader& reader) {
    Synonym wSyn = *std::dynamic_pointer_cast<Synonym>(whileSyn);

    std::unordered_set<Stmt> whileStmts = reader.getStatementsByType(StatementType::WHILE);
    std::unordered_set<Variable> allVars = reader.getVariables();

    std::vector<std::string> stmtNumbers = {};

    for (Stmt stmt : whileStmts) {
        for (Variable var : allVars) {
            if (reader.hasWhilePattern(stmt.stmtNum, var)) {
                stmtNumbers.push_back(std::to_string(stmt.stmtNum));
            }
        }
    }

    return {wSyn, stmtNumbers};
}
