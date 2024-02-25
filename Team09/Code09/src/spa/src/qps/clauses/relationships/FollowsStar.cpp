#include "FollowsStar.h"

FollowsStar::FollowsStar(ClauseArgument& followee, ClauseArgument& follower) : followee(followee), follower(follower) {}

ClauseResult FollowsStar::evaluate(PKBFacadeReader& reader) {
    if (isSimpleResult()) {
        return {reader.hasFollowStarRelationship(followee, follower)};
    }

    if ((followee.isSynonym() && follower.isWildcard()) || (followee.isWildcard() && follower.isSynonym())) {
        return evaluateSynonymWildcard(reader, followee.isSynonym());
    }

    if ((followee.isSynonym() && follower.isInteger()) || (followee.isInteger() && follower.isSynonym())) {
        return evaluateSynonymInteger(reader, followee.isSynonym());
    }

    return evaluateBothSynonyms(reader);
}

ClauseResult FollowsStar::evaluateSynonymWildcard(PKBFacadeReader& reader, bool followeeIsSynonym) {
    Synonym syn = dynamic_cast<Synonym&>(followeeIsSynonym ? this->followee : this->follower);

    std::unordered_set<Stmt> allStmts{};

    if (syn.getType() == DesignEntityType::STMT) {
        allStmts = reader.getStmts();
    } else {
        allStmts = reader.getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()]);
    }

    SynonymValues values{};
    for (Stmt stmt : allStmts) {
        StmtNum stmtNum = stmt.stmtNum;
        std::optional<StmtNum> stmtNumOpt;
        if (followeeIsSynonym) {
            // Get all stmts that are followed by some other stmt
            stmtNumOpt = reader.getFollowee(stmtNum);
        } else {
            // Get all stmts that are following some other stmt
            stmtNumOpt = reader.getFollower(stmtNum);
        }

        if (stmtNumOpt.has_value()) {
            values.push_back(std::to_string(stmtNumOpt.value()));
        }
    }

    return {syn, values};
}

ClauseResult FollowsStar::evaluateSynonymInteger(PKBFacadeReader& reader, bool followeeIsSynonym) {
    Synonym syn = dynamic_cast<Synonym&>(followeeIsSynonym ? this->followee : this->follower);
    Integer integer = dynamic_cast<Integer&>(followeeIsSynonym ? this->follower : this->followee);

    StmtNum stmtNum = std::stoi(integer.getValue());
    std::unordered_set<StmtNum> stmtNums;
    if (followeeIsSynonym) {
        stmtNums = reader.getFolloweesStar(stmtNum);
    } else {
        stmtNums = reader.getFollowersStar(stmtNum);
    }

    if (stmtNums.size() == 0) {
        return {syn, {}};
    }

    if (syn.getType() == DesignEntityType::STMT) {
        SynonymValues values{};
        for (StmtNum stmtNum : stmtNums) {
            values.push_back(std::to_string(stmtNum));
        }
        return {syn, values};
    }

    SynonymValues values{};
    for (StmtNum stmtNum : stmtNums) {
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

ClauseResult FollowsStar::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym followeeSyn = dynamic_cast<Synonym&>(followee);
    Synonym followerSyn = dynamic_cast<Synonym&>(follower);

    std::vector<Synonym> synonyms = {followeeSyn, followerSyn};
    SynonymValues followeeValues{};
    SynonymValues followerValues{};

    for (const Stmt& followee : reader.getStmts()) {
        std::string followeeStmtNumStr = std::to_string(followee.stmtNum);

        for (StmtNum followerStmtNum : reader.getFollowersStar(followee.stmtNum)) {
            followeeValues.push_back(followeeStmtNumStr);
            followerValues.push_back(std::to_string(followerStmtNum));
        }
    }

    std::vector<SynonymValues> synonymValues{followeeValues, followerValues};

    return {synonyms, synonymValues};
}

bool FollowsStar::isSimpleResult() const {
    return !followee.isSynonym() && !follower.isSynonym();
}
