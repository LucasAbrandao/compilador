# Compilador

Implementacao de um compilador manual em C++17 para uma linguagem didatica da disciplina de Compiladores do CEFET-MG.

O projeto esta sendo desenvolvido em etapas incrementais:

- Etapa 1: analisador lexico e tabela de simbolos
- Etapa 2: analisador sintatico
- Etapa 3: analise semantica e geracao de codigo

No estado atual da branch `etapa1`, o repositorio contem a implementacao completa da Etapa 1, com:

- tipos base do compilador
- tabela de simbolos com palavras-chave e identificadores
- lexer manual com tratamento de erros lexicos
- `main.cpp` para leitura do arquivo e impressao dos tokens
- casos de teste em `tests/input`
- saidas esperadas em `tests/expected`

## Objetivo do projeto

O objetivo deste trabalho e construir um compilador completo para a linguagem definida no enunciado da disciplina, sem uso de geradores como Flex, Bison ou ANTLR. Toda a analise e implementada manualmente em C++.

Na Etapa 1, o foco e:

- ler um arquivo-fonte `.lang`
- reconhecer os tokens da linguagem
- identificar erros lexicos com linha correta
- manter uma tabela de simbolos com keywords e identificadores
- imprimir os tokens reconhecidos
- imprimir a tabela de simbolos ao fim da analise valida

## Estado atual

Status por etapa:

| Etapa | Descricao | Status |
|-------|-----------|--------|
| 1 | Lexer + tabela de simbolos | Implementada nesta branch |
| 2 | Parser | Fora do escopo atual |
| 3 | Semantico + codegen | Fora do escopo atual |

Escopo implementado nesta branch:

- `CompilerError`
- `TokenType`, `Token` e `tokenTypeToString`
- `SymbolKind`, `Symbol` e `SymbolTable`
- `Lexer`
- `main.cpp`
- suite inicial de testes da Etapa 1

Escopo ainda nao implementado:

- parser
- analise semantica
- geracao de codigo

## Estrutura do repositorio

```text
compilador/
|-- README.md
|-- Makefile
|-- .gitignore
|-- src/
|   |-- main.cpp
|   |-- common/
|   |   `-- CompilerError.h
|   |-- lexer/
|   |   |-- Token.h
|   |   |-- Token.cpp
|   |   |-- Lexer.h
|   |   `-- Lexer.cpp
|   `-- symbol_table/
|       |-- SymbolTable.h
|       `-- SymbolTable.cpp
`-- tests/
    |-- input/
    `-- expected/
```

Observacao:

- `docs/` e documentacao local de apoio e esta fora do versionamento.
- O README principal do projeto e este arquivo na raiz.

## Requisitos de ambiente

O projeto foi preparado para C++17 com:

- `g++`
- `make`
- flags `-std=c++17 -Wall -Wextra -Wpedantic`

No Windows, o caminho recomendado e usar MSYS2 com ambiente `UCRT64`.

## Como configurar no Windows com MSYS2

1. Instale o MSYS2:
   https://www.msys2.org/

2. Abra o terminal `MSYS2 UCRT64`.

