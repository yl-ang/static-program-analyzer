#pragma once

#include "QueryEntity.h"
#include "Table.h"
#include <unordered_set>

class PKBFacadeReader {
private:
    std::unordered_set<std::string> variables;
    std::unordered_set<std::string> constants;
    std::unordered_set<std::string> procedures;
public:
    PKBFacadeReader::PKBFacadeReader(
        std::unordered_set<std::string> vars,
        std::unordered_set<std::string> consts,
        std::unordered_set<std::string>prods
    ) : variables(vars), constants(consts), procedures(prods) {};
    std::unordered_set<std::string> getVariables() const {
        return variables;
    }
    std::unordered_set<std::string> getConstants() const {
        return constants;
    }
    std::unordered_set<std::string> getProcedures() const {
        return procedures;
    }
};


class Query {
public:
    Query(QueryEntity&); // TODO: Add suchthatclause and patternclause into arguments
    std::vector<std::string> evaluate(const PKBFacadeReader&);
private:
    QueryEntity selectEntity;

    Table buildSelectTable(const PKBFacadeReader&);
};
