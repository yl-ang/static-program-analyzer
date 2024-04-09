#pragma once

#include "Exception.h"

const std::string SYNTAX_ERR_MSG = "Syntax Error";

class QPSSyntaxError : public Exception {
public:
    QPSSyntaxError() : Exception(SYNTAX_ERR_MSG) {}
};