3. Instale o compilador e o make:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make
```

4. Entre na pasta do projeto:

```bash
cd /c/Users/lucas/OneDrive/Documentos/vscode/compilador
```

## Como compilar

No terminal MSYS2 UCRT64:

```bash
make
```

Isso deve gerar o executavel:

```text
compiler
```

Para limpar os artefatos:

```bash
make clean
```

## Como executar

Uso geral:

```bash
./compiler <arquivo.lang>
```

Exemplos:

```bash
./compiler tests/input/teste0.lang
./compiler tests/input/teste3.lang
./compiler tests/input/teste6.lang
```

Comportamento esperado:

- se o arquivo for valido lexicalmente, o programa imprime todos os tokens ate `TK_EOF` e depois imprime a tabela de simbolos
- se houver erro lexico, o compilador imprime a mensagem de erro e encerra imediatamente com codigo diferente de zero

## Formato da saida

Cada token valido e impresso assim:

```text
TOKEN: <tipo> | LEXEMA: <valor> | LINHA: <n>
```

Ao final de uma entrada valida:

```text
=== TABELA DE SIMBOLOS ===
LEXEMA: <lexema> | CATEGORIA: <KEYWORD|IDENTIFIER> | TOKEN: <tipo> | LINHA: <n|->
```

Em caso de erro:

```text
ERRO LEXICO na linha <n>: <descricao>
```

Observacao:

- no codigo-fonte, as mensagens usam acentuacao normal
- a forma exata vista no terminal depende da codificacao configurada no ambiente

## Como testar a Etapa 1

Os casos de entrada ficam em `tests/input` e as saidas esperadas em `tests/expected`.

Casos principais:

- `teste0.lang`: programa minimo valido
- `teste1.lang` a `teste5.lang`: casos oficiais do enunciado
- `teste6.lang`: caso valido adicional com condicional e repeticao
- `teste7.lang`: microcaso para `109.`
- `teste8.lang`: microcaso para caractere Unicode invalido `–`

### Execucao manual de um caso

Exemplo com o baseline:

```bash
./compiler tests/input/teste0.lang
```

Exemplo com erro lexico:

```bash
./compiler tests/input/teste3.lang
```

### Comparando com a saida esperada

Se quiser conferir a saida gerada com o arquivo esperado:

```bash
./compiler tests/input/teste0.lang > saida.txt
diff -u saida.txt tests/expected/teste0.txt
```

Se o `diff` nao mostrar nada, a saida bate exatamente com o esperado.

### Rodando todos os testes automaticamente

Se quiser compilar e validar todos os casos de uma vez no WSL:

```bash
bash scripts/run_all_tests.sh
```

O script:

- roda `make` antes dos testes
- executa todos os arquivos `tests/input/teste*.lang`
- compara cada saida com `tests/expected`
- salva um relatorio unico em `tests/output/report.txt`

Se todos os testes passarem, o script termina com codigo `0`. Se algum falhar, ele termina com codigo diferente de `0`.

### Sequencia recomendada para entender o funcionamento

Rode nesta ordem:

```bash
./compiler tests/input/teste0.lang
./compiler tests/input/teste1.lang
./compiler tests/input/teste2.lang
./compiler tests/input/teste3.lang
./compiler tests/input/teste6.lang
./compiler tests/input/teste7.lang
./compiler tests/input/teste8.lang
```

O que observar:

- `teste0`: fluxo valido completo
- `teste1`: `1altura` vira dois tokens, nao erro lexico
- `teste2`: comentario de bloco nao fechado
- `teste3`: string literal nao fechada
- `teste6`: keywords, identificadores, comentarios e fluxo valido maior
- `teste7`: `109.` gera inteiro e depois erro no ponto
- `teste8`: travessao Unicode `–` vira erro lexico

## Visao geral do funcionamento

O fluxo da Etapa 1 e:

```text
arquivo .lang
   -> leitura completa do conteudo
   -> criacao da SymbolTable
   -> criacao do Lexer
   -> chamadas repetidas de nextToken()
   -> impressao dos tokens
   -> impressao da tabela de simbolos
```

Esse fluxo acontece em `src/main.cpp`.

### Papel do `main.cpp`

Responsabilidades principais:

- receber exatamente um argumento com o caminho do arquivo
- abrir o arquivo
- ler todo o conteudo para uma `std::string`
- instanciar `SymbolTable`
- instanciar `Lexer`
- chamar `nextToken()` em loop
- imprimir cada token, inclusive `TK_EOF`
- imprimir a tabela ao final se nao houver erro
- capturar `CompilerError` e encerrar com erro

Resumo do fluxo:

1. valida argumentos
2. abre o arquivo
3. le o conteudo inteiro
4. cria a tabela de simbolos
5. cria o lexer
6. pede tokens ate `TK_EOF`
7. se tudo der certo, imprime a tabela
8. se der erro, imprime a mensagem e para

## Tipos principais do projeto

### `CompilerError`

Representa a excecao unica do compilador nesta etapa.

Ela guarda:

- categoria do erro
- linha
- mensagem detalhada

Nesta etapa, ela e usada para erros lexicos, por exemplo:

```text
ERRO LEXICO na linha 9: string literal nao fechada
```

### `TokenType`

Enum com todos os tipos de token da linguagem, incluindo:

- keywords como `TK_CLASS`, `TK_INT`, `TK_IF`
- identificadores e constantes
- operadores e delimitadores
- `TK_EOF`
- `TK_UNKNOWN`

### `Token`

Estrutura simples com:

- `type`
- `lexeme`
- `line`

Cada chamada de `nextToken()` devolve um `Token`.

### `SymbolKind`

Distingue:

- `KEYWORD`
- `IDENTIFIER`

### `Symbol`

Representa um item da tabela de simbolos com:

- lexema
- categoria
- tipo de token
- linha de insercao

Para keywords, a linha fica `-1`, que na impressao aparece como `-`.

### `SymbolTable`

Responsavel por armazenar:

- as 15 palavras-chave reservadas
- os identificadores encontrados pela primeira vez

Internamente ela usa:

- `std::unordered_map<std::string, Symbol>` para busca rapida
- `std::vector<std::string>` para preservar a ordem de insercao

Isso permite:

- consulta rapida por lexema
- impressao na ordem correta

### `Lexer`

Responsavel por percorrer o texto-fonte caractere a caractere e gerar tokens.

Ele recebe:

- o conteudo inteiro do arquivo
- uma referencia para a `SymbolTable`

Sua API publica e minima:

- `nextToken()`
- `currentLine()`

## Funcionamento detalhado do lexer

O metodo principal do lexer e `nextToken()`.

A cada chamada, ele faz:

1. pular whitespace e comentarios
2. verificar fim de arquivo
3. analisar o proximo caractere relevante
4. decidir qual rotina de leitura usar
5. retornar um token ou lancar erro

### Controle de posicao e linha

O lexer mantem:

- `position_`: indice atual no texto
- `line_`: linha atual

Sempre que `advance()` consome `\n`, a linha e incrementada. Isso vale inclusive dentro de comentarios de bloco.

### Whitespace e comentarios

Antes de reconhecer qualquer token, o lexer chama `skipWhitespaceAndComments()`.

Ela ignora:

- espacos
- tabulacoes
- quebras de linha
- comentario de linha `// ...`
- comentario de bloco `/* ... */`

