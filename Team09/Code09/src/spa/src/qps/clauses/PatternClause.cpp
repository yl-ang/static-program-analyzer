#include "PatternClause.h"

#include <qps/exceptions/Exception.h>

#include "qps/ParserUtils.h"

PatternClause::PatternClause(ClauseArgument* assign, ClauseArgument* f, ClauseArgument* s)
    : assignSynonym(*assign), firstArg(*f), secondArg(*s) {}

bool PatternClause::equals(const QueryClause& other) const {
    if (const PatternClause* ptr = dynamic_cast<const PatternClause*>(&other)) {
        return firstArg.getValue() == (ptr->firstArg).getValue() &&
               firstArg.getClauseType() == (ptr->firstArg).getClauseType() &&
               secondArg.getValue() == (ptr->secondArg).getValue() &&
               secondArg.getClauseType() == (ptr->secondArg).getClauseType() &&
               removeAllWhitespaces(assignSynonym.getValue()) ==
                   removeAllWhitespaces((ptr->assignSynonym).getValue()) &&
               assignSynonym.getClauseType() == (ptr->assignSynonym).getClauseType();
    }
    return false;
}

ClauseResult PatternClause::evaluate(PKBFacadeReader& reader) {
    bool isFirstSynonym = firstArg.isSynonym();
    if (isFirstSynonym) {
        return evaluateArgSyn(reader);
    } else {
        return evaluateArgNonSyns(reader);
    }

    return {{}, {}};
}

bool PatternClause::isBooleanResult() const {
    return false;
}

bool PatternClause::containsSynonym(const Synonym& s) const {
    return assignSynonym == s || firstArg == s || secondArg == s;
}

std::vector<Synonym> PatternClause::getSynonyms() const {
    std::vector<Synonym> synonyms;

    if (assignSynonym.isSynonym()) {
        synonyms.push_back(dynamic_cast<Synonym&>(assignSynonym));
    }
    if (firstArg.isSynonym()) {
        synonyms.push_back(dynamic_cast<Synonym&>(firstArg));
    }
    if (secondArg.isSynonym()) {
        synonyms.push_back(dynamic_cast<Synonym&>(secondArg));
    }
    return synonyms;
}

ClauseResult PatternClause::evaluateArgSyn(PKBFacadeReader& reader) {
    Synonym aSyn = dynamic_cast<Synonym&>(assignSynonym);
    Synonym fSyn = dynamic_cast<Synonym&>(firstArg);  // This is 100% variable

    std::unordered_set<Stmt> allStmts = reader.getStatementsByType(StatementType::ASSIGN);
    std::unordered_set<Variable> allVars = reader.getVariables();

    std::vector<std::string> stmtNumbers = {};
    std::vector<std::string> synValues = {};

    // Temporary solution to ExpressionSpec
    ClauseArgument* second;
    if (secondArg.isExpressionSpec()) {
        std::string temp = removeAllWhitespaces(secondArg.getValue());
        temp = temp.substr(2, temp.size() - 4);  // MAGIC NUMBER
        second = new ExpressionSpec(temp);
    } else {
        second = &secondArg;
    }

    for (Stmt stmt : allStmts) {
        for (Variable var : allVars) {
            Literal tempLiteral(var);
            if (reader.hasPattern(stmt.stmtNum, tempLiteral, *second)) {
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

ClauseResult PatternClause::evaluateArgNonSyns(PKBFacadeReader& reader) {
    Synonym aSyn = dynamic_cast<Synonym&>(assignSynonym);
    std::unordered_set<Stmt> allStmts = reader.getStmts();
    std::vector<std::string> stmtNumbers = {};

    // Temporary solution to ExpressionSpec
    ClauseArgument* second;
    if (secondArg.isExpressionSpec()) {
        std::string temp = removeAllWhitespaces(secondArg.getValue());
        temp = temp.substr(2, temp.size() - 4);  // MAGIC NUMBER
        second = new ExpressionSpec(temp);
    } else {
        second = &secondArg;
    }

    for (Stmt stmt : allStmts) {
        if (reader.hasPattern(stmt.stmtNum, firstArg, *second)) {
            stmtNumbers.push_back(std::to_string(stmt.stmtNum));
        }
    }

    return {aSyn, stmtNumbers};
}

void PatternClause::checkSemantic(SynonymStore* store) {}
