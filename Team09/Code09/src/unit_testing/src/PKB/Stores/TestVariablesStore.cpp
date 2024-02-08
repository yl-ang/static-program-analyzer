#include "catch.hpp"
#include "PKB/Stores/VariablesStore.h"

// ai-gen start(gpt, 0, e)
// prompt: https://platform.openai.com/playground/p/4PkcZEhqfkWL9ACuj9lLeewI?model=gpt-4&mode=chat
TEST_CASE("VariablesStore - Base Test") {
    VariablesStore variablesStore;

    SECTION("Test setVariables and containsVariable") {
        std::unordered_set<std::string> variables = {"var1", "var2", "var3", "var4"};
        variablesStore.addEntities(variables);

        REQUIRE(variablesStore.containsVariable("var1"));
        REQUIRE(variablesStore.containsVariable("var2"));
        REQUIRE(variablesStore.containsVariable("var3"));
        REQUIRE(variablesStore.containsVariable("var4"));
        // Test for non-existent variable
        REQUIRE_FALSE(variablesStore.containsVariable("var5"));
    }

    SECTION("Test getVariablesStore") {
        std::unordered_set<std::string> variables = {"var1", "var2", "var3", "var4"};
        variablesStore.addEntities(variables);

        const std::unordered_set<std::string>& storedVariables = variablesStore.getAllEntities();

        REQUIRE(storedVariables.size() == 4);
        REQUIRE(storedVariables.find("var1") != storedVariables.end());
        REQUIRE(storedVariables.find("var2") != storedVariables.end());
        REQUIRE(storedVariables.find("var3") != storedVariables.end());
        REQUIRE(storedVariables.find("var4") != storedVariables.end());
        // Test for non-existent variable in the store
        REQUIRE(storedVariables.find("var5") == storedVariables.end());
    }
}
