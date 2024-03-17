#include "PatternTreeNode.h"

#include "queue"
#include "sp/SourceLoader.h"
#include "sp/ast/Ast.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "sp/tokenizer/SpTokenizer.h"
#include "sstream"

PatternTreeNode PatternTreeNode::buildTreeFromString(std::string value) {
    SourceLoader sourceLoader;
    std::stringstream ss{value};
    std::vector<std::string> inputStrings = sourceLoader.loadSimple(ss);
    SpTokenizer tokenizer;
    std::vector<Token> tokens = tokenizer.tokenizeForPattern(inputStrings);
    // AST uses a token queue except for programs. Since we are calling that, we need to build the queue ourselves
    std::queue<Token> tokenQueue;
    for (auto token : tokens) {
        tokenQueue.push(token);
    }

    std::shared_ptr<ExpressionNode> expressionNodeAST = AST().buildExpressionAST(tokenQueue);
    return buildTreeFromAST(expressionNodeAST);
}

PatternTreeNode PatternTreeNode::buildTreeFromAST(std::shared_ptr<ExpressionNode> node) {
    PatternTreeNode root = PatternTreeNode(node->value);
    if (node->left != nullptr) {
        root.left = std::make_shared<PatternTreeNode>(buildTreeFromAST(node->left));
    }

    if (node->right != nullptr) {
        root.right = std::make_shared<PatternTreeNode>(buildTreeFromAST(node->right));
    }
    return root;
}
