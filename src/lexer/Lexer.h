#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include "lexer/Token.h"
#include "symbol_table/SymbolTable.h"

#include <cstddef>
#include <string>

class Lexer {
public:
    Lexer(const std::string& source, SymbolTable& symbolTable);

    Token nextToken();
    int currentLine() const;

private:
    void skipWhitespaceAndComments();
    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readLiteral();

    bool isAtEnd(std::size_t offset = 0) const;
    char peek(std::size_t offset = 0) const;
    char advance();
    bool match(char expected);
    std::string consumeUtf8CodePoint();
    [[noreturn]] void throwInvalidCharacter(const std::string& lexeme, int line);
    Token makeToken(TokenType type, const std::string& lexeme, int line) const;

    std::string source_;
    std::size_t position_;
    int line_;
    SymbolTable& symbolTable_;
};

#endif
