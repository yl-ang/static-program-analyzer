#include "BaseNext.h"

#include <queue>

#include "qps/clauseArguments/Integer.h"
#include "qps/clauses/ClauseEvaluatorUtils.h"

BaseNext::BaseNext(std::shared_ptr<ClauseArgument> currentStmt, std::shared_ptr<ClauseArgument> nextStmt)
    : currentStmt(currentStmt), nextStmt(nextStmt) {}

bool BaseNext::isSimpleResult() const {
    return !currentStmt->isSynonym() && !nextStmt->isSynonym();
}

bool BaseNext::validateArguments() {
    if (currentStmt->isSynonym()) {
        std::shared_ptr<Synonym> first = std::dynamic_pointer_cast<Synonym>(currentStmt);
        if (first->getType() == DesignEntityType::VARIABLE || first->getType() == DesignEntityType::CONSTANT ||
            first->getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    if (nextStmt->isSynonym()) {
        std::shared_ptr<Synonym> second = std::dynamic_pointer_cast<Synonym>(nextStmt);
        if (second->getType() == DesignEntityType::VARIABLE || second->getType() == DesignEntityType::CONSTANT ||
            second->getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    return true;
}

ClauseResult BaseNext::evaluate(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    if (isSimpleResult()) {
        return {hasNextRelationship(reader)};
    }

    if ((currentStmt->isSynonym() && nextStmt->isWildcard()) || (currentStmt->isWildcard() && nextStmt->isSynonym())) {
        return evaluateSynonymWildcard(reader);
    }

    if ((currentStmt->isSynonym() && nextStmt->isInteger()) || (currentStmt->isInteger() && nextStmt->isSynonym())) {
        return evaluateSynonymInteger(reader, evalDb);
    }

    return evaluateBothSynonyms(reader, evalDb);
}

ClauseResult BaseNext::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    bool currentStmtIsSynonym = currentStmt->isSynonym();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(currentStmtIsSynonym ? this->currentStmt : this->nextStmt);

    std::unordered_set<Stmt> allStmts{};

    if (syn.getType() == DesignEntityType::STMT) {
        allStmts = reader.getStmts();
    } else {
        allStmts = reader.getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()]);
    }

    std::vector<Row> values{};
    for (Stmt stmt : allStmts) {
        StmtNum stmtNum = stmt.stmtNum;
        std::unordered_set<StmtNum> stmtNums;
        // Even for next*, we only bother checking if it has a nexter/nextee.
        // if it doesnt, then it cannot have nexterT/nexteeT
        if (currentStmtIsSynonym) {
            stmtNums = reader.getNexter(stmtNum);
        } else {
            // Check that this stmt has nextee
            stmtNums = reader.getNextee(stmtNum);
        }

        if (!stmtNums.empty()) {
            values.push_back(Row{{syn.getName(), std::to_string(stmtNum)}});
        }
    }

    return {syn, values};
}

ClauseResult BaseNext::evaluateSynonymInteger(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    bool currentStmtIsSynonym = currentStmt->isSynonym();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(currentStmtIsSynonym ? this->currentStmt : this->nextStmt);
    StmtNum stmtNum = std::stoi(currentStmtIsSynonym ? this->nextStmt->getValue() : this->currentStmt->getValue());

    std::unordered_set<StmtNum> synonymStmtNums;
    if (currentStmtIsSynonym) {
        synonymStmtNums = getNextees(reader, stmtNum);
    } else {
        synonymStmtNums = getNexters(reader, stmtNum);
    }

    if (synonymStmtNums.empty()) {
        return {syn, {}};
    }

    auto potentialResults = evalDb.getStmts(syn);
    std::vector<Row> results{};

    for (const auto& stmtNum : synonymStmtNums) {
        if (potentialResults.find(stmtNum) != potentialResults.end()) {
            results.push_back(Row{{syn.getName(), std::to_string(stmtNum)}});
        }
    }

    return {syn, results};
}
