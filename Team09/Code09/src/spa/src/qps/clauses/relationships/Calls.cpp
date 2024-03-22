#include "Calls.h"

Calls::Calls(ClauseArgument& caller, ClauseArgument& callee) : BaseCalls(caller, callee) {}

bool Calls::hasCallRelationship(PKBFacadeReader& reader) {
    return reader.hasCallRelationship(this->caller, this->callee);
}

ProcedureSet Calls::getCallee(PKBFacadeReader& reader, const Procedure& caller) {
    return reader.getCallee(caller);
}

ProcedureSet Calls::getCaller(PKBFacadeReader& reader, const Procedure& callee) {
    return reader.getCaller(callee);
}
