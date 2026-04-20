#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <string>

enum class TokenType {
    TK_CLASS,
    TK_INT,
    TK_STRING,
    TK_FLOAT,
    TK_IF,
    TK_ELSE,
    TK_DO,
    TK_WHILE,
    TK_REPEAT,
    TK_UNTIL,
    TK_READ,
    TK_WRITE,
    TK_NOT,
    TK_OR,
    TK_AND,
    TK_IDENTIFIER,
    TK_INTEGER_CONST,
    TK_REAL_CONST,
    TK_LITERAL,
    TK_ASSIGN,
    TK_PLUS,
    TK_MINUS,
    TK_MULT,
    TK_DIV,
    TK_MOD,
    TK_GT,
    TK_GTE,
    TK_LT,
    TK_LTE,
    TK_NEQ,
    TK_EQ,
    TK_LPAREN,
    TK_RPAREN,
    TK_LBRACE,
    TK_RBRACE,
    TK_SEMICOLON,
    TK_COMMA,
    TK_EOF,
    TK_UNKNOWN
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

std::string tokenTypeToString(TokenType type);

#endif
