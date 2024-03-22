#include "NextStar.h"

NextStar::NextStar(ClauseArgument& currentStmt, ClauseArgument& nextStmt) : BaseNext(currentStmt, nextStmt) {}

bool NextStar::hasNextRelationship(PKBFacadeReader& reader) {
    return reader.hasNextStarRelationship(currentStmt, nextStmt);
}

StmtSet NextStar::getNexters(PKBFacadeReader& reader, const StmtNum& nextee) {
    return reader.getNexterStar(nextee);
}

StmtSet NextStar::getNextees(PKBFacadeReader& reader, const StmtNum& nexter) {
    return reader.getNexteeStar(nexter);
}
