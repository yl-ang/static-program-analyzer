#pragma once

#include <string>
#include <vector>

#include "PKB/PKBClient/PKBFacadeReader.h"
#include "QpsTokenizer.h"
#include "exceptions/Exception.h"
#include "parser/PQLParser.h"

class QPS {
 private:
  QpsTokenizer tokenizer;
  PQLParser parser;
  PKBFacadeReader pkbReader;

 public:
  QPS() = default;
  explicit QPS(PKBFacadeReader&);
  std::vector<std::string> processQueries(std::string);
};