Se um comentario de bloco chega ao fim do arquivo sem `*/`, o lexer lanca erro lexico e para.

### Identificadores e keywords

Se o proximo caractere e letra:

- o lexer continua lendo enquanto houver letra ou digito
- forma o lexema completo
- consulta a tabela de simbolos

Se o lexema ja estiver na tabela:

- pode ser keyword
- pode ser identificador ja conhecido

Se nao estiver:

- o lexer insere como `IDENTIFIER`
- devolve `TK_IDENTIFIER`

Exemplos:

- `class` -> `TK_CLASS`
- `altura` -> `TK_IDENTIFIER`
- `Altura` -> `TK_IDENTIFIER`, diferente de `altura`

### Numeros inteiros e reais

Se o proximo caractere e digito:

- o lexer le todos os digitos do inteiro
- depois verifica se existe um `.` seguido de digito

Se existir:

- continua lendo a parte decimal
- devolve `TK_REAL_CONST`

Se nao existir:

- devolve `TK_INTEGER_CONST`

Exemplos:

- `10` -> inteiro
- `2.0` -> real
- `109.` -> primeiro sai `109` como inteiro; na proxima chamada o `.` vira erro

### Maximal munch

A tokenizacao segue o principio de maximal munch: o lexer sempre tenta consumir o maior token valido possivel a partir da posicao atual.

Um caso importante:

```text
1altura
```

O comportamento e:

- `1` vira `TK_INTEGER_CONST`
- `altura` vira `TK_IDENTIFIER`

Isso nao e erro lexico. O problema, se houver, sera tratado pelo parser na Etapa 2.

### Literais string

Se o caractere atual e aspas dupla:

- o lexer le ate a aspas de fechamento
- se encontrar `\n` antes de fechar, gera erro
- se chegar ao EOF antes de fechar, gera erro

Exemplo de erro:

```text
"Digite mais um numero:
```

### Operadores e delimitadores

O lexer reconhece:

- `+`, `-`, `*`, `/`, `%`
- `=`
- `(`, `)`, `{`, `}`, `;`, `,`
- `<`, `<=`, `<>`
- `>`, `>=`
- `:=`

Casos especiais:

- `:` sozinho e erro
- `.` sozinho e erro

### Caracteres invalidos

Qualquer caractere fora do alfabeto lexico permitido gera erro imediatamente.

Isso inclui:

- simbolos nao previstos na especificacao
- caracteres Unicode estranhos, como o travessao `–`

O lexer encerra no primeiro erro encontrado.

## Como a tabela de simbolos funciona

Quando a `SymbolTable` e criada:

- ela ja carrega as 15 keywords da linguagem
- todas entram primeiro na ordem de impressao

Depois, durante a analise lexica:

