#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

using namespace std;

enum Names
{
    UNDEF   = 1000,
    // Nomes e atributos dos tokens da linguagem
    ID           = 1001,  //  letter (letter | digit | _ )∗
    INTEGER      = 1002,  //  [0-9]+
    CHAR         = 1003,  // ‘ch’ | ‘\n’ | ‘\0’, ch é qualquer caractere ASCII exceto \ e aspas simples
    STRING       = 1004,       // 4 - “(ch)*”, ch é qualquer caractere ASCII exceto aspas duplas e \n
    OP           = 1005,           // Operadores
    PLUS         = 1006,         // 5 - +
    MINUS        = 1007,        // 6 - -
    MULT         = 1008,         // 7 - *
    DIV          = 1009,          // 8 - /
    RELOP        = 1010,        // Operadores relacionais
    EQUAL        = 1011,        // 9 - =
    EQ_COM       = 1012,       // 10 - ==
    DIF          = 1013,          // 11 - !=
    LE           = 1014,           // 12 - <=
    LT           = 1015,           // 13 - <
    GE           = 1016,           // 14 - >=
    GT           = 1017,           // 15 - >
    AND          = 1018,          // 16 - &&
    OR           = 1019,           // 17 - ||
    NOT          = 1020,          // 18 - !
    SEP          = 1021,          // Separadores
    PE           = 1022,           // 19 - (
    PD           = 1023,           // 20 - )
    BE           = 1024,           // 21 - {
    BD           = 1025,           // 22 - }
    CE           = 1026,           // 23 - [
    CD           = 1027,           // 24 - ]
    VIRGULA      = 1028,      // 25 - ,
    PONTO_VIGULA = 1029, // 26 - ;
    RESERVED     = 1030,     // category of reserved words
    ELSE         = 1031,         // 30 - else
    IF           = 1032,           // 31 - if
    WHILE        = 1033,        // 32 - while
    FOR          = 1034,          // 33 - for
    CHAR_R       = 1035,       // 34 - char
    INT          = 1036,          // 35 - int
    RETURN       = 1037,       // 36 - return
    VOID         = 1038,         // 37 - void
    LOGOP        = 1039,        // 38 - operador lógico
    END_OF_FILE  = 1040
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
