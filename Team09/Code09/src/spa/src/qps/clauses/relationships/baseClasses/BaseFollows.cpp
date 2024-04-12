#include "BaseFollows.h"

#include "qps/clauseArguments/Integer.h"

BaseFollows::BaseFollows(std::shared_ptr<ClauseArgument> followee, std::shared_ptr<ClauseArgument> follower)
    : followee(followee), follower(follower) {}

bool BaseFollows::validateArguments() {
    if (followee->isSynonym()) {
        std::shared_ptr<Synonym> first = std::dynamic_pointer_cast<Synonym>(followee);
        if (first->getType() == DesignEntityType::VARIABLE || first->getType() == DesignEntityType::CONSTANT ||
            first->getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    if (follower->isSynonym()) {
        std::shared_ptr<Synonym> second = std::dynamic_pointer_cast<Synonym>(follower);
        if (second->getType() == DesignEntityType::VARIABLE || second->getType() == DesignEntityType::CONSTANT ||
            second->getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    return true;
}

ClauseResult BaseFollows::evaluate(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    if (isSimpleResult()) {
        return {hasFollowRelationship(reader)};
    }

    if ((followee->isSynonym() && follower->isWildcard()) || (followee->isWildcard() && follower->isSynonym())) {
        return evaluateSynonymWildcard(reader, evalDb);
    }

    if ((followee->isSynonym() && follower->isInteger()) || (followee->isInteger() && follower->isSynonym())) {
        return evaluateSynonymInteger(reader, evalDb);
    }

    return evaluateBothSynonyms(reader, evalDb);
}

bool BaseFollows::isSimpleResult() const {
    return !followee->isSynonym() && !follower->isSynonym();
}

ClauseResult BaseFollows::evaluateSynonymWildcard(PKBFacadeReader& reader,
                                                  const std::shared_ptr<EvaluationDb>& evalDb) {
    bool followeeIsSynonym = followee->isSynonym();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(followeeIsSynonym ? this->followee : this->follower);

    SynonymValues values{};
    for (StmtNum stmtNum : evalDb->getStmts(syn)) {
        StmtSet stmtNums;
        if (followeeIsSynonym) {
            // If stmt has a follower, then this is a followee
            stmtNums = getFollowers(reader, stmtNum);
        } else {
            // If stmt has a followee, then this is a follower
            stmtNums = getFollowees(reader, stmtNum);
        }

        if (!stmtNums.empty()) {
            values.push_back(std::to_string(stmtNum));
        }
    }

    return {syn, values};
}

ClauseResult BaseFollows::evaluateSynonymInteger(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    bool followeeIsSynonym = followee->isSynonym();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(followeeIsSynonym ? this->followee : this->follower);
    StmtNum stmtNum = std::stoi(followeeIsSynonym ? this->follower->getValue() : this->followee->getValue());

    std::unordered_set<StmtNum> stmtNums;
    if (followeeIsSynonym) {
        stmtNums = getFollowees(reader, stmtNum);
    } else {
        stmtNums = getFollowers(reader, stmtNum);
    }

    std::unordered_set<StmtNum> stmtNumsFiltered{};
    auto existingValues = evalDb->getStmts(syn);
    for (StmtNum stmtNum : stmtNums) {
        if (auto it = existingValues.find(stmtNum); it != existingValues.end()) {
            stmtNumsFiltered.insert(stmtNum);
        }
    }

    SynonymValues values{};
    if (syn.getType() == DesignEntityType::STMT) {
        for (StmtNum stmtNum : stmtNumsFiltered) {
            values.push_back(std::to_string(stmtNum));
        }
        return {syn, values};
    }

    for (StmtNum stmtNum : stmtNumsFiltered) {
        std::optional<Stmt> stmt = reader.getStatementByStmtNum(stmtNum);
        if (!stmt.has_value()) {
            continue;
        }
        StatementType synonymType = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()];
        if (stmt.value().type == synonymType) {
            values.push_back(std::to_string(stmtNum));
        }
    }

    return {syn, values};
}

ClauseResult BaseFollows::evaluateBothSynonyms(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    Synonym followeeSyn = *std::dynamic_pointer_cast<Synonym>(followee);
    Synonym followerSyn = *std::dynamic_pointer_cast<Synonym>(follower);

    std::vector<Synonym> synonyms = {followeeSyn, followerSyn};
    if (followeeSyn == followerSyn) {
        return {synonyms, {}};
    }

    SynonymValues followeeValues{};
    SynonymValues followerValues{};

    for (const StmtNum& followee : evalDb->getStmts(followeeSyn)) {
        StmtSet followers{getFollowers(reader, followee)};
        for (StmtNum followerStmtNum : followers) {
            std::optional<Stmt> followerStmtOpt = reader.getStatementByStmtNum(followerStmtNum);
            if (followerSyn.getType() == DesignEntityType::STMT ||
                (followerStmtOpt.has_value() &&
                 followerStmtOpt.value().type == DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[followerSyn.getType()])) {
                followeeValues.push_back(std::to_string(followee));
                followerValues.push_back(std::to_string(followerStmtNum));
            }
        }
    }

    std::vector<SynonymValues> synonymValues{followeeValues, followerValues};

    return {synonyms, synonymValues};
}
