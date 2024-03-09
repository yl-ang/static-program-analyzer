#pragma once

#include <string>

struct QPSConstants {
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
                                                      IF + '|' + ASSIGN + '|' + VARIABLE + '|' + CONSTANT + '|' +
                                                      PROCEDURE;

    inline static const std::string FOLLOWS = "Follows";
    inline static const std::string FOLLOWS_STAR = "Follows*";
    inline static const std::string PARENT = "Parent";
    inline static const std::string PARENT_STAR = "Parent*";
    inline static const std::string USES = "Uses";
    inline static const std::string MODIFIES = "Modifies";

    inline static const std::string REL_REF =
        FOLLOWS + '|' + FOLLOWS_STAR + '|' + PARENT + '|' + PARENT_STAR + '|' + USES + '|' + MODIFIES;

    inline static const std::string SELECT = "Select";
    inline static const std::string SUCH_THAT = "such that";
    inline static const std::string PATTERN = "pattern";
};
