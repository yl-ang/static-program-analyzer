#include "FollowsStar.h"

FollowsStar::FollowsStar(ClauseArgument& followee, ClauseArgument& follower) : followee(followee), follower(follower) {}

bool FollowsStar::validateArguments() {
    if (followee.isSynonym()) {
        Synonym first = dynamic_cast<Synonym&>(followee);
        if (first.getType() == DesignEntityType::VARIABLE || first.getType() == DesignEntityType::CONSTANT ||
            first.getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    if (follower.isSynonym()) {
        Synonym second = dynamic_cast<Synonym&>(follower);
        if (second.getType() == DesignEntityType::VARIABLE || second.getType() == DesignEntityType::CONSTANT ||
            second.getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    return true;
}

ClauseResult FollowsStar::evaluate(PKBFacadeReader& reader) {
    if (isSimpleResult()) {
        return {reader.hasFollowStarRelationship(followee, follower)};
    }

    if ((followee.isSynonym() && follower.isWildcard()) || (followee.isWildcard() && follower.isSynonym())) {
        return evaluateSynonymWildcard(reader);
    }

    if ((followee.isSynonym() && follower.isInteger()) || (followee.isInteger() && follower.isSynonym())) {
        return evaluateSynonymInteger(reader);
    }

    return evaluateBothSynonyms(reader);
}

ClauseResult FollowsStar::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    bool followeeIsSynonym = followee.isSynonym();
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
            // If stmt has a follower, then this is a followee
            stmtNumOpt = reader.getFollower(stmtNum);
        } else {
            // If stmt has a followee, then this is a follower
            stmtNumOpt = reader.getFollowee(stmtNum);
        }

        if (stmtNumOpt.has_value()) {
            values.push_back(std::to_string(stmtNum));
        }
    }

    return {syn, values};
}

ClauseResult FollowsStar::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool followeeIsSynonym = followee.isSynonym();
    Synonym syn = dynamic_cast<Synonym&>(followeeIsSynonym ? this->followee : this->follower);
    Integer integer = dynamic_cast<Integer&>(followeeIsSynonym ? this->follower : this->followee);

    StmtNum stmtNum = std::stoi(integer.getValue());
    std::unordered_set<StmtNum> stmtNums;
    if (followeeIsSynonym) {
        stmtNums = reader.getFolloweesStar(stmtNum);
    } else {
        stmtNums = reader.getFollowersStar(stmtNum);
    }

    if (stmtNums.empty()) {
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
    if (followeeSyn == followerSyn) {
        return {synonyms, {}};
    }

    SynonymValues followeeValues{};
    SynonymValues followerValues{};

    for (const Stmt& followee : reader.getStmts()) {
        if (followeeSyn.getType() != DesignEntityType::STMT &&
            DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[followeeSyn.getType()] != followee.type) {
            continue;
        }
        std::unordered_set followers{reader.getFollowersStar(followee.stmtNum)};
        for (StmtNum followerStmtNum : followers) {
            std::optional<Stmt> followerStmtOpt = reader.getStatementByStmtNum(followerStmtNum);
            if (followerSyn.getType() == DesignEntityType::STMT ||
                (followerStmtOpt.has_value() &&
                 followerStmtOpt.value().type == DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[followerSyn.getType()])) {
                followeeValues.push_back(std::to_string(followee.stmtNum));
                followerValues.push_back(std::to_string(followerStmtNum));
            }
        }
    }

    std::vector<SynonymValues> synonymValues{followeeValues, followerValues};

    return {synonyms, synonymValues};
}

bool FollowsStar::isSimpleResult() const {
    return !followee.isSynonym() && !follower.isSynonym();
}
