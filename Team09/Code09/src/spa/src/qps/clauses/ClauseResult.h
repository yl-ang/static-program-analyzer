#pragma once

class ClauseResult {
private:
    bool isBoolean;
    bool booleanResult;

public:
    ClauseResult(bool);

    bool isBoolean() const;
    bool isTrue() const;
};
