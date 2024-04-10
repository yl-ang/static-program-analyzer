#pragma once

#include <unordered_map>

#include "TableManager.h"
#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauseArguments/Synonym.h"

class ClauseResult;
using SynonymName = std::string;
using StmtNumStr = std::string;

class EvaluationDb {
public:
    std::shared_ptr<PKBFacadeReader> reader;
    std::shared_ptr<TableManager> tableManager;

    EvaluationDb() = default;
    EvaluationDb(const std::shared_ptr<PKBFacadeReader>& reader, const std::shared_ptr<TableManager>& tm)
        : reader(reader), tableManager(tm) {}

    std::unordered_set<StmtNum> getStmts(const Synonym& syn);
};
