#pragma once

#include <regex>
#include <string>

class DesignEntity {
public:
    inline static const std::string STMT = "stmt";
    inline static const std::string READ = "read";
    inline static const std::string PRINT = "print";
    inline static const std::string CALL = "call";
    inline static const std::string WHILE = "while";
    inline static const std::string IF = "if";
    inline static const std::string ASSIGN = "assign";
    inline static const std::string VARIABLE = "variable";
    inline static const std::string CONSTANT = "constant";
    inline static const std::string PROCEDURE = "procedure";

    inline static const std::string DESIGN_ENTITIES = STMT + '|' + READ + '|' + PRINT + '|' + CALL + '|' + WHILE + '|' +
                                                      IF + '|' + ASSIGN + '|' + VARIABLE + '|' + CONSTANT + '|' + PROCEDURE;
};

bool isIdent(std::string);
bool isName(std::string);
bool isInteger(std::string);
bool isSynonym(std::string);
bool isWildcard(std::string);
bool isQuotedIdent(std::string);

bool isStmtRef(std::string);
bool isEntRef(std::string);

bool isSelectStatement(std::string);
bool isDeclarationStatement(std::string);
