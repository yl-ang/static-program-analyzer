#include "Follows.h"

Follows::Follows(ClauseArgument& followee, ClauseArgument& follower) : followee(followee), follower(follower) {}

bool Follows::validateArguments() {
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
            // Check that this stmt has a follower
            stmtNumOpt = reader.getFollower(stmtNum);
        } else {
            // Check that this stmt follows some followee
            stmtNumOpt = reader.getFollowee(stmtNum);
        }

        if (stmtNumOpt.has_value()) {
            values.push_back(std::to_string(stmtNum));
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

    std::unordered_set<StmtNum> stmtNums{};
    if (stmtNumOpt.has_value()) {
        stmtNums.insert(stmtNumOpt.value());
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

ClauseResult Follows::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym followeeSyn = dynamic_cast<Synonym&>(followee);
    Synonym followerSyn = dynamic_cast<Synonym&>(follower);

    std::vector<Synonym> synonyms{followeeSyn, followerSyn};
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

        std::optional<StmtNum> followerStmtNumOpt = reader.getFollower(followee.stmtNum);

        std::unordered_set<StmtNum> followers{};
        if (followerStmtNumOpt.has_value()) {
            followers.insert(followerStmtNumOpt.value());
        }

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

bool Follows::isSimpleResult() const {
    return !followee.isSynonym() && !follower.isSynonym();
}
