#include "CallsStar.h"

CallsStar::CallsStar(ClauseArgument& caller, ClauseArgument& callee) : BaseCalls(caller, callee) {}

bool CallsStar::hasCallRelationship(PKBFacadeReader& reader) {
    return reader.hasCallStarRelationship(this->caller, this->callee);
}

ProcedureSet CallsStar::getCallee(PKBFacadeReader& reader, const Procedure& caller) {
    return reader.getCalleeStar(caller);
}

ProcedureSet CallsStar::getCaller(PKBFacadeReader& reader, const Procedure& callee) {
    return reader.getCallerStar(callee);
}
