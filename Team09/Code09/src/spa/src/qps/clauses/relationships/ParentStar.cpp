#include "ParentStar.h"

ParentStar::ParentStar(ClauseArgument& parent, ClauseArgument& child) : parent(parent), child(child) {}

bool ParentStar::validateArguments() {
    if (parent.isSynonym()) {
        Synonym first = dynamic_cast<Synonym&>(parent);
        if (first.getType() == DesignEntityType::VARIABLE || first.getType() == DesignEntityType::CONSTANT ||
            first.getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    if (child.isSynonym()) {
        Synonym second = dynamic_cast<Synonym&>(child);
        if (second.getType() == DesignEntityType::VARIABLE || second.getType() == DesignEntityType::CONSTANT ||
            second.getType() == DesignEntityType::PROCEDURE) {
            return false;
        }
    }
    return true;
}

ClauseResult ParentStar::evaluate(PKBFacadeReader& reader) {
    if (isSimpleResult()) {
        return {reader.hasParentStarRelationship(parent, child)};
    }

    if ((parent.isInteger() && child.isSynonym()) || (parent.isSynonym() && child.isInteger())) {
        return evaluateSynonymInteger(reader);
    }

    if ((parent.isWildcard() && child.isSynonym()) || (parent.isSynonym() && child.isWildcard())) {
        return evaluateSynonymWildcard(reader);
    }

    return evaluateBothSynonyms(reader);
}

ClauseResult ParentStar::evaluateSynonymWildcard(PKBFacadeReader& reader) {
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
            // If stmt has children, then this is a parent/grandparent
            std::unordered_set<StmtNum> children = reader.getChildrenStar(stmtNum);
            if (!children.empty()) {
                uniqueValues.insert(stmtNum);
            }
        } else {
            // If stmt has parent, then this is a child/grandchild
            std::unordered_set<StmtNum> parents = reader.getParentsStar(stmtNum);
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

ClauseResult ParentStar::evaluateSynonymInteger(PKBFacadeReader& reader) {
    bool parentIsSynonym = parent.isSynonym();
    Synonym syn = parentIsSynonym ? dynamic_cast<Synonym&>(parent) : dynamic_cast<Synonym&>(child);
    Integer integer = parentIsSynonym ? dynamic_cast<Integer&>(child) : dynamic_cast<Integer&>(parent);

    StmtNum stmtNum = std::stoi(integer.getValue());

    std::unordered_set<StmtNum> stmtNums;
    if (parentIsSynonym) {
        stmtNums = reader.getParentsStar(stmtNum);
    } else {
        stmtNums = reader.getChildrenStar(stmtNum);
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

ClauseResult ParentStar::evaluateBothSynonyms(PKBFacadeReader& reader) {
    Synonym parentSyn = dynamic_cast<Synonym&>(parent);
    Synonym childSyn = dynamic_cast<Synonym&>(child);

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
        std::unordered_set<StmtNum> children = reader.getChildrenStar(parentStmtNum);
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

bool ParentStar::isSimpleResult() const {
    return !parent.isSynonym() && !child.isSynonym();
}
