#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <unordered_map>
#include <string>

#include "symboltable.h"
#include "token.h"

class Scanner
{
private:
    std::string input; // Armazena o texto de entrada
    std::string::size_type pos;
  // Posição atual
    int line; // Linha atual
    SymbolTable* st;

public:
    // Construtor
    Scanner(std::string, SymbolTable*);

    int getLine();

    // Método que retorna o próximo token da entrada
    Token *nextToken();

    // Método para manipular erros
    void lexicalError(std::string);
};

#endif // SCANNER_H
