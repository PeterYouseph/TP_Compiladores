#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <ctype.h> // Funções de caracteres
#include <string>

using namespace std;

enum Names
{
    UNDEF, //0
    //Nomes e atributos dos tokens da linguagem
    ID,       //1 - letter (letter | digit | _ )∗
    INTEGER, //2 - [0-9]+
    CHAR,     //3 -  ‘ch’ | ‘\n’ | ‘\0’, ch é qualquer caractere ASCI exceto \ e aspas simples
    STRING,   //4 - “(ch)*”, ch é qualquer caractere ASCI exceto aspas duplas e \n 
    OP,       // Operadores
    PLUS,     //5 - +
    MINUS,    //6 - -
    MULT,     //7 - *
    DIV,      //8 - /
    RELOP,    // Operadores relacionais
    EQUAL,    //9 - =
    EQ_COM,   //10 - ==
    DIF,      //11 - !=
    LE,       //12 - <=
    LT,       //13 - <
    GE,       //14 - >=
    GT,       //15 - >
    AND,      //16 - &&
    OR,       //17 - ||
    NOT,      //18 - !
    SEP,      // Separadores
    PE,       //19 - (
    PD,       //20 - )
    BE,       // 21 - {
    BD,       //22 - }
    CE,       //23 - [
    CD,       //24 - ]
    VIRGULA,  //25 - ,
    PONTO_VIGULA, //26 - ;
    END_OF_FILE
};

class Token
{
public:
    int name;
    int attribute;
    string lexeme;

    // Construtor padrão
    Token() : name(UNDEF), attribute(UNDEF), lexeme("") {}

    // Construtor com name e lexeme (ID, CHAR, STRING)
    Token(int name, string lexeme = "")
    {
        this->name = name;
        this->attribute = UNDEF;
        this->lexeme = lexeme;
    }

    // Construtor com name e attribute (INTEGER, OP, RELOP, SEP)
    Token(int name, int attr)
    {
        this->name = name;
        this->attribute = attr;
        this->lexeme = "";
    }

    // Destrutor padrão
    ~Token() {}

    // Método que retorna uma string com as informações do token (Name, Attribute e Lexeme)
    string toString() const
    {
        return "Token(Name: " + to_string(name) + ", Attribute: " + to_string(attribute) + ", Lexeme: " + lexeme + ")";
    }
};

#endif // TOKEN_H