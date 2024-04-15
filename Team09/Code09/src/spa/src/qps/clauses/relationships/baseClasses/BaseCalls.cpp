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

ClauseResult BaseCalls::evaluate(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    if (isSimpleResult()) {
        return {hasCallRelationship(reader)};
    }

    if (callee->isWildcard() || caller->isWildcard()) {
        return evaluateSynonymWildcard(reader, evalDb);
    }

    if (callee->isLiteral() || caller->isLiteral()) {
        return evaluateSynonymLiteral(reader, evalDb);
    }

    return evaluateBothSynonyms(reader, evalDb);
}

ClauseResult BaseCalls::evaluateSynonymWildcard(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(callee->isSynonym() ? callee : caller);

    auto potentialResults = evalDb.getProcedures(syn);
    std::vector<Row> result{};

    for (const auto& procName : potentialResults) {
        auto otherProcNames = callee->isSynonym() ? getCaller(reader, procName) : getCallee(reader, procName);
        if (!otherProcNames.empty()) {
            result.push_back(Row{{syn.getName(), procName}});
        }
    }

    return {syn, result};
}

ClauseResult BaseCalls::evaluateSynonymLiteral(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    bool calleeIsSynonym = callee->isSynonym();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(calleeIsSynonym ? callee : caller);

    auto potentialResults = evalDb.getProcedures(syn);
    std::vector<Row> result{};

    // Get all caller/callee of the literal, the opposite of whichever the literal is.
    auto allResults = calleeIsSynonym ? getCallee(reader, caller->getValue()) : getCaller(reader, callee->getValue());
    for (auto procName : potentialResults) {
        if (allResults.find(procName) != allResults.end()) {
            result.push_back(Row{{syn.getName(), procName}});
        }
    }

    return {syn, result};
}

ClauseResult BaseCalls::evaluateBothSynonyms(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym callerSyn = *std::dynamic_pointer_cast<Synonym>(caller);
    Synonym calleeSyn = *std::dynamic_pointer_cast<Synonym>(callee);

    std::vector<Synonym> synonyms{callerSyn, calleeSyn};
    if (callerSyn == calleeSyn) {
        return {synonyms, {}};
    }

    std::vector<Row> rows{};

    auto allCallerProcs = evalDb.getProcedures(callerSyn);
    auto allCalleeProcs = evalDb.getProcedures(calleeSyn);

    if (allCallerProcs.size() < allCalleeProcs.size()) {
        for (const Procedure& callerProc : allCallerProcs) {
            for (const auto& calleeProc : getCallee(reader, callerProc)) {
                if (allCalleeProcs.find(calleeProc) == allCalleeProcs.end()) {
                    continue;
                }

                rows.push_back(Row{{callerSyn.getName(), callerProc}, {calleeSyn.getName(), calleeProc}});
            }
        }
    } else {
        for (const Procedure& calleeProc : allCalleeProcs) {
            for (const auto& callerProc : getCaller(reader, calleeProc)) {
                if (allCallerProcs.find(callerProc) == allCallerProcs.end()) {
                    continue;
                }

                rows.push_back(Row{{callerSyn.getName(), callerProc}, {calleeSyn.getName(), calleeProc}});
            }
        }
    }

    return {synonyms, rows};
}
