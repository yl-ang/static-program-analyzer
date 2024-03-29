#include "BaseCalls.h"

BaseCalls::BaseCalls(std::shared_ptr<ClauseArgument> caller, std::shared_ptr<ClauseArgument> callee)
    : caller(caller), callee(callee) {}

bool BaseCalls::validateArguments() {
    if (caller->isSynonym()) {
        std::shared_ptr<Synonym> first = std::dynamic_pointer_cast<Synonym>(caller);
        if (first->getType() != DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    if (callee->isSynonym()) {
        std::shared_ptr<Synonym> second = std::dynamic_pointer_cast<Synonym>(callee);
        if (second->getType() != DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    return true;
}

bool BaseCalls::isSimpleResult() const {
    return !caller->isSynonym() && !callee->isSynonym();
}

ClauseResult BaseCalls::evaluate(PKBFacadeReader& reader) {
    if (callee->isSynonym() && caller->isSynonym()) {
        return evaluateBothSynonyms(reader);
    }

    if (callee->isSynonym()) {
        return evaluateCalleeSynonym(reader);
    }

    if (caller->isSynonym()) {
        return evaluateCallerSynonym(reader);
    }

    return {hasCallRelationship(reader)};
}

ClauseResult BaseCalls::evaluateCalleeSynonym(PKBFacadeReader& reader) {
    std::unordered_set<Procedure> callers{};
    if (caller->isWildcard()) {
        callers = reader.getProcedures();
    } else {
        callers.insert(caller->getValue());
    }

    SynonymValues values{};
    for (Procedure callerProc : callers) {
        const std::unordered_set<std::string>& callees = getCallee(reader, callerProc);
        values.insert(values.end(), callees.begin(), callees.end());
    }

    Synonym syn = *std::dynamic_pointer_cast<Synonym>(callee);
    return {syn, values};
}

ClauseResult BaseCalls::evaluateCallerSynonym(PKBFacadeReader& reader) {
    std::unordered_set<Procedure> callees{};
    if (callee->isWildcard()) {
        callees = reader.getProcedures();
    } else {
        callees.insert(callee->getValue());
    }

    SynonymValues values{};
    for (Procedure calleeProc : callees) {
        const std::unordered_set<std::string>& callers = getCaller(reader, calleeProc);
        values.insert(values.end(), callers.begin(), callers.end());
    }

    Synonym syn = *std::dynamic_pointer_cast<Synonym>(caller);
    return {syn, values};
}

ClauseResult BaseCalls::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym callerSyn = *std::dynamic_pointer_cast<Synonym>(caller);
    Synonym calleeSyn = *std::dynamic_pointer_cast<Synonym>(callee);

    std::vector<Synonym> synonyms{callerSyn, calleeSyn};
    if (callerSyn == calleeSyn) {
        return {synonyms, {}};
    }

    SynonymValues callerValues{};
    SynonymValues calleeValues{};
    for (Procedure callerProc : reader.getProcedures()) {
        const auto& callees = getCallee(reader, callerProc);
        for (const auto& calleeProc : callees) {
            callerValues.push_back(callerProc);
            calleeValues.push_back(calleeProc);
        }
    }

    std::vector values = {callerValues, calleeValues};
    return {synonyms, values};
}
