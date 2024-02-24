#include "Follows.h"

Follows::Follows(ClauseArgument& followee, ClauseArgument& follower) : followee(followee), follower(follower) {}

ClauseResult Follows::evaluate(PKBFacadeReader& reader) {
    if (isSimpleResult()) {
        return {reader.hasFollowRelationship(followee, follower)};
    }

    if ((followee.isSynonym() && follower.isWildcard()) || (followee.isWildcard() && follower.isSynonym())) {
        return evaluateSynonymWildcard(reader);
    }

    if ((followee.isSynonym() && follower.isInteger()) || (followee.isInteger() && follower.isSynonym())) {
        return evaluateSynonymInteger(reader);
    }

    return evaluateBothSynonyms(reader);
}

ClauseResult Follows::evaluateSynonymWildcard(PKBFacadeReader& reader) {
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

ClauseResult Follows::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool followeeIsSynonym = followee.isSynonym();
    Synonym syn = dynamic_cast<Synonym&>(followeeIsSynonym ? this->followee : this->follower);
    Integer integer = dynamic_cast<Integer&>(followeeIsSynonym ? this->follower : this->followee);

    StmtNum stmtNum = std::stoi(integer.getValue());
    std::optional<StmtNum> stmtNumOpt;
    if (followeeIsSynonym) {
        stmtNumOpt = reader.getFollowee(stmtNum);
    } else {
        stmtNumOpt = reader.getFollower(stmtNum);
    }

    if (!stmtNumOpt.has_value()) {
        return {syn, {}};
    }

    if (syn.getType() == DesignEntityType::STMT) {
        return {syn, {std::to_string(stmtNumOpt.value())}};
    }

    std::optional<Stmt> stmtOpt = reader.getStatementByStmtNum(stmtNumOpt.value());
    const auto synType = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()];
    if (stmtOpt.has_value() && stmtOpt.value().type == synType) {
        return {syn, {std::to_string(stmtNumOpt.value())}};
    }

    return {syn, {std::to_string(stmtNumOpt.value())}};
}

ClauseResult Follows::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym followeeSyn = dynamic_cast<Synonym&>(followee);
    Synonym followerSyn = dynamic_cast<Synonym&>(follower);

    SynonymValues followeeValues{};
    SynonymValues followerValues{};

    for (const Stmt& followee : reader.getStmts()) {
        StmtNum followeeStmtNum = followee.stmtNum;
        std::optional<StmtNum> followerStmtNumOpt = reader.getFollower(followeeStmtNum);
        if (!followerStmtNumOpt.has_value()) {
            continue;
        }

        followeeValues.push_back(std::to_string(followeeStmtNum));
        followerValues.push_back(std::to_string(followerStmtNumOpt.value()));
    }

    std::vector<Synonym> synonyms{followeeSyn, followerSyn};
    std::vector<SynonymValues> synonymValues{followeeValues, followerValues};

    return {synonyms, synonymValues};
}

bool Follows::isSimpleResult() const {
    return !followee.isSynonym() && !follower.isSynonym();
}
