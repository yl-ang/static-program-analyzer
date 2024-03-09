#include "Next.h"

Next::Next(ClauseArgument& currentStmt, ClauseArgument& nextStmt) : currentStmt(currentStmt), nextStmt(nextStmt) {}

bool Next::isSimpleResult() const {
    return !currentStmt.isSynonym() && !nextStmt.isSynonym();
}

ClauseResult Next::evaluate(PKBFacadeReader& reader) {
    if (isSimpleResult()) {
        return {reader.hasNextRelationship(currentStmt, nextStmt)};
    }

    if ((currentStmt.isSynonym() && nextStmt.isWildcard()) || (currentStmt.isWildcard() && nextStmt.isSynonym())) {
        return evaluateSynonymWildcard(reader);
    }

    if ((currentStmt.isSynonym() && nextStmt.isInteger()) || (currentStmt.isInteger() && nextStmt.isSynonym())) {
        return evaluateSynonymInteger(reader);
    }

    return evaluateBothSynonyms(reader);
}

ClauseResult Next::evaluateSynonymWildcard(PKBFacadeReader& reader) {
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

ClauseResult Next::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool currentStmtIsSynonym = currentStmt.isSynonym();
    Synonym syn = dynamic_cast<Synonym&>(currentStmtIsSynonym ? this->currentStmt : this->nextStmt);
    Integer integer = dynamic_cast<Integer&>(currentStmtIsSynonym ? this->nextStmt : this->currentStmt);

    StmtNum stmtNum = std::stoi(integer.getValue());
    std::unordered_set<StmtNum> synonymStmtNums;
    if (currentStmtIsSynonym) {
        synonymStmtNums = reader.getNextee(stmtNum);
    } else {
        synonymStmtNums = reader.getNexter(stmtNum);
    }

    if (synonymStmtNums.empty()) {
        return {syn, {}};
    }

    std::vector<std::string> values{};
    if (syn.getType() == DesignEntityType::STMT) {
        for (StmtNum stmtNum : synonymStmtNums) {
            values.push_back(std::to_string(stmtNum));
        }
    } else {
        for (StmtNum stmtNum : synonymStmtNums) {
            std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
            if (stmt.has_value() && stmt.value().type == DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()]) {
                values.push_back(std::to_string(stmtNum));
            }
        }
    }

    return {syn, values};
}

ClauseResult Next::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym currentSyn = dynamic_cast<Synonym&>(currentStmt);
    Synonym nextSyn = dynamic_cast<Synonym&>(nextStmt);

    std::vector<Synonym> synonyms{currentSyn, nextSyn};
    if (currentSyn == nextSyn) {
        return {synonyms, {}};
    }

    SynonymValues currentSynValues{}, nextSynValues{};

    for (const Stmt& currStmt : reader.getStmts()) {
        if (currentSyn.getType() != DesignEntityType::STMT &&
            currStmt.type != DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[currentSyn.getType()]) {
            continue;
        }

        std::unordered_set<StmtNum> nexters = reader.getNexter(currStmt.stmtNum);
        if (nexters.empty()) {
            continue;
        }

        for (StmtNum nexter : nexters) {
            if (nextSyn.getType() == DesignEntityType::STMT) {
                currentSynValues.push_back(std::to_string(currStmt.stmtNum));
                nextSynValues.push_back(std::to_string(nexter));
            } else {
                std::optional<Stmt> nexterStmt = reader.getStatementByStmtNum(nexter);
                if (nexterStmt.has_value() &&
                    nexterStmt.value().type == DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[nextSyn.getType()]) {
                    currentSynValues.push_back(std::to_string(currStmt.stmtNum));
                    nextSynValues.push_back(std::to_string(nexter));
                }
            }
        }
    }

    std::vector<SynonymValues> synonymValues{currentSynValues, nextSynValues};
    return {synonyms, synonymValues};
}
