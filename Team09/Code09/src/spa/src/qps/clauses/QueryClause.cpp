#include "QueryClause.h"

void QueryClause::setNegation(bool n) {
    negate = n;
}

std::string QueryClause::rowValuesToString(const std::vector<Synonym>& synonyms, const Row& row) {
    std::string str{};
    for (const auto& syn : synonyms) {
        str.append(row.at(syn.getName()));
        str.append(",");
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

    auto allSynonyms = result.getSynonyms();

    // negatedResults = allRowValues - existingRows
    const std::vector<Row> existingRows = result.getAllSynonymValues();
    std::unordered_set<std::string> seen{};
    for (const Row& row : existingRows) {
        seen.insert(rowValuesToString(allSynonyms, row));
    }

    std::vector<RowValues> allRowValues{SynonymValuesRetriever::retrieve(pkb, allSynonyms, evalDb)};
    std::vector<Row> allRows{};
    for (const RowValues& row : allRowValues) {
        Row newRow{};
        for (size_t i = 0; i < allSynonyms.size(); ++i) {
            newRow[allSynonyms[i].getName()] = row[i];
        }
        allRows.push_back(newRow);
    }

    std::vector<Row> negatedResults{};
    for (const Row& row : allRows) {
        if (seen.find(rowValuesToString(allSynonyms, row)) == seen.end()) {
            negatedResults.push_back(row);
        }
    }

    return {result.getSynonyms(), negatedResults};
}
