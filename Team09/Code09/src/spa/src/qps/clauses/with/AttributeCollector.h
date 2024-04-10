#pragma once

#include "pkb/PKBClient/PKBFacadeReader.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/exceptions/clauses/QPSInvalidArgsForStmtsError.h"
#include "qps/exceptions/clauses/QPSMissingProcNameForCallError.h"

class AttributeCollector {
public:
    static std::string collect(PKBFacadeReader& pkb, const Synonym& synonym, const std::string& value);

private:
    static std::string collectCallProcName(PKBFacadeReader& pkb, std::string value);
    static std::string collectReadVarName(PKBFacadeReader& pkb, std::string value);

    static std::string collectPrintVarName(PKBFacadeReader& pkb, std::string value);
};
