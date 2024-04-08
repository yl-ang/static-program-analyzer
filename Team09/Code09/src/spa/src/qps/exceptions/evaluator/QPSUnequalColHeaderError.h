#include "qps/exceptions/Exception.h"

const std::string UNEQUAL_COLS_AND_HEADERS_ERR_MSG = "Number of columns and headers are not equal.";

class QPSUnequalColHeaderError : public Exception {
public:
    QPSUnequalColHeaderError();
};
