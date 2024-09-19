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
    INTERGER, //2 - [0-9]+
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
    PONTO_VIGULA, //25 - ;
    END_OF_FILE
};

class Token
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};

#endif // TOKEN_H