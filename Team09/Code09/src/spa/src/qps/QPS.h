#include "exceptions/Exception.h"
#include "parser/PQLParser.h"
#include "Tokenizer.h"

class QPS {
private:
    Tokenizer tokenizer;
    PQLParser parser;
    PKBFacadeReader pkbReader;
public:
    std::vector<std::string> processQueries(std::string);
};
