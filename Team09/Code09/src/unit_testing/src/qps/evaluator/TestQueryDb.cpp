#include "catch.hpp"
#include "qps/clauses/SuchThatClause.h"
#include "qps/evaluator/QueryDb.h"

// helper functions
static Synonym createVariable(const std::string& name) {
    return {DesignEntityType::VARIABLE, name};
}

static Synonym createProcedure(const std::string& name) {
    return {DesignEntityType::PROCEDURE, name};
}

static Synonym createConstant(const std::string& name) {
    return {DesignEntityType::CONSTANT, name};
}

TEST_CASE("Query Db") {
    auto arg_c = std::make_shared<Synonym>(createVariable("c"));
    auto arg_x = std::make_shared<Synonym>(createVariable("x"));

    auto arg_a = std::make_shared<Synonym>(createVariable("a"));

    auto const_0 = std::make_shared<Synonym>(createConstant("0"));
    auto const_1 = std::make_shared<Synonym>(createConstant("1"));

    std::shared_ptr<QueryClause> clause_c_x = std::make_shared<SuchThatClause>(RelationshipType::FOLLOWS, arg_c, arg_x);
    std::shared_ptr<QueryClause> clause_c = std::make_shared<SuchThatClause>(RelationshipType::FOLLOWS, arg_c, const_0);

    std::shared_ptr<QueryClause> clause_unconnected =
        std::make_shared<SuchThatClause>(RelationshipType::FOLLOWS, arg_a, const_1);

    QueryDb db{{clause_c_x, clause_c, clause_unconnected}};

    db.loadClausesWithEntities({*arg_c, *arg_x});
    auto firstPop = db.next();
    REQUIRE(firstPop.has_value());
    auto secondPop = db.next();
    REQUIRE(secondPop.has_value());
    auto thirdPop = db.next();
    REQUIRE_FALSE(thirdPop.has_value());

    db.loadNewGroup();
    REQUIRE(db.next().has_value());
    REQUIRE_FALSE(db.next().has_value());
}
