#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <ctype.h> // Funções de caracteres
#include <string>

using namespace std;

enum Names
{
    UNDEF,           // 0
    ID,              // 1
    IF,              // 2
    ELSE,            // 3
    THEN,            // 4
    RELOP,           // 5
    EQ,              // 6
    NE,              // 7
    GT,              // 8
    GE,              // 9
    LT,              // 10
    LE,              // 11
    NUMBER,          // 12
    DOUBLE_LITERAL,  // 13
    FLOAT_LITERAL,   // 14
    INTEGER_LITERAL, // 15
    END_OF_FILE      // 16
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

    Token(int name, int attr)
    {
        this->name = name;
        attribute = attr;
    }
};

#endif // TOKEN_H