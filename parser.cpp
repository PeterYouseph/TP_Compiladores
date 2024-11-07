// parser.cpp
#include "parser.h"

// Program → (Function)* EOF
void Parser::parseProgram()
{
    while (currentToken->name != END_OF_FILE)
    {
        parseFunction();
    }
    std::cout << "Compilation completed successfully.\n";
}

// Function → Type ID(ParamTypes){(Type VarDeclaration(...)* ;)(Statement)*}
//         | void ID(ParamTypes){(Type VarDeclaration(...)* ;)(Statement)*}
void Parser::parseFunction()
{
    if (currentToken->name == ID || currentToken->name == INT || currentToken->name == CHAR)
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
    while (currentToken->name == CHAR || currentToken->name == INT)
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

// Type → char | int
void Parser::parseType()
{
    if (currentToken->name == CHAR || currentToken->name == INT)
    {
        nextToken();
    }
    else
    {
        error("Expected 'char' or 'int' type");
    }
}

// VarDeclaration → ID ([integerconstant])?
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

// ParamTypes → void | Type ID([])? (, Type ID([])?)*
void Parser::parseParamTypes()
{
    if (currentToken->name == ID)
    {
        parseType();
        expect(ID);
    }
    else if (currentToken->name == CHAR)
    {
        nextToken(); // void
    }
    else
    {
        error("Expected parameter type or 'void'");
    }
    while (currentToken->name == VIRGULA)
    {
        nextToken();
        parseType();
        expect(ID);
    }
}

// Statement → if ( Expression ) Statement (else Statement)? | ...
void Parser::parseStatement()
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
void Parser::parseAssign()
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
void Parser::parseExpression()
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
