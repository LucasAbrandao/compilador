#include "common/CompilerError.h"
#include "lexer/Lexer.h"
#include "lexer/Token.h"
#include "symbol_table/SymbolTable.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo.lang>\n";
        return 1;
    }

    std::ifstream input(argv[1], std::ios::binary);
    if (!input) {
        std::cerr << "Não foi possível abrir o arquivo: " << argv[1] << '\n';
        return 1;
    }

    const std::string source((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    try {
        SymbolTable symbolTable;
        Lexer lexer(source, symbolTable);

        while (true) {
            const Token token = lexer.nextToken();
            std::cout << "TOKEN: " << tokenTypeToString(token.type)
                      << " | LEXEMA: " << token.lexeme
                      << " | LINHA: " << token.line << '\n';

            if (token.type == TokenType::TK_EOF) {
                break;
            }
        }

        std::cout << "\n=== TABELA DE SÍMBOLOS ===\n";
        symbolTable.print();
    } catch (const CompilerError& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
