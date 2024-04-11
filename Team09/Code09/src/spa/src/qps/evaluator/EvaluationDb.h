#pragma once

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "TableManager.h"
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

    std::unordered_set<StmtNum> getStmts(const Synonym& syn);
};
