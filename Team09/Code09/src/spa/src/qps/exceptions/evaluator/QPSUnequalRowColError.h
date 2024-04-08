#pragma once

#include "qps/exceptions/Exception.h"

const std::string UNEQUAL_ROWS_IN_COLS_ERR_MSG = "Number of rows in columns are not equal.";

class QPSUnequalRowColError : public Exception {
public:
    QPSUnequalRowColError();
};
