#include "BaseParent.h"

#include "qps/clauseArguments/Integer.h"

BaseParent::BaseParent(std::shared_ptr<ClauseArgument> parent, std::shared_ptr<ClauseArgument> child)
    : parent(parent), child(child) {}

bool BaseParent::validateArguments() {
    if (parent->isSynonym()) {
        std::shared_ptr<Synonym> first = std::dynamic_pointer_cast<Synonym>(parent);
        if (first->getType() == DesignEntityType::VARIABLE || first->getType() == DesignEntityType::CONSTANT ||
            first->getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    if (child->isSynonym()) {
        std::shared_ptr<Synonym> second = std::dynamic_pointer_cast<Synonym>(child);
        if (second->getType() == DesignEntityType::VARIABLE || second->getType() == DesignEntityType::CONSTANT ||
            second->getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    return true;
}

ClauseResult BaseParent::evaluate(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    if (isSimpleResult()) {
        return {hasParentRelationship(reader)};
    }

    if ((parent->isInteger() && child->isSynonym()) || (parent->isSynonym() && child->isInteger())) {
        return evaluateSynonymInteger(reader, evalDb);
    }

    if ((parent->isWildcard() && child->isSynonym()) || (parent->isSynonym() && child->isWildcard())) {
        return evaluateSynonymWildcard(reader, evalDb);
    }

    return evaluateBothSynonyms(reader, evalDb);
}

ClauseResult BaseParent::evaluateSynonymWildcard(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    bool parentIsSynonym = parent->isSynonym();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(parentIsSynonym ? parent : child);

    auto allStmts = evalDb.getStmts(syn);

    std::unordered_set<StmtNum> uniqueValues{};
    for (StmtNum stmtNum : allStmts) {
        auto otherResults = parentIsSynonym ? getChildren(reader, stmtNum) : getParents(reader, stmtNum);
        if (!otherResults.empty()) {
            uniqueValues.insert(stmtNum);
        }
    }

    std::vector<Row> values{};
    for (const int& value : uniqueValues) {
        values.push_back(Row{{syn.getName(), std::to_string(value)}});
    }

    return {syn, values};
}

ClauseResult BaseParent::evaluateSynonymInteger(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    bool parentIsSynonym = parent->isSynonym();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(parentIsSynonym ? parent : child);
    StmtNum stmtNum = std::stoi(parentIsSynonym ? child->getValue() : parent->getValue());

    std::unordered_set<StmtNum> stmtNums;
    if (parentIsSynonym) {
        stmtNums = getParents(reader, stmtNum);
    } else {
        stmtNums = getChildren(reader, stmtNum);
    }

    std::vector<Row> values{};
    auto potentialResults = evalDb.getStmts(syn);
    for (auto potentialResult : evalDb.getStmts(syn)) {
        if (stmtNums.find(potentialResult) != stmtNums.end()) {
            values.push_back(Row{{syn.getName(), std::to_string(potentialResult)}});
        }
    }

    return {syn, values};
}

ClauseResult BaseParent::evaluateBothSynonyms(PKBFacadeReader& reader, EvaluationDb& evalDb) {
    Synonym parentSyn = *std::dynamic_pointer_cast<Synonym>(parent);
    Synonym childSyn = *std::dynamic_pointer_cast<Synonym>(child);

    std::vector<Synonym> synonyms{parentSyn, childSyn};
    if (parentSyn == childSyn) {
        return {synonyms, {}};
    }

    std::vector<Row> values{};

    auto potentialChildrenValues = evalDb.getStmts(childSyn);

    for (const StmtNum& parentStmtNum : evalDb.getStmts(parentSyn)) {
        std::unordered_set<StmtNum> children = getChildren(reader, parentStmtNum);
        for (const StmtNum& childStmtNum : children) {
            if (potentialChildrenValues.find(childStmtNum) != potentialChildrenValues.end()) {
                values.push_back(Row{{parentSyn.getName(), std::to_string(parentStmtNum)},
                                     {childSyn.getName(), std::to_string(childStmtNum)}});
            }
        }
    }

    return {synonyms, values};
}

bool BaseParent::isSimpleResult() const {
    return !parent->isSynonym() && !child->isSynonym();
}
