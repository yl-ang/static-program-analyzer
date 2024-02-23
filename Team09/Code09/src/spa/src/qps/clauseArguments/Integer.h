#pragma once

#include "ClauseArgument.h"

class Integer : public ClauseArgument {
private:
    std::string value;

public:
    Integer(const std::string& v);
    std::string getValue() const override;
    bool isInteger() const override;
    bool operator==(const ClauseArgument& other) const override;
};
