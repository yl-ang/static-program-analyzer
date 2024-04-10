#pragma once

#include <unordered_map>

#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauseArguments/Synonym.h"

using SynonymName = std::string;
using StmtNumStr = std::string;

class EvaluationDb {
public:
    std::shared_ptr<PKBFacadeReader> reader;

    std::unordered_map<SynonymName, std::unordered_set<StmtNum>> stmtNumCache{};
    std::unordered_map<SynonymName, std::unordered_set<Variable>> variableCache{};
    std::unordered_map<SynonymName, std::unordered_set<Procedure>> procedureCache{};

    EvaluationDb() = default;
    EvaluationDb(const std::shared_ptr<PKBFacadeReader>& reader)
        : reader(reader), stmtNumCache(), variableCache(), procedureCache() {}

    std::unordered_set<StmtNum> getStmts(const Synonym& syn);
};
