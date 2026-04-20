#include "lexer/Token.h"

#include <stdexcept>

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::TK_CLASS:
            return "TK_CLASS";
        case TokenType::TK_INT:
            return "TK_INT";
        case TokenType::TK_STRING:
            return "TK_STRING";
        case TokenType::TK_FLOAT:
            return "TK_FLOAT";
        case TokenType::TK_IF:
            return "TK_IF";
        case TokenType::TK_ELSE:
            return "TK_ELSE";
        case TokenType::TK_DO:
            return "TK_DO";
        case TokenType::TK_WHILE:
            return "TK_WHILE";
        case TokenType::TK_REPEAT:
            return "TK_REPEAT";
        case TokenType::TK_UNTIL:
            return "TK_UNTIL";
        case TokenType::TK_READ:
            return "TK_READ";
        case TokenType::TK_WRITE:
            return "TK_WRITE";
        case TokenType::TK_NOT:
            return "TK_NOT";
        case TokenType::TK_OR:
            return "TK_OR";
        case TokenType::TK_AND:
            return "TK_AND";
        case TokenType::TK_IDENTIFIER:
            return "TK_IDENTIFIER";
        case TokenType::TK_INTEGER_CONST:
            return "TK_INTEGER_CONST";
        case TokenType::TK_REAL_CONST:
            return "TK_REAL_CONST";
        case TokenType::TK_LITERAL:
            return "TK_LITERAL";
        case TokenType::TK_ASSIGN:
            return "TK_ASSIGN";
        case TokenType::TK_PLUS:
            return "TK_PLUS";
        case TokenType::TK_MINUS:
            return "TK_MINUS";
        case TokenType::TK_MULT:
            return "TK_MULT";
        case TokenType::TK_DIV:
            return "TK_DIV";
        case TokenType::TK_MOD:
            return "TK_MOD";
        case TokenType::TK_GT:
            return "TK_GT";
        case TokenType::TK_GTE:
            return "TK_GTE";
        case TokenType::TK_LT:
            return "TK_LT";
        case TokenType::TK_LTE:
            return "TK_LTE";
        case TokenType::TK_NEQ:
            return "TK_NEQ";
        case TokenType::TK_EQ:
            return "TK_EQ";
        case TokenType::TK_LPAREN:
            return "TK_LPAREN";
        case TokenType::TK_RPAREN:
            return "TK_RPAREN";
        case TokenType::TK_LBRACE:
            return "TK_LBRACE";
        case TokenType::TK_RBRACE:
            return "TK_RBRACE";
        case TokenType::TK_SEMICOLON:
            return "TK_SEMICOLON";
        case TokenType::TK_COMMA:
            return "TK_COMMA";
        case TokenType::TK_EOF:
            return "TK_EOF";
        case TokenType::TK_UNKNOWN:
            return "TK_UNKNOWN";
    }

    throw std::invalid_argument("TokenType inválido");
}
