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

ClauseResult BaseParent::evaluate(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
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

ClauseResult BaseParent::evaluateSynonymWildcard(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    bool parentIsSynonym = parent->isSynonym();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(parentIsSynonym ? parent : child);

    std::unordered_set<Stmt> allStmts{};
    if (syn.getType() == DesignEntityType::STMT) {
        allStmts = reader.getStmts();
    } else {
        allStmts = reader.getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()]);
    }

    std::unordered_set<StmtNum> uniqueValues{};
    for (Stmt stmt : allStmts) {
        StmtNum stmtNum = stmt.stmtNum;
        if (parentIsSynonym) {
            // If stmt has children, then this is a parent/grandparent
            std::unordered_set<StmtNum> children = getChildren(reader, stmtNum);
            if (!children.empty()) {
                uniqueValues.insert(stmtNum);
            }
        } else {
            // If stmt has parent, then this is a child/grandchild
            std::unordered_set<StmtNum> parents = getParents(reader, stmtNum);
            if (!parents.empty()) {
                uniqueValues.insert(stmtNum);
            }
        }
    }

    SynonymValues values{};
    for (const int& value : uniqueValues) {
        values.push_back(std::to_string(value));
    }

    return {syn, values};
}

ClauseResult BaseParent::evaluateSynonymInteger(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    bool parentIsSynonym = parent->isSynonym();
    Synonym syn = *std::dynamic_pointer_cast<Synonym>(parentIsSynonym ? parent : child);
    StmtNum stmtNum = std::stoi(parentIsSynonym ? child->getValue() : parent->getValue());

    std::unordered_set<StmtNum> stmtNums;
    if (parentIsSynonym) {
        stmtNums = getParents(reader, stmtNum);
    } else {
        stmtNums = getChildren(reader, stmtNum);
    }

    SynonymValues values{};
    if (syn.getType() == DesignEntityType::STMT) {
        for (StmtNum stmt : stmtNums) {
            values.push_back(std::to_string(stmt));
        }
    } else {
        StatementType synonymType = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()];
        for (StmtNum stmt : stmtNums) {
            std::optional<Stmt> stmtOpt = reader.getStatementByStmtNum(stmt);
            if (stmtOpt.has_value() && stmtOpt.value().type == synonymType) {
                values.push_back(std::to_string(stmt));
            }
        }
    }

    return {syn, values};
}

ClauseResult BaseParent::evaluateBothSynonyms(PKBFacadeReader& reader, const std::shared_ptr<EvaluationDb>& evalDb) {
    Synonym parentSyn = *std::dynamic_pointer_cast<Synonym>(parent);
    Synonym childSyn = *std::dynamic_pointer_cast<Synonym>(child);

    std::vector<Synonym> synonyms{parentSyn, childSyn};
    if (parentSyn == childSyn) {
        return {synonyms, {}};
    }

    SynonymValues parentValues{};
    SynonymValues childValues{};

    for (const Stmt& parent : reader.getStmts()) {
        if (parentSyn.getType() != DesignEntityType::STMT &&
            DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[parentSyn.getType()] != parent.type) {
            continue;
        }

        StmtNum parentStmtNum = parent.stmtNum;
        std::unordered_set<StmtNum> children = getChildren(reader, parentStmtNum);
        for (StmtNum child : children) {
            std::optional<Stmt> childStmtOpt = reader.getStatementByStmtNum(child);
            if (childSyn.getType() == DesignEntityType::STMT ||
                (childStmtOpt.has_value() &&
                 childStmtOpt.value().type == DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[childSyn.getType()])) {
                parentValues.push_back(std::to_string(parentStmtNum));
                childValues.push_back(std::to_string(child));
            }
        }
    }

    std::vector<SynonymValues> synonymValues{parentValues, childValues};

    return {synonyms, synonymValues};
}

bool BaseParent::isSimpleResult() const {
    return !parent->isSynonym() && !child->isSynonym();
}
