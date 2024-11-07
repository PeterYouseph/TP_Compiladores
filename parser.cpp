// parser.cpp
#include "parser.h"

// Analisa o programa completo
void Parser::parseProgram()
{
    while (currentToken->name != END_OF_FILE)
    {
        parseFunction();
    }
    std::cout << "Compilation completed successfully.\n";
}

//  Type ID(ParamTypes){(Type VarDeclaration(...)* ;)(Statement)*}
//         | void ID(ParamTypes){(Type VarDeclaration(...)* ;)(Statement)*}
void Parser::parseFunction() // Faz a análise da função completa
{
    if (currentToken->name == ID || currentToken->name == INTEGER || currentToken->name == CHAR)
    {
        parseType(); // Analisa o tipo da função
        expect(ID);  // Espera o nome da função
    }
    else
    {
        error("Expected function type or 'void'");
    }

    if (currentToken->lexeme == "main") // Lidar com a função principal (main)
    {
        nextToken(); // Avança para o próximo token após "main"
    }

    expect(PE);        // Espera o token '('
    parseParamTypes(); // Analisa os parâmetros da função
    expect(PD);        // Espera o token ')'

    expect(BE); // Espera o token '{'
    while (currentToken->name == CHAR || currentToken->name == INTEGER)
    {
        parseType();
        parseVarDeclaration();
        expect(PONTO_VIGULA); // ';' após a declaração da variável
    }
    while (currentToken->name != BD) // Fecha a função
    {
        parseStatement();
    }
    expect(BD); // Fecha o bloco da função
}

// Verifica se o token atual é um tipo válido
void Parser::parseType()
{
    if (currentToken->name == CHAR || currentToken->name == INTEGER)
    {
        nextToken();
    }
    else
    {
        error("Expected 'char' or 'int' type");
    }
}

// Valida a declaração de variáveis
void Parser::parseVarDeclaration()
{
    expect(ID);
    if (currentToken->name == CE)
    { // '['
        nextToken();
        expect(INTEGER); // array size
        expect(CD);      // ']'
    }
}

// Valida os tipos dos parâmetros (ParamTypes → void | Type ID ([])? (, Type ID ([])?)*)
void Parser::parseParamTypes()
{
    if (currentToken->name == ID)
    {
        parseType();
        expect(ID);
    }
    else if (currentToken->name == CHAR)
    {
        nextToken(); //  Avança para o próximo token
    }
    else
    {
        error("Expected parameter type or 'void'");
    }
    while (currentToken->name == VIRGULA) // Verifica se há mais parâmetros
    {
        nextToken();
        parseType();
        expect(ID);
    }
}

// Statement → if ( Expression ) Statement (else Statement)? | ...
void Parser::parseStatement() // Analisa as declarações de instruções (Statement)
{
    if (currentToken->name == IF)
    {
        nextToken();
        expect(PE); // '('
        parseExpression();
        expect(PD); // ')'
        parseStatement();
        if (currentToken->name == ELSE)
        {
            nextToken();
            parseStatement();
        }
    }
}

// Assign → ID ([ Expression ])? = Expression
void Parser::parseAssign() // Analisa a atribuição de variáveis
{
    expect(ID);
    if (currentToken->name == CE)
    { // '['
        nextToken();
        parseExpression();
        expect(CD); // ']'
    }
    expect(EQUAL);
    parseExpression();
}

// Expression → - Expression | ! Expression | Expression BinOp Expression | ...
void Parser::parseExpression() // Analisa as expressões
{
    if (currentToken->name == MINUS)
    {
        nextToken();
        parseExpression();
    }
    else if (currentToken->name == NOT)
    {
        nextToken();
        parseExpression();
    }
    else
    {
        // Outros casos
    }
}
