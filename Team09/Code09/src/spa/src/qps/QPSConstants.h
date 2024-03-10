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
    inline static const std::string FOLLOWS_STAR_REGEX = "Follows\\*";
    inline static const std::string PARENT = "Parent";
    inline static const std::string PARENT_STAR = "Parent*";
    inline static const std::string PARENT_STAR_REGEX = "Parent\\*";
    inline static const std::string USES = "Uses";
    inline static const std::string MODIFIES = "Modifies";
    inline static const std::string NEXT = "Next";
    inline static const std::string NEXT_STAR = "Next*";
    inline static const std::string NEXT_STAR_REGEX = "Next\\*";

    inline static const std::string REL_REF_REGEX = FOLLOWS + '|' + FOLLOWS_STAR_REGEX + '|' + PARENT + '|' +
                                                    PARENT_STAR_REGEX + '|' + USES + '|' + MODIFIES + '|' + NEXT + '|' +
                                                    NEXT_STAR_REGEX;

    inline static const std::string SELECT = "Select";
    inline static const std::string BOOLEAN = "BOOLEAN";
    inline static const std::string SUCH_THAT = "such that";
    inline static const std::string PATTERN = "pattern";
};
