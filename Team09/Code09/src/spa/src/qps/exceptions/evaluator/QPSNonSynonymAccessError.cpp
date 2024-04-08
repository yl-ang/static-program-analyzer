#include "QPSNonSynonymAccessError.h"

#include "qps/exceptions/Exception.h"

QPSNonSynonymAccessError::QPSNonSynonymAccessError() : Exception(NON_EXISTENT_SYNONYM_ACCESS_ERR_MSG) {}
