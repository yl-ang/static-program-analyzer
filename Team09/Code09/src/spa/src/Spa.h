#pragma once

#include <string>
#include <utility>
#include <vector>

#include "PKB/PKB.h"
#include "PKB/PKBClient/PKBFacadeReader.h"
#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "qps/QPS.h"
#include "sp/Sp.h"

class SPA {
 public:
  explicit SPA(PKB pkb)
      : pkbWriter(PKBFacadeWriter(pkb)), pkbReader(PKBFacadeReader(pkb)) {}

  inline void parse(std::string filename) const {
    SP sp = SP(pkbWriter);
    sp.processFile(filename);
  }

  inline std::vector<std::string> evaluate(const std::string& query) {
    QPS qps = QPS(pkbReader);
    std::vector<std::string> result = qps.processQueries(query);
    return result;
  }

 private:
  PKBFacadeWriter pkbWriter;
  PKBFacadeReader pkbReader;
};
