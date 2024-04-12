#include "QueryClause.h"

void QueryClause::setNegation(bool n) {
    negate = n;
}

std::string QueryClause::rowValuesToString(const RowValues& row) {
    std::string str{};
    for (const std::string& value : row) {
        str += value + ",";
    }
    return str;
}

ClauseResult QueryClause::runEvaluation(PKBFacadeReader& pkb, EvaluationDb& evalDb) {
    ClauseResult result = evaluate(pkb, evalDb);
    if (!negate) {
        return result;
    }

    if (result.isBoolean()) {
        return {!result.getBoolean()};
    }

    // negatedResults = allRowValues - existingRows
    const std::vector<RowValues> existingRows = ClauseEvaluatorUtils::transpose(result.getAllSynonymValues());
    std::unordered_set<std::string> seen{};
    for (const RowValues& row : existingRows) {
        seen.insert(rowValuesToString(row));
    }

    std::vector<RowValues> allRowValues{SynonymValuesRetriever::retrieve(pkb, result.getSynonyms(), evalDb)};
    std::vector<RowValues> negatedResults{};

    for (RowValues& row : allRowValues) {
        if (seen.find(rowValuesToString(row)) == seen.end()) {
            negatedResults.push_back(row);
        }
    }

    std::vector<SynonymValues> transposedNegatedResults{ClauseEvaluatorUtils::transpose(negatedResults)};
    return {result.getSynonyms(), transposedNegatedResults};
}
