#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <unordered_map>
#include <string>
#include "token.h"

class Scanner
{
private:
    std::string input; // Armazena o texto de entrada
    std::unordered_map<std::string, int> reservedWords;
    int pos;  // Posição atual
    int line; // Linha atual

    void initializeReservedWords();

public:
    // Construtor
    Scanner(std::string);

    int getLine();

    // Método que retorna o próximo token da entrada
    Token *nextToken();

    // Método para manipular erros
    void lexicalError(std::string);
};

#endif // SCANNER_H
