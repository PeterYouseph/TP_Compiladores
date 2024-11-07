#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "token.h"
#include <iostream>
#include <stdexcept>

class Parser
{
private:
    Scanner *scanner;    // A instância do scanner
    Token *currentToken; // O token atual

    void nextToken()
    {
        currentToken = scanner->nextToken(); // Obtém o próximo token
    }

    void expect(int tokenType)
    {
        if (currentToken->name == tokenType)
        {
            nextToken(); // Avança para o próximo token
        }
        else
        {
            error("Unexpected token");
        }
    }

    void error(const std::string &message)
    {
        throw std::runtime_error("Syntax Error: " + message + " at line " + std::to_string(scanner->getLine()));
    }

public:
    Parser(Scanner *scanner) : scanner(scanner)
    {
        nextToken(); // Inicializa o primeiro token
    }

    void parseProgram();        // Program → (Function)* EOF - Analisa o programa completo
    void parseFunction();       // Function → Type ID(ParamTypes){(Type VarDeclaration(...)* ;)(Statement)*} - Analisa a função completa
    void parseVarDeclaration(); // VarDeclaration → ID ([integerconstant])? - Valida a declaração de variáveis
    void parseType();           // Type → char | int - Verifica se o token atual é um tipo válido
    void parseParamTypes();     // ParamTypes → void | Type ID ([])? (, Type ID ([])?)* - Valida os tipos dos parâmetros
    void parseStatement();      // Statement → if ( Expression ) Statement (else Statement)? | ... - Analisa as declarações de instruções (Statement)
    void parseAssign();         // Assign → ID ([ Expression ])? = Expression - Valida a atribuição
    void parseExpression();     // Expression → - Expression | ! Expression | Expression BinOp Expression | ... - Analisa as expressões
};

#endif // PARSER_H
