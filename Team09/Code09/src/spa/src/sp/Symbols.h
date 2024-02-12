#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

enum SYMBOL {
  ADD = '+',
  SUB = '-',
  MUL = '*',
  DIV = '/',
  MOD = '%',
  NOT = '!',
  LESS_THAN = '<',
  GREATER_THAN = '>',
  EQUAL = '=',
  AND = '&',
  OPEN_CURLY = '{',
  CLOSE_CURLY = '}',
  OPEN_BRACKET = '(',
  CLOSE_BRACKET = ')',
  SEMICOLON = ';',
  OR = '|'
};

// this map contains a mapping of symbols that contain multiple symbols
// for example, '<=' consists of '<' and '='.
// so its entry in the map would be {'<' : '='}
std::unordered_map<char, char> SYMBOL_PAIRS = {
    {LESS_THAN, EQUAL}, {GREATER_THAN, EQUAL}, {NOT, EQUAL}, {AND, AND},
    {OR, OR},           {EQUAL, EQUAL}};

std::unordered_set<char> SYMBOLS{
    ADD,          SUB,           MUL,       DIV, MOD,        NOT,
    LESS_THAN,    GREATER_THAN,  EQUAL,     AND, OPEN_CURLY, CLOSE_CURLY,
    OPEN_BRACKET, CLOSE_BRACKET, SEMICOLON, OR};
