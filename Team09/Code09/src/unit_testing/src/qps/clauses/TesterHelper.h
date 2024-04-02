#include "PKB/PKBClient/PKBFacadeReader.h"
#include "catch.hpp"
#include "qps/clauseArguments/Wildcard.h"
#include "qps/clauses/SuchThatClause.h"

class SuchThatTester {
private:  // NOLINT
    PKBFacadeReader pkb;
    std::shared_ptr<ClauseArgument> firstArg;
    std::shared_ptr<ClauseArgument> secondArg;
    ClauseResult result;

public:  // NOLINT
    SuchThatTester(PKBFacadeReader pkb, std::shared_ptr<ClauseArgument> firstArg,
                   std::shared_ptr<ClauseArgument> secondArg, RelationshipType rsType)
        : pkb{pkb},
          firstArg{firstArg},
          secondArg{secondArg},
          result{std::make_shared<SuchThatClause>(rsType, firstArg, secondArg)->evaluate(pkb)} {}

    void testBoolean(bool expected) {
        REQUIRE(result.isBoolean());
        REQUIRE(result.getBoolean() == expected);
    }

    SuchThatTester testSynonyms(std::vector<Synonym> expectedSynonyms) {
        REQUIRE(!result.isBoolean());
        REQUIRE(result.getSynonyms() == expectedSynonyms);
        return *this;
    }

    SuchThatTester testSynonymValues(std::vector<SynonymValues> expectedValues) {
        REQUIRE(!result.isBoolean());

        auto allSynonymValues = result.getAllSynonymValues();

        REQUIRE(allSynonymValues.size() == expectedValues.size());

        if (allSynonymValues.size() == 0) {
            return *this;
        }

        std::unordered_set<std::string> entries{};
        for (int i = 0; i < allSynonymValues[0].size(); i++) {
            std::string entry = "";
            for (int j = 0; j < allSynonymValues.size(); j++) {
                entry += allSynonymValues[j][i] + ",";
            }
            auto it = entries.find(entry);
            if (it == entries.end()) {
                entries.insert(entry);
            } else {
                FAIL("Duplicate entry found in synonym values: " + entry);
            }
        }

        for (int i = 0; i < expectedValues[0].size(); i++) {
            std::string entry = "";
            for (int j = 0; j < expectedValues.size(); j++) {
                entry += expectedValues[j][i] + ",";
            }
            auto it = entries.find(entry);
            if (it == entries.end()) {
                FAIL("Missing entry in synonym values: " + entry);
            }
        }

        return *this;
    }
};

class FollowsTester : public SuchThatTester {
public:
    FollowsTester(PKBFacadeReader pkb, std::shared_ptr<ClauseArgument> firstArg,
                  std::shared_ptr<ClauseArgument> secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::FOLLOWS) {}
};

class FollowsStarTester : public SuchThatTester {
public:
    FollowsStarTester(PKBFacadeReader pkb, std::shared_ptr<ClauseArgument> firstArg,
                      std::shared_ptr<ClauseArgument> secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::FOLLOWS_STAR) {}
};

class ParentTester : public SuchThatTester {
public:
    ParentTester(PKBFacadeReader pkb, std::shared_ptr<ClauseArgument> firstArg,
                 std::shared_ptr<ClauseArgument> secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::PARENT) {}
};

class ParentStarTester : public SuchThatTester {
public:
    ParentStarTester(PKBFacadeReader pkb, std::shared_ptr<ClauseArgument> firstArg,
                     std::shared_ptr<ClauseArgument> secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::PARENT_STAR) {}
};

class UsesTester : public SuchThatTester {
public:
    UsesTester(PKBFacadeReader pkb, std::shared_ptr<ClauseArgument> firstArg, std::shared_ptr<ClauseArgument> secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::USES) {}
};

class ModifiesTester : public SuchThatTester {
public:
    ModifiesTester(PKBFacadeReader pkb, std::shared_ptr<ClauseArgument> firstArg,
                   std::shared_ptr<ClauseArgument> secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::MODIFIES) {}
};

class NextTester : public SuchThatTester {
public:
    NextTester(PKBFacadeReader pkb, std::shared_ptr<ClauseArgument> firstArg, std::shared_ptr<ClauseArgument> secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::NEXT) {}
};

class NextStarTester : public SuchThatTester {
public:
    NextStarTester(PKBFacadeReader pkb, std::shared_ptr<ClauseArgument> firstArg,
                   std::shared_ptr<ClauseArgument> secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::NEXT_STAR) {}
};

class AffectsTester : public SuchThatTester {
public:
    AffectsTester(PKBFacadeReader pkb, std::shared_ptr<ClauseArgument> firstArg,
                  std::shared_ptr<ClauseArgument> secondArg)
        : SuchThatTester(pkb, firstArg, secondArg, RelationshipType::AFFECTS) {}
};
