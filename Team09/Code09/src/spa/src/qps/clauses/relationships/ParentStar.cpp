#include "ParentStar.h"

ParentStar::ParentStar(ClauseArgument& parent, ClauseArgument& child) : parent(parent), child(child) {}

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
            // Get parent & grandparents of stmt
            std::unordered_set<StmtNum> parents = reader.getParentsStar(stmtNum);
            uniqueValues.insert(parents.begin(), parents.end());
        } else {
            // Get all children & grandchildren of stmt
            std::unordered_set<StmtNum> children = reader.getChildrenStar(stmtNum);
            uniqueValues.insert(children.begin(), children.end());
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

    SynonymValues parentValues{};
    SynonymValues childValues{};

    for (const Stmt& parent : reader.getStmts()) {
        StmtNum parentStmtNum = parent.stmtNum;
        std::unordered_set<StmtNum> children = reader.getChildrenStar(parentStmtNum);
        for (StmtNum child : children) {
            parentValues.push_back(std::to_string(parentStmtNum));
            childValues.push_back(std::to_string(child));
        }
    }

    std::vector<Synonym> synonyms{parentSyn, childSyn};
    std::vector<SynonymValues> synonymValues{parentValues, childValues};

    return {synonyms, synonymValues};
}

bool ParentStar::isSimpleResult() const {
    return !parent.isSynonym() && !child.isSynonym();
}
