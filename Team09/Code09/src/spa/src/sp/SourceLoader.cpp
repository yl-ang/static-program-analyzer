#include "SourceLoader.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Symbols.h"

/*
Iterate through, character by character.
When we encounter a symbol/space:
  add the current word into the result array
  if its a symbol, we determine if its a double symbol and act accordingly
  if its a space but we have a word to be added, we add the word and continue
*/

std::vector<std::string> SourceLoader::loadSimple(
    std::stringstream& stringStream) {
    std::string word = "";
    std::vector<std::string> parsedString;
    while (peekNextChar(stringStream) != EOF) {
        // ignore white spaces
        while (std::isspace(peekNextChar(stringStream))) {
            // if we currently have a word, we add it
            if (word.length()) {
                parsedString.push_back(word);
                word = "";
            }
            getNextChar(stringStream);
        }

        char currentChar = peekNextChar(stringStream);
        /* If the next character is a character symbol, add the current word to
        the result. Then we attempt to create the symbol string.
        */
        if (isCharSymbol(currentChar)) {
            if (word.length()) {
                parsedString.push_back(word);
                word = "";
            }

            std::string symbolString = createSymbolString(stringStream);
            parsedString.push_back(symbolString);
        } else if (currentChar != EOF) {
            word += getNextChar(stringStream);
        }
    }

    // if we still have something in word, add it to the parsed string
    if (word.length()) {
        parsedString.push_back(word);
    }

    return parsedString;
}

bool SourceLoader::isCharSymbol(char character) {
    return SYMBOLS.find(character) != SYMBOLS.end();
}

bool SourceLoader::isValidSymbolPair(char key, char value) {
    return SYMBOL_PAIRS.find(key) != SYMBOL_PAIRS.end() &&
           SYMBOL_PAIRS[key] == value;
}

char SourceLoader::peekNextChar(std::stringstream& stream) {
    return static_cast<char>(stream.peek());
}

char SourceLoader::getNextChar(std::stringstream& stream) {
    return static_cast<char>(stream.get());
}

/*
A symbol may have a pair (eg. <= is actually a <, = pair). We get the
first symbol first. Then we need to check whether there is a valid pair
that contains both the first symbol and the next character in the stream.
If there is a match, we can add both of them. Otherwise, we add 1 of them.
*/
std::string SourceLoader::createSymbolString(std::stringstream& stream) {
    std::string symbolString = "";
    char symbolChar = getNextChar(stream);
    symbolString += symbolChar;
    char nextChar = peekNextChar(stream);
    if (isValidSymbolPair(symbolChar, nextChar)) {
        symbolString += getNextChar(stream);
    }
    return symbolString;
}
