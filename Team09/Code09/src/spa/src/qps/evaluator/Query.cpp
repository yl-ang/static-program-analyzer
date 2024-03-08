#include "Query.h"

Query::Query(const std::vector<Synonym>& se, const std::vector<SuchThatClause>& stc,
             const std::vector<PatternClause>& pc)
    : selectEntities(se), suchThatClauses(stc), patternClauses(pc) {}

std::vector<std::string> Query::evaluate(PKBFacadeReader& pkb) {
    std::vector<QueryClause*> clauses{};
    clauses.insert(clauses.end(), suchThatClauses.begin(), suchThatClauses.end());
    clauses.insert(clauses.end(), patternClauses.begin(), patternClauses.end());

    // get truthy clauses first
    std::vector<QueryClause*> truthyClauses{};
    for (QueryClause* clause : clauses) {
        if (clause->evaluate(pkb).isBoolean() && clause->evaluate(pkb).getBoolean()) {
            truthyClauses.push_back(clause);
        }
    }

    // OLD ALGORITHM

    std::vector<ClauseResult> clauseResults{};
    for (SuchThatClause stc : suchThatClauses) {
        clauseResults.push_back(stc.evaluate(pkb));
    }

    for (PatternClause pc : patternClauses) {
        clauseResults.push_back(pc.evaluate(pkb));
    }

    if (clauseResults.empty()) {
        return buildSelectTable(pkb).extractResults(selectEntities);
    }

    std::vector<Table> clauseTables{};
    for (const ClauseResult& result : clauseResults) {
        // 1. Check if all Boolean results are true.
        if (result.isBoolean()) {
            if (result.getBoolean()) {
                continue;
            }
            // If a single false, short-circuit and return empty.
            return {};
        }

        // 2. Check if a table has common synonyms with the select clause.
        std::vector<Synonym> headers = result.getSynonyms();

        // 3. Consolidate the table
        std::vector<ColumnData> columns = result.getAllSynonymValues();
        clauseTables.push_back(Table{headers, columns});
    }

    if (clauseTables.empty()) {
        return buildSelectTable(pkb).extractResults(selectEntities);
    }

    Table result = clauseTables[0];
    for (size_t i = 1; i < clauseTables.size(); i++) {
        result = result.join(clauseTables[i]);
    }

    bool hasCommonSynonyms = false;
    for (Synonym header : result.getHeaders()) {
        if (std::find(selectEntities.begin(), selectEntities.end(), header) != selectEntities.end()) {
            hasCommonSynonyms = true;
            break;
        }
    }

    if (hasCommonSynonyms) {
        return result.extractResults(selectEntities);
    }
    if (result.isEmpty()) {
        return {};
    }
    return buildSelectTable(pkb).extractResults(selectEntities);
}

std::vector<Synonym> Query::getSelectEntities() const {
    return selectEntities;
}

Table Query::buildSelectTable(const PKBFacadeReader& pkb) {
    std::vector<Synonym> header{selectEntities};
    std::vector<ColumnData> columns{};

    for (Synonym entity : selectEntities) {
        ColumnData row{};

        switch (entity.getType()) {
        case DesignEntityType::VARIABLE:
            for (std::string var : pkb.getVariables()) {
                row.push_back(var);
            }
            break;
        case DesignEntityType::CONSTANT:
            for (std::string con : pkb.getConstants()) {
                row.push_back(con);
            }
            break;
        case DesignEntityType::PROCEDURE:
            for (std::string prod : pkb.getProcedures()) {
                row.push_back(prod);
            }
            break;
        case DesignEntityType::STMT:
            for (Stmt stmt : pkb.getStmts()) {
                row.push_back(std::to_string(stmt.stmtNum));
            }
            break;
        case DesignEntityType::READ:
        case DesignEntityType::ASSIGN:
        case DesignEntityType::CALL:
        case DesignEntityType::PRINT:
        case DesignEntityType::WHILE:
        case DesignEntityType::IF:
            for (Stmt stmt : pkb.getStatementsByType(DESIGN_ENTITY_TYPE_TO_STMT_TYPE_MAP[entity.getType()])) {
                row.push_back(std::to_string(stmt.stmtNum));
            }
            break;
        }
        columns.push_back(row);
    }

    return Table{header, columns};
}
