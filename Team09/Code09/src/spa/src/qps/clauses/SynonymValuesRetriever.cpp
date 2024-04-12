#include "SynonymValuesRetriever.h"

#include "qps/clauseArguments/Synonym.h"

std::vector<RowValues> SynonymValuesRetriever::retrieve(PKBFacadeReader& pkb, std::vector<Synonym> synonyms,
                                                        const std::shared_ptr<EvaluationDb>& evalDb) {
    std::vector<RowValues> unjoinedSets{};

    for (Synonym synonym : synonyms) {
        RowValues synonymValues = getSynonymValues(pkb, synonym, evalDb);
        unjoinedSets.push_back(synonymValues);
    }

    std::vector<RowValues> joinedResults{cartesianProduct(unjoinedSets)};

    return joinedResults;
}

RowValues SynonymValuesRetriever::retrieve(PKBFacadeReader& pkb, Synonym synonym,
                                           const std::shared_ptr<EvaluationDb>& evalDb) {
    return getSynonymValues(pkb, synonym, evalDb);
}

ClauseResult SynonymValuesRetriever::retrieveAsClauseResult(PKBFacadeReader& pkb, std::vector<Synonym> synonyms,
                                                            const std::shared_ptr<EvaluationDb>& evalDb) {
    std::vector<RowValues> rows = retrieve(pkb, synonyms, evalDb);
    std::vector<SynonymValues> columns = ClauseEvaluatorUtils::transpose(rows);
    return {synonyms, columns};
}

std::vector<RowValues> SynonymValuesRetriever::cartesianProduct(const std::vector<RowValues>& values) {
    if (values.empty()) {
        return {{}};
    }

    std::vector<RowValues> result{};

    for (const auto& value : values[0]) {
        result.push_back({value});
    }

    for (size_t i = 1; i < values.size(); ++i) {
        std::vector<RowValues> updatedResult{};
        for (const auto& value : values[i]) {
            for (const auto& combination : result) {
                auto newCombination = combination;
                newCombination.push_back(value);
                updatedResult.push_back(newCombination);
            }
        }
        result = updatedResult;
    }

    return result;
}

std::vector<std::string> SynonymValuesRetriever::getSynonymValues(PKBFacadeReader& pkb, Synonym synonym,
                                                                  const std::shared_ptr<EvaluationDb>& evalDb) {
    std::vector<std::string> col{};

    switch (synonym.getType()) {
    case DesignEntityType::VARIABLE:
        for (std::string var : evalDb->getVariables(synonym)) {
            col.push_back(var);
        }
        break;
    case DesignEntityType::CONSTANT:
        for (std::string con : evalDb->getConstants(synonym)) {
            col.push_back(con);
        }
        break;
    case DesignEntityType::PROCEDURE:
        for (std::string prod : evalDb->getProcedures(synonym)) {
            col.push_back(prod);
        }
        break;
    case DesignEntityType::STMT:
    case DesignEntityType::READ:
    case DesignEntityType::ASSIGN:
    case DesignEntityType::CALL:
    case DesignEntityType::PRINT:
    case DesignEntityType::WHILE:
    case DesignEntityType::IF:
        for (const StmtNum& stmtNum : evalDb->getStmts(synonym)) {
            col.push_back(std::to_string(stmtNum));
        }
        break;
    case DesignEntityType::UNKNOWN:
        throw QPSUnknownDETypeError();
    }

    return col;
}
