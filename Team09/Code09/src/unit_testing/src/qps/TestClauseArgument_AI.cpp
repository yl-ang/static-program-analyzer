#include "catch.hpp"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/ExpressionSpec.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Literal.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/Wildcard.h"

// ai-gen start(copilot, 2, e)
// prompt: Write tests for the ClauseArgument class that ensures that the relevant isXXX method returns the correct
// value
TEST_CASE("ClauseArgument isXXX methods") {
    SECTION("isExpressionSpec") {
        std::shared_ptr<ClauseArgument> expressionSpec = std::make_shared<ExpressionSpec>("x");
        REQUIRE(expressionSpec->isExpressionSpec());
        REQUIRE_FALSE(expressionSpec->isInteger());
        REQUIRE_FALSE(expressionSpec->isLiteral());
        REQUIRE_FALSE(expressionSpec->isSynonym());
        REQUIRE_FALSE(expressionSpec->isWildcard());
    }

    SECTION("isInteger") {
        std::shared_ptr<ClauseArgument> integer = std::make_shared<Integer>("4");
        REQUIRE(integer->isInteger());
        REQUIRE_FALSE(integer->isExpressionSpec());
        REQUIRE_FALSE(integer->isLiteral());
        REQUIRE_FALSE(integer->isSynonym());
        REQUIRE_FALSE(integer->isWildcard());
    }

    SECTION("isLiteral") {
        std::shared_ptr<ClauseArgument> literal = std::make_shared<Literal>("v");
        REQUIRE(literal->isLiteral());
        REQUIRE_FALSE(literal->isExpressionSpec());
        REQUIRE_FALSE(literal->isInteger());
        REQUIRE_FALSE(literal->isSynonym());
        REQUIRE_FALSE(literal->isWildcard());
    }

    SECTION("isSynonym") {
        std::shared_ptr<ClauseArgument> synonym = std::make_shared<Synonym>(DesignEntityType::STMT, "s1");
        REQUIRE(synonym->isSynonym());
        REQUIRE_FALSE(synonym->isExpressionSpec());
        REQUIRE_FALSE(synonym->isInteger());
        REQUIRE_FALSE(synonym->isLiteral());
        REQUIRE_FALSE(synonym->isWildcard());
    }

    SECTION("isWildcard") {
        std::shared_ptr<ClauseArgument> wildcard = std::make_shared<Wildcard>();
        REQUIRE(wildcard->isWildcard());
        REQUIRE_FALSE(wildcard->isExpressionSpec());
        REQUIRE_FALSE(wildcard->isInteger());
        REQUIRE_FALSE(wildcard->isLiteral());
        REQUIRE_FALSE(wildcard->isSynonym());
    }
}
// ai-gen end

// ai-gen start(copilot, 0, e)
// prompt: Write tests for the ClauseArgument class that ensures that the values are retrievable
TEST_CASE("ClauseArgument getValue methods") {
    SECTION("ExpressionSpec") {
        std::shared_ptr<ClauseArgument> expressionSpec = std::make_shared<ExpressionSpec>("x");
        REQUIRE(expressionSpec->getValue() == "x");
    }

    SECTION("Integer") {
        std::shared_ptr<ClauseArgument> integer = std::make_shared<Integer>("4");
        REQUIRE(integer->getValue() == "4");
    }

    SECTION("Literal") {
        std::shared_ptr<ClauseArgument> literal = std::make_shared<Literal>("v");
        REQUIRE(literal->getValue() == "v");
    }

    SECTION("Synonym") {
        std::shared_ptr<ClauseArgument> synonym = std::make_shared<Synonym>(DesignEntityType::STMT, "s1");
        REQUIRE(synonym->getValue() == "s1");
    }

    SECTION("Wildcard") {
        std::shared_ptr<ClauseArgument> wildcard = std::make_shared<Wildcard>();
        REQUIRE(wildcard->getValue() == "_");
    }
}
// ai-gen end

