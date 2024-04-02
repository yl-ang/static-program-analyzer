#include "AssignPattern.h"

AssignPattern::AssignPattern(std::shared_ptr<ClauseArgument> assignSyn,
                             std::vector<std::shared_ptr<ClauseArgument>> args)
    : assignSyn(assignSyn), arguments(args) {}

ClauseResult AssignPattern::evaluate(PKBFacadeReader& reader) {
    if (arguments[0]->isSynonym()) {
        return evaluateFirstArgSyn(reader);
    } else {
        return evaluateNoArgsSyns(reader);
    }
}

bool AssignPattern::validateArguments() {
    if (arguments.size() != 2) {
        return false;
    }
    std::shared_ptr<Synonym> syn = std::dynamic_pointer_cast<Synonym>(assignSyn);
    if (syn->getType() != DesignEntityType::ASSIGN) {
        return false;
    }
    if (!arguments[1]->isExpressionSpec() && !arguments[1]->isWildcard()) {
        return false;
    }
    return true;
}

ClauseResult AssignPattern::evaluateFirstArgSyn(PKBFacadeReader& reader) {
    Synonym aSyn = *std::dynamic_pointer_cast<Synonym>(assignSyn);
    Synonym fSyn = *std::dynamic_pointer_cast<Synonym>(arguments[0]);  // This is 100% variable

    std::unordered_set<Stmt> assignStmts = reader.getStatementsByType(StatementType::ASSIGN);
    std::unordered_set<Variable> allVars = reader.getVariables();

    std::vector<std::string> stmtNumbers = {};
    std::vector<std::string> synValues = {};

    std::string secondString = arguments[1]->getValue();
    bool isPartial = false;
    if (arguments[1]->isExpressionSpec()) {
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
    Synonym aSyn = *std::dynamic_pointer_cast<Synonym>(assignSyn);
    std::unordered_set<Stmt> assignStmts = reader.getStatementsByType(StatementType::ASSIGN);
    std::vector<std::string> stmtNumbers = {};

    if (arguments[0]->isWildcard() && arguments[1]->isWildcard()) {
        for (Stmt stmt : assignStmts) {
            stmtNumbers.push_back(std::to_string(stmt.stmtNum));
        }
        return {aSyn, stmtNumbers};
    }

    std::string firstString = arguments[0]->getValue();
    std::string secondString = arguments[1]->getValue();
    bool isPartial = false;
    if (arguments[1]->isExpressionSpec()) {
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
