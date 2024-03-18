#include "PatternTreeNode.h"

#include <memory>
#include <sstream>

#include "queue"
#include "sp/SourceLoader.h"
#include "sp/ast/Ast.h"
#include "sp/ast/grammar_nodes/ExpressionNode.h"
#include "sp/tokenizer/SpTokenizer.h"

const char SERIALISING_DELIMITER[] = "#";

std::shared_ptr<PatternTreeNode> PatternTreeNode::buildTreeFromString(std::string value) {
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

std::shared_ptr<PatternTreeNode> PatternTreeNode::buildTreeFromAST(std::shared_ptr<ExpressionNode> node) {
    auto root = std::make_shared<PatternTreeNode>(node->value);
    if (node->left != nullptr) {
        root->left = buildTreeFromAST(node->left);
    }

    if (node->right != nullptr) {
        root->right = buildTreeFromAST(node->right);
    }
    return root;
}

std::string PatternTreeNode::serialiseToString(std::shared_ptr<PatternTreeNode> node) {
    std::ostringstream out;
    serializeHelper(node, out);
    return out.str();
}
void PatternTreeNode::serializeHelper(std::shared_ptr<PatternTreeNode> node, std::ostringstream& out) {
    if (node != nullptr) {
        out << node->value << " ";
        serializeHelper(node->left, out);
        serializeHelper(node->right, out);
    } else {
        out << SERIALISING_DELIMITER << " ";
    }
}

std::shared_ptr<PatternTreeNode> PatternTreeNode::deserializeToNode(std::string value) {
    std::istringstream in(value);
    return deserializeHelper(in);
}
std::shared_ptr<PatternTreeNode> PatternTreeNode::deserializeHelper(std::istringstream& in) {
    std::string val;
    in >> val;
    if (val == SERIALISING_DELIMITER)
        return nullptr;
    std::shared_ptr<PatternTreeNode> root = std::make_shared<PatternTreeNode>(PatternTreeNode(val));
    root->left = deserializeHelper(in);
    root->right = deserializeHelper(in);
    return root;
}
