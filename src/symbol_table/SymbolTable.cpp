#include "symbol_table/SymbolTable.h"

#include <iostream>
#include <utility>

namespace {

const char* symbolKindToString(SymbolKind kind) {
    switch (kind) {
        case SymbolKind::KEYWORD:
            return "KEYWORD";
        case SymbolKind::IDENTIFIER:
            return "IDENTIFIER";
    }

    return "UNKNOWN";
}

}  // namespace

SymbolTable::SymbolTable() {
    preloadKeywords();
}

Symbol* SymbolTable::insert(const std::string& lexeme, TokenType tokenType, int line) {
    auto it = symbols_.find(lexeme);
    if (it != symbols_.end()) {
        return &it->second;
    }

    Symbol symbol{lexeme, SymbolKind::IDENTIFIER, tokenType, line};
    auto [insertedIt, inserted] = symbols_.emplace(lexeme, std::move(symbol));
    if (inserted) {
        insertionOrder_.push_back(lexeme);
    }

    return &insertedIt->second;
}

Symbol* SymbolTable::lookup(const std::string& lexeme) {
    auto it = symbols_.find(lexeme);
    if (it == symbols_.end()) {
        return nullptr;
    }

    return &it->second;
}

const Symbol* SymbolTable::lookup(const std::string& lexeme) const {
    auto it = symbols_.find(lexeme);
    if (it == symbols_.end()) {
        return nullptr;
    }

    return &it->second;
}

void SymbolTable::print() const {
    for (const std::string& lexeme : insertionOrder_) {
        const Symbol* symbol = lookup(lexeme);
        if (symbol == nullptr) {
            continue;
        }

        std::cout << "LEXEMA: " << symbol->lexeme
                  << " | CATEGORIA: " << symbolKindToString(symbol->kind)
                  << " | TOKEN: " << tokenTypeToString(symbol->tokenType)
                  << " | LINHA: ";

        if (symbol->declLine < 0) {
            std::cout << "-";
        } else {
            std::cout << symbol->declLine;
        }

        std::cout << '\n';
    }
}

void SymbolTable::preloadKeywords() {
    insertKeyword("class", TokenType::TK_CLASS);
    insertKeyword("int", TokenType::TK_INT);
    insertKeyword("string", TokenType::TK_STRING);
    insertKeyword("float", TokenType::TK_FLOAT);
    insertKeyword("if", TokenType::TK_IF);
    insertKeyword("else", TokenType::TK_ELSE);
    insertKeyword("do", TokenType::TK_DO);
    insertKeyword("while", TokenType::TK_WHILE);
    insertKeyword("repeat", TokenType::TK_REPEAT);
    insertKeyword("until", TokenType::TK_UNTIL);
    insertKeyword("read", TokenType::TK_READ);
    insertKeyword("write", TokenType::TK_WRITE);
    insertKeyword("not", TokenType::TK_NOT);
    insertKeyword("or", TokenType::TK_OR);
    insertKeyword("and", TokenType::TK_AND);
}

Symbol* SymbolTable::insertKeyword(const std::string& lexeme, TokenType tokenType) {
    auto it = symbols_.find(lexeme);
    if (it != symbols_.end()) {
        return &it->second;
    }

    Symbol symbol{lexeme, SymbolKind::KEYWORD, tokenType, -1};
    auto [insertedIt, inserted] = symbols_.emplace(lexeme, std::move(symbol));
    if (inserted) {
        insertionOrder_.push_back(lexeme);
    }

    return &insertedIt->second;
}
