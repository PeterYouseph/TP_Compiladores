#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

using namespace std;

enum Names
{
    UNDEF, // 0
    // Nomes e atributos dos tokens da linguagem
    ID,           // 1 - letter (letter | digit | _ )∗
    INTEGER,      // 2 - [0-9]+
    CHAR,         // 3 -  ‘ch’ | ‘\n’ | ‘\0’, ch é qualquer caractere ASCII exceto \ e aspas simples
    STRING,       // 4 - “(ch)*”, ch é qualquer caractere ASCII exceto aspas duplas e \n
    OP,           // Operadores
    PLUS,         // 5 - +
    MINUS,        // 6 - -
    MULT,         // 7 - *
    DIV,          // 8 - /
    RELOP,        // Operadores relacionais
    EQUAL,        // 9 - =
    EQ_COM,       // 10 - ==
    DIF,          // 11 - !=
    LE,           // 12 - <=
    LT,           // 13 - <
    GE,           // 14 - >=
    GT,           // 15 - >
    AND,          // 16 - &&
    OR,           // 17 - ||
    NOT,          // 18 - !
    SEP,          // Separadores
    PE,           // 19 - (
    PD,           // 20 - )
    BE,           // 21 - {
    BD,           // 22 - }
    CE,           // 23 - [
    CD,           // 24 - ]
    VIRGULA,      // 25 - ,
    PONTO_VIGULA, // 26 - ;
    ELSE,         // 30 - else
    IF,           // 31 - if
    WHILE,        // 32 - while
    FOR,          // 33 - for
    INT,          // 34 - int
    END_OF_FILE
};

class Token
{
public:
    int name;
    int attribute;
    string lexeme;

    // Construtor que aceita name, attribute e lexeme
    Token(int name = UNDEF, int attr = UNDEF, string lexeme = "")
        : name(name), attribute(attr), lexeme(lexeme) {}

    // Destrutor padrão
    ~Token() {}

    // Método que retorna uma string com as informações do token (Name, Attribute e Lexeme)
    string toString() const
    {
        return "Token(Name: " + to_string(name) + ", Attribute: " + to_string(attribute) + ", Lexeme: " + lexeme + ")";
    }
};

#endif // TOKEN_H
