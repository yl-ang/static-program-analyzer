#include "Matchable.h"

#include <queue>

#include "Ast.h"
#include "sp/SourceLoader.h"
#include "sp/tokenizer/SpTokenizer.h"

std::shared_ptr<Matchable> MatchableBuilder::fromExpressionString(const std::string& input) {
    std::stringstream ss{input};
    SourceLoader sourceLoader;
    auto inputStrings = sourceLoader.loadSimple(ss);

    SpTokenizer tokenizer{};
    std::vector<Token> tokens = tokenizer.tokenizeWithoutValidation(inputStrings);

    std::queue<Token> tokenQueue{};
    for (auto& token : tokens) {
        tokenQueue.push(token);
    }

    AST ast{};
    return ast.buildExpressionAST(tokenQueue);
}
