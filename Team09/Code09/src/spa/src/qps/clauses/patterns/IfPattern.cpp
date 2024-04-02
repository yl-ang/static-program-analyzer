#include "IfPattern.h"

IfPattern::IfPattern(std::shared_ptr<ClauseArgument> ifSyn, std::vector<std::shared_ptr<ClauseArgument>> args)
    : ifSyn(ifSyn), arguments(args) {}

ClauseResult IfPattern::evaluate(PKBFacadeReader& reader) {
    if (arguments[0]->isSynonym()) {
        return evaluateFirstArgSyn(reader);
    } else if (arguments[0]->isLiteral()) {
        return evaluateFirstArgLiteral(reader);
    } else {
        return evaluateFirstArgWildcard(reader);
    }
}

bool IfPattern::validateArguments() {
    if (arguments.size() != 3) {
        return false;
    }
    std::shared_ptr<Synonym> syn = std::dynamic_pointer_cast<Synonym>(ifSyn);
    if (syn->getType() != DesignEntityType::IF) {
        return false;
    }
    if (!arguments[1]->isWildcard() || !arguments[2]->isWildcard()) {
        return false;
    }
    return true;
}

ClauseResult IfPattern::evaluateFirstArgSyn(PKBFacadeReader& reader) {
    Synonym iSyn = *std::dynamic_pointer_cast<Synonym>(ifSyn);
    Synonym fSyn = *std::dynamic_pointer_cast<Synonym>(arguments[0]);

    std::unordered_set<Stmt> ifStmts = reader.getStatementsByType(StatementType::IF);
    std::unordered_set<Variable> allVars = reader.getVariables();

    std::vector<std::string> stmtNumbers = {};
    std::vector<std::string> synValues = {};

    for (Stmt stmt : ifStmts) {
        for (Variable var : allVars) {
            if (reader.hasIfPattern(stmt.stmtNum, var)) {
                // keep track of syn and stmt
                stmtNumbers.push_back(std::to_string(stmt.stmtNum));
                synValues.push_back(var);
            }
        }
    }

    std::vector<Synonym> returnSyn = {iSyn, fSyn};
    std::vector<SynonymValues> returnSynValues = {stmtNumbers, synValues};
    return {returnSyn, returnSynValues};
}

ClauseResult IfPattern::evaluateFirstArgLiteral(PKBFacadeReader& reader) {
    Synonym iSyn = *std::dynamic_pointer_cast<Synonym>(ifSyn);

    std::unordered_set<Stmt> ifStmts = reader.getStatementsByType(StatementType::IF);

    std::vector<std::string> stmtNumbers = {};

    for (Stmt stmt : ifStmts) {
        if (reader.hasIfPattern(stmt.stmtNum, arguments[0]->getValue())) {
            stmtNumbers.push_back(std::to_string(stmt.stmtNum));
        }
    }

    return {iSyn, stmtNumbers};
}

ClauseResult IfPattern::evaluateFirstArgWildcard(PKBFacadeReader& reader) {
    Synonym iSyn = *std::dynamic_pointer_cast<Synonym>(ifSyn);

    std::unordered_set<Stmt> ifStmts = reader.getStatementsByType(StatementType::IF);
    std::unordered_set<Variable> allVars = reader.getVariables();

    std::vector<std::string> stmtNumbers = {};

    for (Stmt stmt : ifStmts) {
        for (Variable var : allVars) {
            if (reader.hasIfPattern(stmt.stmtNum, var)) {
                stmtNumbers.push_back(std::to_string(stmt.stmtNum));
            }
        }
    }

    return {iSyn, stmtNumbers};
}
