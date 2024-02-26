#pragma once

#include <string>

#include "../de/AstVisitor.h"

// Visitor interface for ASTNodes
class AstNodeInterface {
public:
    virtual void accept(AstVisitor* visitor) = 0;
};