namespace {
const std::string& defaultSynonymValue = "value";
void REQUIRE_SUCCESS_VALIDATE(const DesignEntityType& det, const SynonymAttributeType& sat) {
    std::shared_ptr<Synonym> synonym = std::make_shared<Synonym>(det, defaultSynonymValue, sat);
    REQUIRE(synonym->getType() == det);

    const auto& attr = synonym->getAttr();
    REQUIRE(attr.has_value());
    REQUIRE(attr.value() == sat);
    REQUIRE(synonym->validateAttribute());
}

void REQUIRE_FAIL_VALIDATION(const DesignEntityType& det, const SynonymAttributeType& sat) {
    std::shared_ptr<Synonym> synonym = std::make_shared<Synonym>(det, defaultSynonymValue, sat);
    REQUIRE_FALSE(synonym->validateAttribute());
}
}  // namespace

/*
 *\brief Tests according to attribute values definition.
 *\see https://nus-cs3203.github.io/course-website/contents/advanced-spa-requirements/PQL/with-clauses.html
 */
TEST_CASE("Synonym With Attribute") {
    SECTION("Synonym should not have attribute") {
        std::shared_ptr<Synonym> synonym = std::make_shared<Synonym>(DesignEntityType::PROCEDURE, "p");
        REQUIRE_FALSE(synonym->getAttr().has_value());
    }

    SECTION("ProcName") {
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::PROCEDURE, SynonymAttributeType::PROCNAME);
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::CALL, SynonymAttributeType::PROCNAME);

        REQUIRE_FAIL_VALIDATION(DesignEntityType::STMT, SynonymAttributeType::PROCNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::VARIABLE, SynonymAttributeType::PROCNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::READ, SynonymAttributeType::PROCNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::PRINT, SynonymAttributeType::PROCNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::CONSTANT, SynonymAttributeType::PROCNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::WHILE, SynonymAttributeType::PROCNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::IF, SynonymAttributeType::PROCNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::ASSIGN, SynonymAttributeType::PROCNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::UNKNOWN, SynonymAttributeType::PROCNAME);
    }

    SECTION("VARNAME") {
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::VARIABLE, SynonymAttributeType::VARNAME);
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::READ, SynonymAttributeType::VARNAME);
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::PRINT, SynonymAttributeType::VARNAME);

        REQUIRE_FAIL_VALIDATION(DesignEntityType::PROCEDURE, SynonymAttributeType::VARNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::CALL, SynonymAttributeType::VARNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::STMT, SynonymAttributeType::VARNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::CONSTANT, SynonymAttributeType::VARNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::WHILE, SynonymAttributeType::VARNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::IF, SynonymAttributeType::VARNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::ASSIGN, SynonymAttributeType::VARNAME);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::UNKNOWN, SynonymAttributeType::VARNAME);
    }

    SECTION("VALUE") {
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::CONSTANT, SynonymAttributeType::VALUE);

        REQUIRE_FAIL_VALIDATION(DesignEntityType::VARIABLE, SynonymAttributeType::VALUE);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::READ, SynonymAttributeType::VALUE);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::PRINT, SynonymAttributeType::VALUE);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::PROCEDURE, SynonymAttributeType::VALUE);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::CALL, SynonymAttributeType::VALUE);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::STMT, SynonymAttributeType::VALUE);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::WHILE, SynonymAttributeType::VALUE);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::IF, SynonymAttributeType::VALUE);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::ASSIGN, SynonymAttributeType::VALUE);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::UNKNOWN, SynonymAttributeType::VALUE);
    }

    SECTION("STMTNUM") {
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::STMT, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::READ, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::PRINT, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::CALL, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::WHILE, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::IF, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_VALIDATE(DesignEntityType::ASSIGN, SynonymAttributeType::STMTNUM);

        REQUIRE_FAIL_VALIDATION(DesignEntityType::CONSTANT, SynonymAttributeType::STMTNUM);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::VARIABLE, SynonymAttributeType::STMTNUM);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::PROCEDURE, SynonymAttributeType::STMTNUM);
        REQUIRE_FAIL_VALIDATION(DesignEntityType::UNKNOWN, SynonymAttributeType::STMTNUM);
    }
}
