#include "AssignPattern.h"

AssignPattern::AssignPattern(std::shared_ptr<ClauseArgument> assignSyn,
                             std::vector<std::shared_ptr<ClauseArgument>> args)
    : assignSyn(assignSyn), arguments(args) {}

ClauseResult AssignPattern::evaluate(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    if (arguments[0]->isSynonym()) {
        return evaluateFirstArgSyn(reader, evalDb);
    } else {
        return evaluateNoArgsSyns(reader, evalDb);
    }
}

bool AssignPattern::validateArguments() {
    if (arguments.size() != 2) {
        return false;
    }
    if (!arguments[1]->isExpressionSpec() && !arguments[1]->isWildcard()) {
        return false;
    }
    return true;
}

ClauseResult AssignPattern::evaluateFirstArgSyn(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    Synonym aSyn = *std::dynamic_pointer_cast<Synonym>(assignSyn);
    Synonym fSyn = *std::dynamic_pointer_cast<Synonym>(arguments[0]);  // This is 100% variable

    std::unordered_set<StmtNum> assignStmts = evalDb->getStmts(aSyn);
    std::unordered_set<Variable> allVars = evalDb->getVariables(fSyn);

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
    for (const StmtNum& stmtNum : assignStmts) {
        for (const Variable& var : allVars) {
            if (isPartial) {
                hasPattern = reader.hasPartialPattern(stmtNum, var, secondString);
            } else {
                hasPattern = reader.hasExactPattern(stmtNum, var, secondString);
            }

            if (hasPattern) {
                // keep track of syn and stmt
                stmtNumbers.push_back(std::to_string(stmtNum));
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

ClauseResult AssignPattern::evaluateNoArgsSyns(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    Synonym aSyn = *std::dynamic_pointer_cast<Synonym>(assignSyn);
    std::unordered_set<StmtNum> assignStmts = evalDb->getStmts(aSyn);
    std::vector<std::string> stmtNumbers = {};

    if (arguments[0]->isWildcard() && arguments[1]->isWildcard()) {
        for (const StmtNum& stmtNum : assignStmts) {
            stmtNumbers.push_back(std::to_string(stmtNum));
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
    for (const StmtNum& stmtNum : assignStmts) {
        if (isPartial) {
            hasPattern = reader.hasPartialPattern(stmtNum, firstString, secondString);
        } else {
            hasPattern = reader.hasExactPattern(stmtNum, firstString, secondString);
        }

        if (hasPattern) {
            stmtNumbers.push_back(std::to_string(stmtNum));
        }
    }

    return {aSyn, stmtNumbers};
}
