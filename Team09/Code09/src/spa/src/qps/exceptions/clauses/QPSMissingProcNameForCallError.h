#pragma once
#include "qps/exceptions/Exception.h"

const std::string MISSING_PROC_NAME_FOR_CALL_ERR_MSG = "No procedure name found for Call Statement";

class QPSMissingProcNameForCallError : public Exception {
public:
    QPSMissingProcNameForCallError() : Exception(MISSING_PROC_NAME_FOR_CALL_ERR_MSG){};
};
