#include "AssignPattern.h"

AssignPattern::AssignPattern(ClauseArgument* assignSyn, std::vector<ClauseArgument*> args)
    : assignSyn(*assignSyn), firstArg(*args[0]), secondArg(*args[1]) {}

ClauseResult AssignPattern::evaluate(PKBFacadeReader& reader) {
    if (firstArg.isSynonym()) {
        return evaluateFirstArgSyn(reader);
    } else {
        return evaluateNoArgsSyns(reader);
    }
}

ClauseResult AssignPattern::evaluateFirstArgSyn(PKBFacadeReader& reader) {
    Synonym aSyn = dynamic_cast<Synonym&>(assignSyn);
    Synonym fSyn = dynamic_cast<Synonym&>(firstArg);  // This is 100% variable

    std::unordered_set<Stmt> assignStmts = reader.getStatementsByType(StatementType::ASSIGN);
    std::unordered_set<Variable> allVars = reader.getVariables();

    std::vector<std::string> stmtNumbers = {};
    std::vector<std::string> synValues = {};

    std::string secondString = secondArg.getValue();
    bool isPartial = false;
    if (secondArg.isExpressionSpec()) {
        if (isPartialExpression(secondString)) {
            secondString = removeCharsFrom(secondString, QPSConstants::WILDCARD);
            isPartial = true;
        }
        secondString = removeCharsFrom(secondString, QPSConstants::QUOTATIONS);
    }

    bool hasPattern;
    for (Stmt stmt : assignStmts) {
        for (Variable var : allVars) {
            if (isPartial) {
                hasPattern = reader.hasPartialPattern(stmt.stmtNum, var, secondString);
            } else {
                hasPattern = reader.hasExactPattern(stmt.stmtNum, var, secondString);
            }

            if (hasPattern) {
                // keep track of syn and stmt
                stmtNumbers.push_back(std::to_string(stmt.stmtNum));
                synValues.push_back(var);
                // no two variables can be on the lhs
                break;
            }
        }
    }

    if (stmtNumbers.size() != synValues.size()) {
        throw Exception("Unequal size of the 2 lists.");
    }

    std::vector<Synonym> returnSyn = {aSyn, fSyn};
    std::vector<SynonymValues> returnSynValues = {stmtNumbers, synValues};
    return {returnSyn, returnSynValues};
}

ClauseResult AssignPattern::evaluateNoArgsSyns(PKBFacadeReader& reader) {
    Synonym aSyn = dynamic_cast<Synonym&>(assignSyn);
    std::unordered_set<Stmt> assignStmts = reader.getStatementsByType(StatementType::ASSIGN);
    std::vector<std::string> stmtNumbers = {};

    if (firstArg.isWildcard() && secondArg.isWildcard()) {
        for (Stmt stmt : assignStmts) {
            stmtNumbers.push_back(std::to_string(stmt.stmtNum));
        }
        return {aSyn, stmtNumbers};
    }

    std::string firstString = firstArg.getValue();
    std::string secondString = secondArg.getValue();
    bool isPartial = false;
    if (secondArg.isExpressionSpec()) {
        if (isPartialExpression(secondString)) {
            secondString = removeCharsFrom(secondString, QPSConstants::WILDCARD);
            isPartial = true;
        }
        secondString = removeCharsFrom(secondString, QPSConstants::QUOTATIONS);
    }

    bool hasPattern;
    for (Stmt stmt : assignStmts) {
        if (isPartial) {
            hasPattern = reader.hasPartialPattern(stmt.stmtNum, firstString, secondString);
        } else {
            hasPattern = reader.hasExactPattern(stmt.stmtNum, firstString, secondString);
        }

        if (hasPattern) {
            stmtNumbers.push_back(std::to_string(stmt.stmtNum));
        }
    }

    return {aSyn, stmtNumbers};
}
