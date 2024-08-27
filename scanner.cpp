#include "scanner.h"

// Construtor
Scanner::Scanner(string input)
{
    this->input = input;
    cout << "Entrada: " << input << endl
         << "Tamanho: "
         << input.length() << endl;
    pos = 0;
}

// Método que retorna o próximo token da entrada
Token *Scanner::nextToken()
{
    Token *tok;
    int state = 0;

    while (true)
    {
        switch (state)
        {
        case 0: // case 9: case 12: case 22:
            if (input[pos] == '\0')
            {
                tok = new Token(END_OF_FILE);
                return tok;
            }
            if (input[pos] == '<')
                state = 1;
            else if (input[pos] == '=')
                state = 5;
            else if (input[pos] == '>')
                state = 6;
            else if (isalpha(input[pos]))
                state = 10;
            else if (isdigit(input[pos]))
                state = 13;
            else if (isspace(input[pos]))
                state = 23;
            else
                lexicalError();
            // TO BE CONTINUED

            pos++;
            break;

        case 1:
            if (input[pos] == '=')
                state = 2;
            else if (input[pos] == '>')
                state = 3;
            else
                state = 4;

            pos++;

            break;

        case 2: // LE
            tok = new Token(RELOP, LE);

            return tok;

        case 3: // NE
            tok = new Token(RELOP, NE);

            return tok;

        case 4: // LT
            tok = new Token(RELOP, LT);
            pos--;
            return tok;

        case 5: // EQ
            tok = new Token(RELOP, EQ);

            return tok;

        case 6:
            if (input[pos] == '=')
                state = 7;
            else
                state = 8;

            pos++;

            break;

        case 7: // GE
            tok = new Token(RELOP, GE);

            return tok;

        case 8: // GT
            tok = new Token(RELOP, GT);

            pos--;

            return tok;

        case 10:
            if (!isalnum(input[pos]))
                state = 11;

            pos++;

            break;

        case 11:
            tok = new Token(ID);

            pos--;

            return tok;

        case 13:
            if (input[pos] == '.')
                state = 14;
            else if (input[pos] == 'E')
                state = 16;
            else if (!(isdigit(input[pos])))
                state = 20;

            pos++;

            break;

        case 14:
            if (isdigit(input[pos]))
            {
                state = 15;
                pos++;
            }
            else
                lexicalError();

            break;

        case 15:
            if (input[pos] == 'E')
                state = 16;
            else if (!(isdigit(input[pos])))
                state = 21;

            pos++;

            break;

        case 16:
            if (input[pos] == '+' || input[pos] == '-')
                state = 17;
            else if (isdigit(input[pos]))
                state = 18;
            else
                lexicalError();

            pos++;

            break;

        case 17:
            if (isdigit(input[pos]))
            {
                state = 18;
                pos++;
            }
            else
                lexicalError();

            break;

        case 18:
            if (!isdigit(input[pos]))
                state = 19;

            pos++;

            break;

        case 19:
            tok = new Token(NUMBER, DOUBLE_LITERAL);

            pos--;

            return tok;

        case 20:
            tok = new Token(NUMBER, INTEGER_LITERAL);

            pos--;

            return tok;

        case 21:
            tok = new Token(NUMBER, FLOAT_LITERAL);

            pos--;

            return tok;

        case 23:
            if (!isspace(input[pos]))
                state = 24;

            pos++;

            break;

        case 24:
            pos--;
            state = 0;

            break;

        default:
            lexicalError();
        }
    }
} // Fim nextToken

void Scanner::lexicalError()
{
    cout << "Token mal formatado\n";
    // Mostrando aviso de erro com a posição do erro e o caractere que causou o erro na cor vermelha
    cout << "Erro na posição " << pos << ": " << "\033[1;31m" << input[pos] << "\033[0m" << endl;
    exit(EXIT_FAILURE);
}

// Como compilar:
// g++ -o scanner scanner.cpp principal.cpp token.cpp
// ./scanner
// Como compilar no Ubuntu:
// g++ -o scanner scanner.cpp principal.cpp token.cpp
// ./scanner
