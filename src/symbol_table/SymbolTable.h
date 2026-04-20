#ifndef SYMBOL_TABLE_SYMBOL_TABLE_H
#define SYMBOL_TABLE_SYMBOL_TABLE_H

#include "lexer/Token.h"

#include <string>
#include <unordered_map>
#include <vector>

enum class SymbolKind {
    KEYWORD,
    IDENTIFIER
};

struct Symbol {
    std::string lexeme;
    SymbolKind kind;
    TokenType tokenType;
    int declLine;
};

class SymbolTable {
public:
    SymbolTable();

    Symbol* insert(const std::string& lexeme, TokenType tokenType, int line);
    Symbol* lookup(const std::string& lexeme);
    const Symbol* lookup(const std::string& lexeme) const;
    void print() const;

private:
    void preloadKeywords();
    Symbol* insertKeyword(const std::string& lexeme, TokenType tokenType);

    std::unordered_map<std::string, Symbol> symbols_;
    std::vector<std::string> insertionOrder_;
};

#endif
