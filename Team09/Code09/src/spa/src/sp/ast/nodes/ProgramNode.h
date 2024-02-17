#include <string>
#include <vector>

#include "AstNode.h"
#include "sp/tokenizer/Token.h"
class ProgramNode : public ASTNode {
public:
    ProgramNode() : ASTNode("main", "program") {}
};
