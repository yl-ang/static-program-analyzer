#pragma once

#include <string>
#include <vector>

#include "QpsTokenizer.h"
#include "parser/PQLParser.h"
#include "pkb/PKBClient/PKBFacadeReader.h"
#include "validator/Validator.h"

class QPS {
private:
    QpsTokenizer tokenizer;
    PQLParser parser;
    PKBFacadeReader pkbReader;

public:
    explicit QPS(PKBFacadeReader&);
    QPS() = default;
    std::vector<std::string> processQueries(std::string);
};
