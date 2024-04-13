#pragma once

#include "TableManager.h"
#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauseArguments/Synonym.h"

class ClauseResult;
using SynonymName = std::string;
using StmtNumStr = std::string;

class EvaluationDb {
public:
    PKBFacadeReader reader;
    std::shared_ptr<TableManager> tableManager;

    EvaluationDb() = default;

    EvaluationDb(const PKBFacadeReader& reader, const std::shared_ptr<TableManager>& tm)
        : reader(reader), tableManager(tm) {}

    std::unordered_set<StmtNum> getStmts(const Synonym& syn) const;
    std::unordered_set<Variable> getVariables(const Synonym& syn) const;
    std::unordered_set<Constant> getConstants(const Synonym& syn) const;
    std::unordered_set<Procedure> getProcedures(const Synonym& syn) const;
};
