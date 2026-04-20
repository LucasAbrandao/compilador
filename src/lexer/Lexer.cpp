#include "lexer/Lexer.h"

#include "common/CompilerError.h"

#include <cctype>

namespace {

bool isAsciiLetter(char ch) {
    const unsigned char value = static_cast<unsigned char>(ch);
    return (value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z');
}

bool isAsciiDigit(char ch) {
    const unsigned char value = static_cast<unsigned char>(ch);
    return value >= '0' && value <= '9';
}

bool isWhitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\f' || ch == '\v';
}

}  // namespace

Lexer::Lexer(const std::string& source, SymbolTable& symbolTable)
    : source_(source), position_(0), line_(1), symbolTable_(symbolTable) {}

Token Lexer::nextToken() {
    skipWhitespaceAndComments();

    if (isAtEnd()) {
        return makeToken(TokenType::TK_EOF, "EOF", line_);
    }

    const int tokenLine = line_;
    const char current = advance();

    if (isAsciiLetter(current)) {
        position_--;
        return readIdentifierOrKeyword();
    }

    if (isAsciiDigit(current)) {
        position_--;
        return readNumber();
    }

    switch (current) {
        case '"':
            position_--;
            return readLiteral();
        case '+':
            return makeToken(TokenType::TK_PLUS, "+", tokenLine);
        case '-':
            return makeToken(TokenType::TK_MINUS, "-", tokenLine);
        case '*':
            return makeToken(TokenType::TK_MULT, "*", tokenLine);
        case '%':
            return makeToken(TokenType::TK_MOD, "%", tokenLine);
        case '=':
            return makeToken(TokenType::TK_EQ, "=", tokenLine);
        case '(':
            return makeToken(TokenType::TK_LPAREN, "(", tokenLine);
        case ')':
            return makeToken(TokenType::TK_RPAREN, ")", tokenLine);
        case '{':
            return makeToken(TokenType::TK_LBRACE, "{", tokenLine);
        case '}':
            return makeToken(TokenType::TK_RBRACE, "}", tokenLine);
        case ';':
            return makeToken(TokenType::TK_SEMICOLON, ";", tokenLine);
        case ',':
            return makeToken(TokenType::TK_COMMA, ",", tokenLine);
        case '/':
            return makeToken(TokenType::TK_DIV, "/", tokenLine);
        case '<':
            if (match('=')) {
                return makeToken(TokenType::TK_LTE, "<=", tokenLine);
            }
            if (match('>')) {
                return makeToken(TokenType::TK_NEQ, "<>", tokenLine);
            }
            return makeToken(TokenType::TK_LT, "<", tokenLine);
        case '>':
            if (match('=')) {
                return makeToken(TokenType::TK_GTE, ">=", tokenLine);
            }
            return makeToken(TokenType::TK_GT, ">", tokenLine);
        case ':':
            if (match('=')) {
                return makeToken(TokenType::TK_ASSIGN, ":=", tokenLine);
            }
            throwInvalidCharacter(":", tokenLine);
        case '.':
            throwInvalidCharacter(".", tokenLine);
        default:
            if (static_cast<unsigned char>(current) > 0x7F) {
                position_--;
                throwInvalidCharacter(consumeUtf8CodePoint(), tokenLine);
            }
            throwInvalidCharacter(std::string(1, current), tokenLine);
    }
}

int Lexer::currentLine() const {
    return line_;
}

void Lexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        const char current = peek();

        if (isWhitespace(current)) {
            advance();
            continue;
        }

        if (current == '/' && !isAtEnd(1)) {
            const char next = peek(1);

            if (next == '/') {
                advance();
                advance();
                while (!isAtEnd() && peek() != '\n') {
                    advance();
                }
                continue;
            }

            if (next == '*') {
                const int commentLine = line_;
                advance();
                advance();

                while (true) {
                    if (isAtEnd()) {
                        throw CompilerError("LÉXICO", commentLine, "comentário de bloco não fechado");
                    }

                    if (peek() == '*' && !isAtEnd(1) && peek(1) == '/') {
                        advance();
                        advance();
                        break;
                    }

                    advance();
                }
                continue;
            }
        }

        break;
    }
}

Token Lexer::readIdentifierOrKeyword() {
    const int tokenLine = line_;
    const std::size_t start = position_;

    while (!isAtEnd() && (isAsciiLetter(peek()) || isAsciiDigit(peek()))) {
        advance();
    }

    const std::string lexeme = source_.substr(start, position_ - start);
    Symbol* symbol = symbolTable_.lookup(lexeme);
    if (symbol != nullptr) {
        return makeToken(symbol->tokenType, lexeme, tokenLine);
    }

    symbolTable_.insert(lexeme, TokenType::TK_IDENTIFIER, tokenLine);
    return makeToken(TokenType::TK_IDENTIFIER, lexeme, tokenLine);
}

Token Lexer::readNumber() {
    const int tokenLine = line_;
    const std::size_t start = position_;

    while (!isAtEnd() && isAsciiDigit(peek())) {
        advance();
    }

    if (!isAtEnd() && peek() == '.' && !isAtEnd(1) && isAsciiDigit(peek(1))) {
        advance();
        while (!isAtEnd() && isAsciiDigit(peek())) {
            advance();
        }
        return makeToken(TokenType::TK_REAL_CONST, source_.substr(start, position_ - start), tokenLine);
    }

    return makeToken(TokenType::TK_INTEGER_CONST, source_.substr(start, position_ - start), tokenLine);
}

Token Lexer::readLiteral() {
    const int tokenLine = line_;
    const std::size_t start = position_;

    advance();

    while (!isAtEnd()) {
        const char current = advance();
        if (current == '"') {
            return makeToken(TokenType::TK_LITERAL, source_.substr(start, position_ - start), tokenLine);
        }

        if (current == '\n') {
            throw CompilerError("LÉXICO", tokenLine, "string literal não fechada");
        }
    }

    throw CompilerError("LÉXICO", tokenLine, "string literal não fechada");
}

bool Lexer::isAtEnd(std::size_t offset) const {
    return position_ + offset >= source_.size();
}

char Lexer::peek(std::size_t offset) const {
    if (isAtEnd(offset)) {
        return '\0';
    }

    return source_[position_ + offset];
}

char Lexer::advance() {
    const char current = source_[position_++];
    if (current == '\n') {
        ++line_;
    }
    return current;
}

bool Lexer::match(char expected) {
    if (isAtEnd() || peek() != expected) {
        return false;
    }

    advance();
    return true;
}

std::string Lexer::consumeUtf8CodePoint() {
    if (isAtEnd()) {
        return "";
    }

    const unsigned char first = static_cast<unsigned char>(peek());
    std::size_t length = 1;

    if ((first & 0xE0U) == 0xC0U) {
        length = 2;
    } else if ((first & 0xF0U) == 0xE0U) {
        length = 3;
    } else if ((first & 0xF8U) == 0xF0U) {
        length = 4;
    }

    const std::size_t available = source_.size() - position_;
    if (length > available) {
        length = 1;
    }

    const std::string lexeme = source_.substr(position_, length);
    position_ += length;
    return lexeme;
}

[[noreturn]] void Lexer::throwInvalidCharacter(const std::string& lexeme, int line) {
    throw CompilerError("LÉXICO", line, "caractere inválido '" + lexeme + "'");
}

Token Lexer::makeToken(TokenType type, const std::string& lexeme, int line) const {
    return Token{type, lexeme, line};
}
