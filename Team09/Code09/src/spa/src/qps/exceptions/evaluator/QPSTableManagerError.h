#pragma once

#include "qps/exceptions/Exception.h"

const std::string CONVERTING_BOOLEAN_RESULT_TO_TABLE_EXCEPTION =
    "Illegal attempt at converting boolean clause result to table.";

class QPSTableManagerError : public Exception {
public:
    QPSTableManagerError();
};
