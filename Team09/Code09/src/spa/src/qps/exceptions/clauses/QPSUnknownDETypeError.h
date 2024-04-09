#pragma once
#include "qps/exceptions/Exception.h"

const std::string UNKNOWN_DE_TYPE_ERR_MSG = "Unknown design entity type.";

class QPSUnknownDETypeError : public Exception {
public:
    QPSUnknownDETypeError() : Exception(UNKNOWN_DE_TYPE_ERR_MSG){};
};
