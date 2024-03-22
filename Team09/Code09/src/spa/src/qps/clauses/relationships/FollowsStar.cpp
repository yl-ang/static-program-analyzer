#include "FollowsStar.h"

FollowsStar::FollowsStar(ClauseArgument& followee, ClauseArgument& follower) : BaseFollows(followee, follower) {}

bool FollowsStar::hasFollowRelationship(PKBFacadeReader& reader) {
    return reader.hasFollowStarRelationship(followee, follower);
}

StmtSet FollowsStar::getFollowers(PKBFacadeReader& reader, const StmtNum& stmtNum) {
    return reader.getFollowersStar(stmtNum);
}

StmtSet FollowsStar::getFollowees(PKBFacadeReader& reader, const StmtNum& stmtNum) {
    return reader.getFolloweesStar(stmtNum);
}
