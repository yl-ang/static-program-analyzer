#include "Matchable.h"

#include <queue>

#include "Ast.h"
#include "sp/tokenizer/SpTokenizer.h"

std::shared_ptr<Matchable> MatchableBuilder::fromExpressionString(const std::vector<std::string>& input) {
    SpTokenizer tokenizer{};
    std::vector<Token> tokens = tokenizer.tokenizeWithoutValidation(input);

    std::queue<Token> tokenQueue{};
    for (auto& token : tokens) {
        tokenQueue.push(token);
    }

    AST ast{};
    return ast.buildExpressionAST(tokenQueue);
}
