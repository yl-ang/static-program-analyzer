#include "Next.h"

Next::Next(ClauseArgument& currentStmt, ClauseArgument& nextStmt) : BaseNext(currentStmt, nextStmt) {}

bool Next::hasNextRelationship(PKBFacadeReader& reader) {
    return reader.hasNextRelationship(currentStmt, nextStmt);
}

StmtSet Next::getNexters(PKBFacadeReader& reader, const StmtNum& nextee) {
    return reader.getNexter(nextee);
}

StmtSet Next::getNextees(PKBFacadeReader& reader, const StmtNum& nexter) {
    return reader.getNextee(nexter);
}
