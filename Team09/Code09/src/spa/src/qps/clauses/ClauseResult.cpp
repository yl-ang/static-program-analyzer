ClauseResult::ClauseResult(bool booleanResult) : isBoolean(true), booleanResult(booleanResult) {}

bool ClauseResult::isBoolean() const {
    return isBoolean;
}

bool ClauseResult::isTrue() const {
    return booleanResult;
}
