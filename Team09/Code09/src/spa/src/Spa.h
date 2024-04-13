#pragma once

#include <string>
#include <utility>
#include <vector>

#include "pkb/PKB.h"
#include "pkb/PKBClient/PKBFacadeReader.h"
#include "pkb/PKBClient/PKBFacadeWriter.h"
#include "qps/QPS.h"
#include "sp/Sp.h"

class SPA {
public:
    explicit SPA() : pkbWriter(PKBFacadeWriter(pkb)), pkbReader(PKBFacadeReader(pkb)) {}

    inline void parse(std::string filename) const {
        SP sp = SP(pkbWriter);
        sp.processFile(filename);
    }

    inline std::vector<std::string> evaluate(const std::string& query) {
        QPS qps = QPS(pkbReader);
        std::vector<std::string> result = qps.processQueries(query);
        return result;
    }

    // For integration Test (SP PKB)
    inline PKB& getInternalPKB() {
        return pkb;
    }

private:
    PKB pkb;
    PKBFacadeWriter pkbWriter;
    PKBFacadeReader pkbReader;
};
