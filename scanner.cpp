#include "scanner.h"

// Construtor
Scanner::Scanner(string input)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl
         << "Tamanho: "
         << input.length() << endl;*/
    pos = 0;
    line = 1;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,line) )
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n"; 

    //A próxima linha deve ser comentada posteriormente.
    //Ela é utilizada apenas para verificar se o 
    //preenchimento de input foi feito corretamente.
    cout << this->input;
}

int
Scanner::getLine()
{
    return line;
}

// Método que retorna o próximo token da entrada
Token *Scanner::nextToken()
{
    Token *tok;
    string lexeme;
    string l;
    string msg;
    int state = 0;

    while (true)
    {
        switch (state)
        {
        case 0:
            if (input[pos] == '\0') // Verifica se chegou ao final do arquivo, retornando o token de END_OF_FILE
            {
                tok = new Token(END_OF_FILE);
                return tok;
            }
            else if (isalpha(input[pos])) // Verifica se o caractere é uma letra
            {
                lexeme = std::string(1, input[pos]);
                state = 1;
            }
            else if (isdigit(input[pos])) // Verifica se o caractere é um dígito
                state = 2;
            else if (input[pos] == '\'') // Verifica se o caractere é uma aspas simples
                state = 3;
            else if (input[pos] == '+') // Verifica se o caractere é um operador de soma
            {
                tok = new Token(OP, PLUS);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '-') // Verifica se o caractere é um operador de subtração
            {
                tok = new Token(OP, MINUS);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '*') // Verifica se o caractere é um operador de multiplicação
            {
                tok = new Token(OP, MULT);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '=') // Verifica se o caractere é um operador de atribuição
                state = 5;
            else if (input[pos] == '<') // Verifica se o caractere é um operador de menor que (LT)
                state = 6;
            else if (input[pos] == '>') // Verifica se o caractere é um operador de maior que (GT)
                state = 7;
            else if (input[pos] == '!') // Verifica se o caractere é um operador de negação
                state = 8;
            else if (input[pos] == '&') // Verifica se o caractere é um operador de AND
                state = 9;
            else if (input[pos] == '|') // Verifica se o caractere é um operador de OR
                state = 10;
            else if (input[pos] == '(') // Verifica se o caractere é um parêntese esquerdo
            {
                tok = new Token(SEP, PE);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == ')') // Verifica se o caractere é um parêntese direito
            {
                tok = new Token(SEP, PD);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '{') // Verifica se o caractere é uma chave esquerda
            {
                tok = new Token(SEP, BE);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '}') // Verifica se o caractere é uma chave direita
            {
                tok = new Token(SEP, BD);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '[') // Verifica se o caractere é um colchete esquerdo
            {
                tok = new Token(SEP, CE);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == ']') // Verifica se o caractere é um colchete direito
            {
                tok = new Token(SEP, CD);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == ',') // Verifica se o caractere é uma vírgula
            {
                tok = new Token(SEP, VIRGULA);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == ';') // Verifica se o caractere é um ponto e vírgula
            {
                tok = new Token(SEP, PONTO_VIGULA);
                pos++;  // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '/' && input[pos + 1] == '/') // Verifica se o caractere é uma barra dupla (comentário de linha)
            {
                while (input[pos] != '\n')
                    pos++;
            }
            else if (input[pos] == '/' && input[pos + 1] == '*') // Verifica se o caractere é uma barra asterisco (comentário em bloco)
            {
                pos += 2; // Avança após '/*'
                while (!(input[pos] == '*' && input[pos + 1] == '/'))
                {
                    if (input[pos] == '\n')
                        line++; // Conta novas linhas
                    pos++;
                }
                pos += 2; // Avança após '*/'
            }
            else if (input[pos] == '/') // Verifica se o caractere é uma barra ( OP de divisão)
            {
                tok = new Token(OP, DIV);
                pos++;  // Avança para o próximo caractere após '/'
                return tok;
            }
            else if (isblank(input[pos]))
                state = 11;
            else
                pos++; // Avança para o próximo caractere se for inválido no estado atual
            break;

        case 1:
            if (!(isalnum(input[pos + 1])) && input[pos + 1] != '_') // Verifica se o caractere não é alfanumérico ou um underline
            {
                l = std::string(1, input[pos]);
                lexeme = lexeme + l;
                tok = new Token(ID, lexeme);
                pos++;  // Avança para o próximo caractere após processar o token
                return tok;
            }
            else // Se o caractere for alfanumérico ou underline, ele é adicionado ao lexema
            {
                l = std::string(1, input[pos]);
                lexeme = lexeme + l;
                pos++;  // Continua processando
            }
            break;

        case 2:
            if (!(isdigit(input[pos + 1]))) // Verifica se o caractere não é um dígito
            {
                tok = new Token(INTEGER);
                pos++;  // Avança para o próximo caractere após processar o token
                return tok;
            }
            pos++;  // Continua processando
            break;

        case 3:
            if (!(isprint(input[pos])) || input[pos] == '\\' || input[pos] == '\'')
            {
                msg = "Caractere recebido não é ASCII imprimível ou é \\ ou \'.";
                lexicalError(msg);
            }
            state = 4;
            pos++;
            break;

        case 4:
            if (input[pos] == '\'')
            {
                tok = new Token(CHAR);
                pos++;  // Avança para o próximo caractere após processar o token
                return tok;
            }
            else
            {
                msg = "Esperado aspas simples '. Recebido: " + std::string(1, input[pos]);
                lexicalError(msg);
            }
            break;

        case 5:
            if (input[pos + 1] == '=')
            {
                tok = new Token(RELOP, EQUAL);
                pos += 2; // Avança após '=='
                return tok;
            }
            else
            {
                tok = new Token(RELOP, EQ_COM);
                pos++;  // Avança para o próximo caractere
                return tok;
            }

        case 6:
            if (input[pos + 1] == '=')
            {
                tok = new Token(RELOP, LE);
                pos += 2; // Avança após '<='
                return tok;
            }
            else
            {
                tok = new Token(RELOP, LT);
                pos++;  // Avança para o próximo caractere
                return tok;
            }

        case 7:
            if (input[pos + 1] == '=')
            {
                tok = new Token(RELOP, GE);
                pos += 2; // Avança após '>='
                return tok;
            }
            else
            {
                tok = new Token(RELOP, GT);
                pos++;  // Avança para o próximo caractere
                return tok;
            }

        case 8:
            if (input[pos + 1] == '=')
            {
                tok = new Token(RELOP, DIF);
                pos += 2; // Avança após '!='
                return tok;
            }
            else
            {
                msg = "Esperado =. Recebido: " + std::string(1, input[pos + 1]);
                lexicalError(msg);
            }
            break;

        case 9:
            if (input[pos + 1] == '&')
            {
                tok = new Token(OP, AND);
                pos += 2; // Avança após '&&'
                return tok;
            }
            else
            {
                msg = "Esperado &. Recebido: " + std::string(1, input[pos + 1]);
                lexicalError(msg);
            }
            break;

        case 10:
            if (input[pos + 1] == '|')
            {
                tok = new Token(OP, OR);
                pos += 2; // Avança após '||'
                return tok;
            }
            else
            {
                msg = "Esperado |. Recebido: " + std::string(1, input[pos + 1]);
                lexicalError(msg);
            }
            break;

        case 11:
            if (input[pos] == '\n') // Verifica se é uma nova linha
                line++;
            pos++;
            state = 0; // Volta para o estado inicial
            break;
                //     if (input[pos] == '=')
        //         state = 2;
        //     else if (input[pos] == '>')
        //         state = 3;
        //     else
        //         state = 4;

        //     pos++;

        //     break;

        // case 2: // LE
        //     tok = new Token(RELOP, LE);

        //     return tok;

        // case 3: // NE
        //     tok = new Token(RELOP, DIF);

        //     return tok;

        // case 4: // LT
        //     tok = new Token(RELOP, LT);
        //     pos--;
        //     return tok;

        // case 5: // EQ
        //     tok = new Token(RELOP, EQUAL);

        //     return tok;

        // case 6:
        //     if (input[pos] == '=')
        //         state = 7;
        //     else
        //         state = 8;

        //     pos++;

        //     break;

        // case 7: // GE
        //     tok = new Token(RELOP, GE);

        //     return tok;

        // case 8: // GT
        //     tok = new Token(RELOP, GT);

        //     pos--;

        //     return tok;

        // case 10:
        //     if (!isalnum(input[pos]))
        //         state = 11;

        //     pos++;

        //     break;

        // case 11:
        //     tok = new Token(ID);

        //     pos--;

        //     return tok;

        // case 13:
        //     if (input[pos] == '.')
        //         state = 14;
        //     else if (input[pos] == 'E')
        //         state = 16;
        //     else if (!(isdigit(input[pos])))
        //         state = 20;

        //     pos++;

        //     break;

        // case 14:
        //     if (isdigit(input[pos]))
        //     {
        //         state = 15;
        //         pos++;
        //     }
        //     else
        //         //lexicalError();

        //     break;

        // case 15:
        //     if (input[pos] == 'E')
        //         state = 16;
        //     else if (!(isdigit(input[pos])))
        //         state = 21;

        //     pos++;

        //     break;

        // case 16:
        //     if (input[pos] == '+' || input[pos] == '-')
        //         state = 17;
        //     else if (isdigit(input[pos]))
        //         state = 18;
        //     else
        //         //lexicalError();

        //     pos++;

        //     break;

        // case 17:
        //     if (isdigit(input[pos]))
        //     {
        //         state = 18;
        //         pos++;
        //     }
        //     else
        //         //lexicalError();

        //     break;

        // case 18:
        //     if (!isdigit(input[pos]))
        //         state = 19;

        //     pos++;

        //     break;

        // case 20:
        //     tok = new Token(INTERGER);

        //     pos--;

        //     return tok;

        // case 23:
        //     if (!isspace(input[pos]))
        //         state = 24;

        //     pos++;

        //     break;

        // case 24:
        //     pos--;
        //     state = 0;

        //     break;

        // default:
        //     //lexicalError();
        }
    }
}// Fim nextToken

void Scanner::lexicalError(string msg)
{
    cout << "Token mal formatado\n";
    // Mostrando aviso de erro com a posição do erro e o caractere que causou o erro na cor vermelha
    cout << "Erro na posição " << pos << ": " << "\033[1;31m" << input[pos] << "\033[0m" << endl;
    cout << "Tipo de erro: " << msg;
    exit(EXIT_FAILURE);
}

// Como compilar:
// g++ -o scanner scanner.cpp principal.cpp token.cpp
// ./scanner
// Como compilar no Ubuntu:
// g++ -o scanner scanner.cpp principal.cpp token.cpp
// ./scanner
