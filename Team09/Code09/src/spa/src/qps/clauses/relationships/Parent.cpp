#include "Parent.h"

Parent::Parent(ClauseArgument& parent, ClauseArgument& child) : parent(parent), child(child) {}

ClauseResult Parent::evaluate(PKBFacadeReader& reader) {
    if (isSimpleResult()) {
        return {reader.hasParentRelationship(parent, child)};
    }

    if ((parent.isInteger() && child.isSynonym()) || (parent.isSynonym() && child.isInteger())) {
        return evaluateSynonymInteger(reader);
    }

    if ((parent.isWildcard() && child.isSynonym()) || (parent.isSynonym() && child.isWildcard())) {
        return evaluateSynonymWildcard(reader);
    }

    return evaluateBothSynonyms(reader);
}

ClauseResult Parent::evaluateSynonymWildcard(PKBFacadeReader& reader) {
    bool parentIsSynonym = parent.isSynonym();
    Synonym syn = parentIsSynonym ? dynamic_cast<Synonym&>(parent) : dynamic_cast<Synonym&>(child);

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
            // Get parent of stmt
            std::optional<StmtNum> stmtNumOpt = reader.getParent(stmtNum);
            if (!stmtNumOpt.has_value()) {
                continue;
            }
            uniqueValues.insert(stmtNumOpt.value());
        } else {
            // Get all children of stmt
            std::unordered_set<StmtNum> children = reader.getChildren(stmtNum);
            uniqueValues.insert(children.begin(), children.end());
        }
    }

    SynonymValues values{};
    for (const int& value : uniqueValues) {
        values.push_back(std::to_string(value));
    }

    return {syn, values};
}

ClauseResult Parent::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool parentIsSynonym = parent.isSynonym();
    Synonym syn = parentIsSynonym ? dynamic_cast<Synonym&>(parent) : dynamic_cast<Synonym&>(child);
    Integer integer = parentIsSynonym ? dynamic_cast<Integer&>(child) : dynamic_cast<Integer&>(parent);

    StmtNum stmtNum = std::stoi(integer.getValue());
    if (parentIsSynonym) {
        std::optional<StmtNum> parentStmtNumOpt = reader.getParent(stmtNum);
        if (!parentStmtNumOpt.has_value()) {
            return {syn, {}};
        }
        if (syn.getType() == DesignEntityType::STMT) {
            return {syn, {std::to_string(parentStmtNumOpt.value())}};
        }

        // Since synonym is not a stmt, check if parent is of the correct type
        StatementType synonymType = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()];
        if (reader.getStatementByStmtNum(stmtNum)->type == synonymType) {
            return {syn, {std::to_string(parentStmtNumOpt.value())}};
        }
        return {syn, {}};
    }

    std::unordered_set<StmtNum> children = reader.getChildren(stmtNum);
    SynonymValues values{};
    if (syn.getType() == DesignEntityType::STMT) {
        for (StmtNum child : children) {
            values.push_back(std::to_string(child));
        }
    } else {
        StatementType synonymType = DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[syn.getType()];
        for (StmtNum child : children) {
            if (reader.getStatementByStmtNum(stmtNum)->type == synonymType) {
                values.push_back(std::to_string(child));
            }
        }
    }
    return {syn, values};
}

ClauseResult Parent::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym parentSyn = dynamic_cast<Synonym&>(parent);
    Synonym childSyn = dynamic_cast<Synonym&>(child);

    SynonymValues parentValues{};
    SynonymValues childValues{};

    for (const Stmt& child : reader.getStmts()) {
        StmtNum childStmtNum = child.stmtNum;
        std::optional<StmtNum> parentStmtNumOpt = reader.getParent(childStmtNum);
        if (!parentStmtNumOpt.has_value()) {
            continue;
        }

        parentValues.push_back(std::to_string(parentStmtNumOpt.value()));
        childValues.push_back(std::to_string(childStmtNum));
    }

    std::vector<Synonym> synonyms{parentSyn, childSyn};
    std::vector<SynonymValues> synonymValues{parentValues, childValues};

    return {synonyms, synonymValues};
}

bool Parent::isSimpleResult() const {
    return !parent.isSynonym() && !child.isSynonym();
}
