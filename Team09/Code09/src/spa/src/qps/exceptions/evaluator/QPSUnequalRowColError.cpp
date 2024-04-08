#include "QPSUnequalRowColError.h"

#include "qps/exceptions/Exception.h"

QPSUnequalRowColError::QPSUnequalRowColError() : Exception(UNEQUAL_ROWS_IN_COLS_ERR_MSG) {}
