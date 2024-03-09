#include <unordered_set>

#include "../src/spa/src/sp/Sp.h"
#include "PKB/PKB.h"
#include "PKB/PKBClient/PKBFacadeReader.h"
#include "PKB/PKBClient/PKBFacadeWriter.h"
#include "catch.hpp"

using namespace std;  // NOLINT

TEST_CASE("SP-PKB integration") {
    PKB pkb;

    SECTION("CFG extracted correctly") {
        std::string fileName = "../../../src/integration_testing/src/test_files/Website_cfg_example.txt";
        PKBFacadeWriter pkbWriter = PKBFacadeWriter(pkb);
        PKBFacadeReader pkbReader = PKBFacadeReader(pkb);
        SP sp = SP(pkbWriter);
        sp.processFile(fileName);
        // validate the relationships
        std::unordered_map<int, std::vector<int>> expected = {{1, {2}},  {2, {3}},   {3, {4, 7}}, {4, {5}},
                                                              {5, {6}},  {6, {3}},   {7, {8, 9}}, {8, {10}},
                                                              {9, {10}}, {10, {11}}, {11, {12}}};
        for (const auto& [key, values] : expected) {
            REQUIRE(pkbReader.getNexter(key) == std::unordered_set<StmtNum>(values.begin(), values.end()));
        }
    }
}
