#include "ExpressionSpec.h"

#include "qps/QPSConstants.h"

ExpressionSpec::ExpressionSpec(const std::string& v) : value(v) {}

std::string ExpressionSpec::getValue() const {
    return value;
}

bool ExpressionSpec::isExpressionSpec() const {
    return true;
}

std::string ExpressionSpec::getClauseType() const {
    return QPSConstants::EXPR_SPEC_CLAUSE_TYPE;
}

bool ExpressionSpec::operator==(const ClauseArgument& other) const {
    if (!other.isExpressionSpec()) {
        return false;
    }
    return this->getValue() == other.getValue();
}
