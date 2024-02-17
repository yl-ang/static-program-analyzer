#pragma once

#include <string>
#include <vector>

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "QpsTokenizer.h"
#include "parser/PQLParser.h"
#include "validator/Validator.h"

class QPS {
private:
    QpsTokenizer tokenizer;
    Validator validator;
    PQLParser parser;
    PKBFacadeReader pkbReader;

public:
    explicit QPS(PKBFacadeReader&);
    QPS() = default;
    std::vector<std::string> processQueries(std::string);
};
