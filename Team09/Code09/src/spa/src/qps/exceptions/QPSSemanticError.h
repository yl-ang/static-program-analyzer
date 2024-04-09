#pragma once

#include "Exception.h"

const std::string SEMANTIC_ERR_MSG = "Semantic Error";

class QPSSemanticError : public Exception {
public:
    QPSSemanticError() : Exception(SEMANTIC_ERR_MSG) {}
};
