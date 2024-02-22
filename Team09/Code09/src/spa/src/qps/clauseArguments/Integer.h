#pragma once

#include "ClauseArgument.h"

class Integer : public ClauseArgument {
private:
    int value;

public:
    Integer(const int& v);
    int getValue() const;
    bool operator==(const Integer& other) const;
};
