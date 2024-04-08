#pragma once
#include "qps/exceptions/Exception.h"

const std::string INVALID_ARGS_FOR_STMT_ERR_MSG = "None or more than 1 variable found when reading ";

class QPSMissingProcNameForStmtError : public Exception {
public:
    QPSMissingProcNameForStmtError(std::string statement) : Exception(INVALID_ARGS_FOR_STMT_ERR_MSG + statement){};
};
