#pragma once
#include "qps/exceptions/Exception.h"

const std::string NON_EXISTENT_SYNONYM_ACCESS_ERR_MSG = "Attempting to access attribute of non-existent synonym";

class QPSNonSynonymAccessError : public Exception {
public:
    QPSNonSynonymAccessError() : Exception(NON_EXISTENT_SYNONYM_ACCESS_ERR_MSG){};
};
