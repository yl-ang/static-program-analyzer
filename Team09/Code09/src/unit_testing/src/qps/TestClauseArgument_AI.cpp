#include "catch.hpp"
#include "qps/clauseArguments/ClauseArgument.h"
#include "qps/clauseArguments/ExpressionSpec.h"
#include "qps/clauseArguments/Integer.h"
#include "qps/clauseArguments/Literal.h"
#include "qps/clauseArguments/Synonym.h"
#include "qps/clauseArguments/SynonymWithAttribute.h"
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
void REQUIRE_SUCCESS_CREATE(const DesignEntityType& det, const SynonymAttributeType& sat) {
    std::shared_ptr<Synonym> synonym = std::make_shared<SynonymWithAttribute>(det, defaultSynonymValue, sat);
    REQUIRE(synonym->getType() == det);

    const auto& attr = synonym->getAttr();
    REQUIRE(attr.has_value());
    REQUIRE(attr.value() == sat);
}

void THROW_SEMANTIC_ERROR(const DesignEntityType& det, const SynonymAttributeType& sat) {
    REQUIRE_THROWS_AS(std::make_shared<SynonymWithAttribute>(det, defaultSynonymValue, sat), QPSSemanticError);
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
        REQUIRE_SUCCESS_CREATE(DesignEntityType::PROCEDURE, SynonymAttributeType::PROCNAME);
        REQUIRE_SUCCESS_CREATE(DesignEntityType::CALL, SynonymAttributeType::PROCNAME);

        THROW_SEMANTIC_ERROR(DesignEntityType::STMT, SynonymAttributeType::PROCNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::VARIABLE, SynonymAttributeType::PROCNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::READ, SynonymAttributeType::PROCNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::PRINT, SynonymAttributeType::PROCNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::CONSTANT, SynonymAttributeType::PROCNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::WHILE, SynonymAttributeType::PROCNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::IF, SynonymAttributeType::PROCNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::ASSIGN, SynonymAttributeType::PROCNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::UNKNOWN, SynonymAttributeType::PROCNAME);
    }

    SECTION("VARNAME") {
        REQUIRE_SUCCESS_CREATE(DesignEntityType::VARIABLE, SynonymAttributeType::VARNAME);
        REQUIRE_SUCCESS_CREATE(DesignEntityType::READ, SynonymAttributeType::VARNAME);
        REQUIRE_SUCCESS_CREATE(DesignEntityType::PRINT, SynonymAttributeType::VARNAME);

        THROW_SEMANTIC_ERROR(DesignEntityType::PROCEDURE, SynonymAttributeType::VARNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::CALL, SynonymAttributeType::VARNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::STMT, SynonymAttributeType::VARNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::CONSTANT, SynonymAttributeType::VARNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::WHILE, SynonymAttributeType::VARNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::IF, SynonymAttributeType::VARNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::ASSIGN, SynonymAttributeType::VARNAME);
        THROW_SEMANTIC_ERROR(DesignEntityType::UNKNOWN, SynonymAttributeType::VARNAME);
    }

    SECTION("VALUE") {
        REQUIRE_SUCCESS_CREATE(DesignEntityType::CONSTANT, SynonymAttributeType::VALUE);

        THROW_SEMANTIC_ERROR(DesignEntityType::VARIABLE, SynonymAttributeType::VALUE);
        THROW_SEMANTIC_ERROR(DesignEntityType::READ, SynonymAttributeType::VALUE);
        THROW_SEMANTIC_ERROR(DesignEntityType::PRINT, SynonymAttributeType::VALUE);
        THROW_SEMANTIC_ERROR(DesignEntityType::PROCEDURE, SynonymAttributeType::VALUE);
        THROW_SEMANTIC_ERROR(DesignEntityType::CALL, SynonymAttributeType::VALUE);
        THROW_SEMANTIC_ERROR(DesignEntityType::STMT, SynonymAttributeType::VALUE);
        THROW_SEMANTIC_ERROR(DesignEntityType::WHILE, SynonymAttributeType::VALUE);
        THROW_SEMANTIC_ERROR(DesignEntityType::IF, SynonymAttributeType::VALUE);
        THROW_SEMANTIC_ERROR(DesignEntityType::ASSIGN, SynonymAttributeType::VALUE);
        THROW_SEMANTIC_ERROR(DesignEntityType::UNKNOWN, SynonymAttributeType::VALUE);
    }

    SECTION("STMTNUM") {
        REQUIRE_SUCCESS_CREATE(DesignEntityType::STMT, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_CREATE(DesignEntityType::READ, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_CREATE(DesignEntityType::PRINT, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_CREATE(DesignEntityType::CALL, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_CREATE(DesignEntityType::WHILE, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_CREATE(DesignEntityType::IF, SynonymAttributeType::STMTNUM);
        REQUIRE_SUCCESS_CREATE(DesignEntityType::ASSIGN, SynonymAttributeType::STMTNUM);

        THROW_SEMANTIC_ERROR(DesignEntityType::CONSTANT, SynonymAttributeType::STMTNUM);
        THROW_SEMANTIC_ERROR(DesignEntityType::VARIABLE, SynonymAttributeType::STMTNUM);
        THROW_SEMANTIC_ERROR(DesignEntityType::PROCEDURE, SynonymAttributeType::STMTNUM);
        THROW_SEMANTIC_ERROR(DesignEntityType::UNKNOWN, SynonymAttributeType::STMTNUM);
    }
}
