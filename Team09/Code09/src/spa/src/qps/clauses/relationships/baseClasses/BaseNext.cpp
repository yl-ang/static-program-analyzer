#include "BaseNext.h"

#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseEvaluatorUtils.h"

BaseNext::BaseNext(ClauseArgument& currentStmt, ClauseArgument& nextStmt)
    : currentStmt(currentStmt), nextStmt(nextStmt) {}

bool BaseNext::isSimpleResult() const {
    return !currentStmt.isSynonym() && !nextStmt.isSynonym();
}

bool BaseNext::validateArguments() {
    if (currentStmt.isSynonym()) {
        Synonym first = dynamic_cast<Synonym&>(currentStmt);
        if (first.getType() == DesignEntityType::VARIABLE || first.getType() == DesignEntityType::CONSTANT ||
            first.getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    if (nextStmt.isSynonym()) {
        Synonym second = dynamic_cast<Synonym&>(nextStmt);
        if (second.getType() == DesignEntityType::VARIABLE || second.getType() == DesignEntityType::CONSTANT ||
            second.getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    return true;
}

ClauseResult BaseNext::evaluate(PKBFacadeReader& reader) {
    if (isSimpleResult()) {
        return {hasNextRelationship(reader)};
    }

    if ((currentStmt.isSynonym() && nextStmt.isWildcard()) || (currentStmt.isWildcard() && nextStmt.isSynonym())) {
        return evaluateSynonymWildcard(reader);
    }

    if ((currentStmt.isSynonym() && nextStmt.isInteger()) || (currentStmt.isInteger() && nextStmt.isSynonym())) {
        return evaluateSynonymInteger(reader);
    }

    return evaluateBothSynonyms(reader);
}

ClauseResult BaseNext::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    bool currentStmtIsSynonym = currentStmt.isSynonym();
    Synonym syn = dynamic_cast<Synonym&>(currentStmtIsSynonym ? this->currentStmt : this->nextStmt);

    std::unordered_set<Stmt> allStmts{};

    if (syn.getType() == DesignEntityType::STMT) {
        allStmts = reader.getStmts();
    } else {
        allStmts = reader.getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()]);
    }

    SynonymValues values{};
    for (Stmt stmt : allStmts) {
        StmtNum stmtNum = stmt.stmtNum;
        std::unordered_set<StmtNum> stmtNums;
        if (currentStmtIsSynonym) {
            // Check that this stmt has nexter
            stmtNums = reader.getNexter(stmtNum);
        } else {
            // Check that this stmt has nextee
            stmtNums = reader.getNextee(stmtNum);
        }

        if (!stmtNums.empty()) {
            values.push_back(std::to_string(stmtNum));
        }
    }

    return {syn, values};
}

ClauseResult BaseNext::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool currentStmtIsSynonym = currentStmt.isSynonym();
    Synonym syn = dynamic_cast<Synonym&>(currentStmtIsSynonym ? this->currentStmt : this->nextStmt);
    Integer integer = dynamic_cast<Integer&>(currentStmtIsSynonym ? this->nextStmt : this->currentStmt);

    StmtNum stmtNum = std::stoi(integer.getValue());
    std::unordered_set<StmtNum> synonymStmtNums;
    if (currentStmtIsSynonym) {
        synonymStmtNums = getNextees(reader, stmtNum);
    } else {
        synonymStmtNums = getNexters(reader, stmtNum);
    }

    if (synonymStmtNums.empty()) {
        return {syn, {}};
    }

    return {syn, ClauseEvaluatorUtils::filterStatementsByType(reader, syn.getType(), synonymStmtNums)};
}

ClauseResult BaseNext::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym currentSyn = dynamic_cast<Synonym&>(currentStmt);
    Synonym nextSyn = dynamic_cast<Synonym&>(nextStmt);

    std::vector<Synonym> synonyms{currentSyn, nextSyn};

    SynonymValues currentSynValues{}, nextSynValues{};

    const std::unordered_set<Stmt>& allStmts =
        currentSyn.getType() != DesignEntityType::STMT
            ? reader.getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[currentSyn.getType()])
            : reader.getStmts();

    for (const Stmt& currStmt : allStmts) {
        std::unordered_set<StmtNum> nexters = getNexters(reader, currStmt.stmtNum);
        if (nexters.empty()) {
            continue;
        }

        if (currentSyn == nextSyn) {
            if (nexters.find(currStmt.stmtNum) != nexters.end()) {
                currentSynValues.push_back(std::to_string(currStmt.stmtNum));
                nextSynValues.push_back(std::to_string(currStmt.stmtNum));
            }

            continue;
        }
        std::vector nexterStmts = ClauseEvaluatorUtils::filterStatementsByType(reader, nextSyn.getType(), nexters);

        nextSynValues.reserve(nextSynValues.size() + nexterStmts.size());
        nextSynValues.insert(nextSynValues.end(), nexterStmts.begin(), nexterStmts.end());

        currentSynValues.reserve(currentSynValues.size() + nexterStmts.size());
        currentSynValues.insert(currentSynValues.end(), nexterStmts.size(), std::to_string(currStmt.stmtNum));
    }

    std::vector<SynonymValues> synonymValues{currentSynValues, nextSynValues};
    return {synonyms, synonymValues};
}
