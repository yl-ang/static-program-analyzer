#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <iostream>
#include <list>
#include <string>

// include your other headers here
#include "AbstractWrapper.h"
#include "PKB/PKB.h"
#include "Spa.h"
#include "qps/QPS.h"
#include "sp/Sp.h"
class TestWrapper : public AbstractWrapper {
 private:
  SP sourceProcessor;
  QPS queryProcessor;
  PKB pkb;

 public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);

  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif
