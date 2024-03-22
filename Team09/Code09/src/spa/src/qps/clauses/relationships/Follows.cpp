#include "Follows.h"

Follows::Follows(ClauseArgument& followee, ClauseArgument& follower) : BaseFollows(followee, follower) {}

bool Follows::hasFollowRelationship(PKBFacadeReader& reader) {
    return reader.hasFollowRelationship(followee, follower);
}

StmtSet Follows::getFollowers(PKBFacadeReader& reader, const StmtNum& stmtNum) {
    std::optional stmtOpt = reader.getFollower(stmtNum);
    return stmtOpt.has_value() ? StmtSet{stmtOpt.value()} : StmtSet{};
}

StmtSet Follows::getFollowees(PKBFacadeReader& reader, const StmtNum& stmtNum) {
    std::optional stmtOpt = reader.getFollowee(stmtNum);
    return stmtOpt.has_value() ? StmtSet{stmtOpt.value()} : StmtSet{};
}
