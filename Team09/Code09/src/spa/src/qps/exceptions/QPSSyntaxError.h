#pragma once

#include "Exception.h"

const std::string SYNTAX_ERR_MSG = "SyntaxError";

class QPSSyntaxError : public Exception {
public:
    QPSSyntaxError() : Exception(SYNTAX_ERR_MSG) {}
};