- cada identificador novo e inserido na primeira ocorrencia
- identificadores repetidos nao sao inseridos novamente

Por isso a impressao final sempre mostra:

1. as keywords primeiro
2. os identificadores depois, na ordem em que apareceram pela primeira vez

Exemplo conceitual:

```text
class Teste {
  int x;
  {
    x := 1;
    write(x);
  }
}
```

Na tabela:

- `class`, `int`, `write` ja existem como keywords
- `Teste` entra como identificador
- `x` entra como identificador
- o segundo uso de `x` nao cria nova entrada

## Walkthrough guiado com `teste0.lang`

Arquivo:

```text
class Baseline {
  int x;
  {
    x := 1;
    write(x);
  }
}
```

Passo a passo:

1. `class`
   - comeca com letra
   - o lexer le a palavra toda
   - consulta a tabela
   - encontra keyword
   - retorna `TK_CLASS`

2. `Baseline`
   - comeca com letra
   - nao existe na tabela
   - entra como identificador
   - retorna `TK_IDENTIFIER`

3. `{`
   - reconhecido diretamente como `TK_LBRACE`

4. `int`
   - encontrado como keyword
   - retorna `TK_INT`

5. `x`
   - nao existe ainda
   - entra na tabela
   - retorna `TK_IDENTIFIER`

6. `;`
   - retorna `TK_SEMICOLON`

7. `{`
   - retorna `TK_LBRACE`

8. `x := 1;`
   - `x` retorna como identificador ja conhecido
   - `:=` retorna `TK_ASSIGN`
   - `1` retorna `TK_INTEGER_CONST`
   - `;` retorna `TK_SEMICOLON`

9. `write(x);`
   - `write` ja existe como keyword
   - `(` e `)` viram parenteses
   - `x` volta como identificador
   - `;` fecha o comando

10. `}`
    - fecha o bloco interno

11. `}`
    - fecha o programa

12. EOF
    - o lexer retorna `TK_EOF`

Depois disso, o programa imprime a tabela de simbolos com as keywords e, ao final, `Baseline` e `x`.

## Casos de erro didaticos

### `teste2.lang`

Mostra comentario de bloco nao fechado:

- o lexer entra em `/*`
- percorre o texto ate o fim
- nao encontra `*/`
- lanca erro lexico

### `teste3.lang`

Mostra string literal nao fechada:

- o lexer encontra `"`
- continua lendo a string
- encontra quebra de linha antes do fechamento
- lanca erro

### `teste7.lang`

Mostra o comportamento de `109.`:

- `109` e reconhecido como inteiro
- o `.` nao forma real valido porque nao ha digito depois
- o ponto fica para a proxima leitura
- o lexer acusa `caractere invalido '.'`

### `teste8.lang`

Mostra erro com travessao Unicode:

- o caractere `–` nao faz parte do alfabeto lexico permitido
- o lexer encerra com erro lexico no primeiro ponto invalido

## Relacao entre codigo e comportamento

Se voce quiser estudar o codigo por ordem de leitura, use esta sequencia:

1. `src/main.cpp`
2. `src/lexer/Token.h`
3. `src/lexer/Token.cpp`
4. `src/symbol_table/SymbolTable.h`
5. `src/symbol_table/SymbolTable.cpp`
6. `src/common/CompilerError.h`
7. `src/lexer/Lexer.h`
8. `src/lexer/Lexer.cpp`

Motivo:

- `main.cpp` mostra o fluxo geral
- `Token` mostra o contrato basico dos resultados do lexer
- `SymbolTable` mostra como a linguagem diferencia keywords e identificadores
- `CompilerError` mostra o formato dos erros
- `Lexer.cpp` concentra a logica principal

## Proximos passos

Depois da Etapa 1, a evolucao natural do projeto e:

1. implementar o parser por descida recursiva
2. validar a estrutura sintatica da linguagem
3. adicionar analise semantica
4. gerar codigo para o alvo definido

Essas partes ainda nao fazem parte desta branch.

## Resumo final

Hoje, esta branch ja permite:

- ler programas `.lang`
- reconhecer tokens da linguagem
- montar a tabela de simbolos
- detectar erros lexicos com linha
- validar a Etapa 1 com casos de teste reproduziveis

Se o objetivo for entender o projeto rapidamente:

1. compile com `make`
2. rode `teste0.lang`
3. rode `teste3.lang`
4. compare a saida com `tests/expected`
5. leia `Lexer.cpp` com o README ao lado
