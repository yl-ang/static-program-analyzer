#include "CallsStar.h"

CallsStar::CallsStar(ClauseArgument& caller, ClauseArgument& callee) : callee(caller), caller(callee) {}

bool CallsStar::validateArguments() {
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

ClauseResult CallsStar::evaluate(PKBFacadeReader& reader) {
    if (callee.isSynonym() && caller.isSynonym()) {
        return evaluateBothSynonyms(reader);
    }

    if (callee.isSynonym()) {
        return evaluateCalleeSynonym(reader);
    }

    if (caller.isSynonym()) {
        return evaluateCallerSynonym(reader);
    }

    return {reader.hasCallStarRelationship(callee, caller)};
}

ClauseResult CallsStar::evaluateCalleeSynonym(PKBFacadeReader& reader) {
    std::unordered_set<Procedure> callers{};
    if (caller.isWildcard()) {
        callers = reader.getProcedures();
    } else {
        callers.insert(caller.getValue());
    }

    SynonymValues values{};
    for (Procedure callerProc : callers) {
        const std::unordered_set<std::string>& callees = reader.getCalleeStar(callerProc);
        values.insert(values.end(), callees.begin(), callees.end());
    }

    Synonym syn = dynamic_cast<Synonym&>(callee);
    return {syn, values};
}

ClauseResult CallsStar::evaluateCallerSynonym(PKBFacadeReader& reader) {
    std::unordered_set<Procedure> callees{};
    if (callee.isWildcard()) {
        callees = reader.getProcedures();
    } else {
        callees.insert(callee.getValue());
    }

    SynonymValues values{};
    for (Procedure calleeProc : callees) {
        const std::unordered_set<std::string>& callers = reader.getCallerStar(calleeProc);
        values.insert(values.end(), callers.begin(), callers.end());
    }

    Synonym syn = dynamic_cast<Synonym&>(caller);
    return {syn, values};
}

ClauseResult CallsStar::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym callerSyn = dynamic_cast<Synonym&>(caller);
    Synonym calleeSyn = dynamic_cast<Synonym&>(callee);

    SynonymValues callerValues{};
    SynonymValues calleeValues{};
    for (Procedure callerProc : reader.getProcedures()) {
        const auto& callees = reader.getCalleeStar(callerProc);
        for (const auto& calleeProc : callees) {
            callerValues.push_back(callerProc);
            calleeValues.push_back(calleeProc);
        }
    }

    std::vector synonyms = {callerSyn, calleeSyn};
    std::vector values = {callerValues, calleeValues};
    return {synonyms, values};
}

bool CallsStar::isSimpleResult() const {
    return !callee.isSynonym() && !caller.isSynonym();
}
