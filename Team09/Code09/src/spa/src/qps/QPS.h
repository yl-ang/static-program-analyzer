#include "evaluator/PQLEvaluator.h"
#include "exceptions/Exception.h"
#include "parser/PQLParser.h"
#include "Tokenizer.h"

class QPS {
private:
    Tokenizer tokenizer;
    PQLParser parser;
    PQLEvaluator evaluator;
public:
    std::vector<std::string> processQueries(std::string);
};
