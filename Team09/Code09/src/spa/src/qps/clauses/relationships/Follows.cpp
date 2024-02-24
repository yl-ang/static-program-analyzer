#include "Follows.h"

Follows::Follows(ClauseArgument& followee, ClauseArgument& follower) : followee(followee), follower(follower) {}

ClauseResult Follows::evaluate(PKBFacadeReader& reader) {
    if (hasNoSynonyms() || hasAtLeastOneWildcard()) {
        return {reader.hasFollowRelationship(followee, follower)};
    }

    if ((followee.isSynonym() && follower.isInteger()) || (followee.isInteger() && follower.isSynonym())) {
        return evaluateSynonymInteger(reader, followee.isSynonym());
    }

    return evaluateBothSynonyms(reader);
}

ClauseResult Follows::evaluateSynonymInteger(PKBFacadeReader& reader, bool followeeIsSynonym) {
    Synonym syn = static_cast<Synonym&>(followeeIsSynonym ? this->followee : this->follower);
    Integer integer = static_cast<Integer&>(followeeIsSynonym ? this->follower : this->followee);

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

    Stmt* stmt = reader.getStatementByStmtNum(stmtNumOpt.value());
    StatementType synonymType = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()];
    if (stmt->type != synonymType) {
        return {syn, {}};
    }

    return {syn, {std::to_string(stmtNumOpt.value())}};
}

ClauseResult Follows::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym followeeSyn = static_cast<Synonym&>(followee);
    Synonym followerSyn = static_cast<Synonym&>(follower);

    std::vector<Synonym> synonyms = {followeeSyn, followerSyn};
    SynonymValues followeeValues{};
    SynonymValues followerValues{};

    for (const Stmt& followee : reader.getStmts()) {
        StmtNum followeeStmtNum = followee.stmtNum;
        std::optional<StmtNum> followerStmtNumOpt = reader.getFollower(followeeStmtNum);
        if (!followerStmtNumOpt.has_value()) {
            continue;
        }

        followeeValues.push_back({std::to_string(followerStmtNumOpt.value())});
        followerValues.push_back({std::to_string(followeeStmtNum)});
    }

    std::vector<SynonymValues> synonymValues{followeeValues, followerValues};

    return {synonyms, synonymValues};
}

bool Follows::hasNoSynonyms() {
    return !followee.isSynonym() && !follower.isSynonym();
}

bool Follows::hasAtLeastOneWildcard() {
    return followee.isWildcard() || follower.isWildcard();
}
