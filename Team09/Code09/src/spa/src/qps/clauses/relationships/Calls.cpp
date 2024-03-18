#include "Calls.h"

Calls::Calls(ClauseArgument& caller, ClauseArgument& callee) : caller(caller), callee(callee) {}

bool Calls::validateArguments() {
    if (caller.isSynonym()) {
        Synonym first = dynamic_cast<Synonym&>(caller);
        if (first.getType() != DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    if (callee.isSynonym()) {
        Synonym second = dynamic_cast<Synonym&>(callee);
        if (second.getType() != DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    return true;
}

ClauseResult Calls::evaluate(PKBFacadeReader& reader) {
    if (callee.isSynonym() && caller.isSynonym()) {
        return evaluateBothSynonyms(reader);
    }

    if (callee.isSynonym()) {
        return evaluateCalleeSynonym(reader);
    }

    if (caller.isSynonym()) {
        return evaluateCallerSynonym(reader);
    }

    return {reader.hasCallRelationship(callee, caller)};
}

ClauseResult Calls::evaluateCalleeSynonym(PKBFacadeReader& reader) {
    std::unordered_set<Procedure> callers{};
    if (caller.isWildcard()) {
        callers = reader.getProcedures();
    } else {
        callers.insert(caller.getValue());
    }

    SynonymValues values{};
    for (Procedure callerProc : callers) {
        const std::unordered_set<std::string>& callees = reader.getCallee(callerProc);
        values.insert(values.end(), callees.begin(), callees.end());
    }

    Synonym syn = dynamic_cast<Synonym&>(callee);
    return {syn, values};
}

ClauseResult Calls::evaluateCallerSynonym(PKBFacadeReader& reader) {
    std::unordered_set<Procedure> callees{};
    if (callee.isWildcard()) {
        callees = reader.getProcedures();
    } else {
        callees.insert(callee.getValue());
    }

    SynonymValues values{};
    for (Procedure calleeProc : callees) {
        const std::unordered_set<std::string>& callers = reader.getCaller(calleeProc);
        values.insert(values.end(), callers.begin(), callers.end());
    }

    Synonym syn = dynamic_cast<Synonym&>(caller);
    return {syn, values};
}

ClauseResult Calls::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym callerSyn = dynamic_cast<Synonym&>(caller);
    Synonym calleeSyn = dynamic_cast<Synonym&>(callee);

    SynonymValues callerValues{};
    SynonymValues calleeValues{};
    for (Procedure callerProc : reader.getProcedures()) {
        const auto& callees = reader.getCallee(callerProc);
        for (const auto& calleeProc : callees) {
            callerValues.push_back(callerProc);
            calleeValues.push_back(calleeProc);
        }
    }

    std::vector synonyms = {callerSyn, calleeSyn};
    std::vector values = {callerValues, calleeValues};
    return {synonyms, values};
}

bool Calls::isSimpleResult() const {
    return !callee.isSynonym() && !caller.isSynonym();
}
