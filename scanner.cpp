#include "scanner.h"

// Construtor
Scanner::Scanner(string input, SymbolTable *symbolTable)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl
         << "Tamanho: "
         << input.length() << endl;*/
    pos = 0;
    line = 1;
    st = symbolTable;

    // st = table;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile, line))
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else
        cout << "Unable to open file\n";

    // A próxima linha deve ser comentada posteriormente.
    // Ela é utilizada apenas para verificar se o
    // preenchimento de input foi feito corretamente.
    cout << this->input;
}

int Scanner::getLine()
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
        case 0: {
            lexeme = "";            // Inicializa o lexema como uma string vazia
            if (input[pos] == '\0') // Verifica se chegou ao final do arquivo, retornando o token de END_OF_FILE
            {
                tok = new Token(END_OF_FILE, UNDEF);
                return tok;
            }
            else if (isalpha(input[pos])) // Verifica se o caractere é uma letra ou um underline
                state = 1;
            else if (isdigit(input[pos])) // Verifica se o caractere é um dígito
                state = 2;
            else if (input[pos] == '\'') // Verifica se o caractere é uma aspas simples
                state = 3;
            else if (input[pos] == '+') // Verifica se o caractere é um operador de soma
            {
                tok = new Token(OP, PLUS, "+");
                pos++; // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '-') // Verifica se o caractere é um operador de subtração
            {
                tok = new Token(OP, MINUS, "-");
                pos++; // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '*') // Verifica se o caractere é um operador de multiplicação
            {
                tok = new Token(OP, MULT, "*");
                pos++; // Avança para o próximo caractere
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
                tok = new Token(SEP, PE, "(");
                pos++; // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == ')') // Verifica se o caractere é um parêntese direito
            {
                tok = new Token(SEP, PD, ")");
                pos++; // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '{') // Verifica se o caractere é uma chave esquerda
            {
                tok = new Token(SEP, BE, "{");
                pos++; // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '}') // Verifica se o caractere é uma chave direita
            {
                tok = new Token(SEP, BD, "}");
                pos++; // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '[') // Verifica se o caractere é um colchete esquerdo
            {
                tok = new Token(SEP, CE, "[");
                pos++; // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == ']') // Verifica se o caractere é um colchete direito
            {
                tok = new Token(SEP, CD, "]");
                pos++; // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == ',') // Verifica se o caractere é uma vírgula
            {
                tok = new Token(SEP, VIRGULA, ",");
                pos++; // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == ';') // Verifica se o caractere é um ponto e vírgula
            {
                tok = new Token(SEP, PONTO_VIGULA, ";");
                pos++; // Avança para o próximo caractere
                return tok;
            }
            else if (input[pos] == '/' && input[pos + 1] == '/') // Verifica se o caractere é uma barra dupla (comentário de linha)
            {
                while (input[pos] != '\n' && input[pos] != '\0' && pos < input.length())
                    pos++;
            }
            else if (input[pos] == '/' && input[pos + 1] == '*') // Verifica se o caractere é uma barra asterisco (comentário em bloco)
            {
                pos += 2; // Avança após '/*'
                while (!(input[pos] == '*' && input[pos + 1] == '/'))
                {
                    if (input[pos] == '\n' || input[pos] == '\0' || pos >= input.length())
                        line++; // Conta novas linhas
                    pos++;
                }
                pos += 2; // Avança após '*/'
            }
            else if (input[pos] == '/') // Verifica se o caractere é uma barra ( OP de divisão)
            {
                tok = new Token(OP, DIV, "/");
                pos++; // Avança para o próximo caractere após '/'
                return tok;
            }
            else if (isblank(input[pos]) || input[pos] == '\n') // Condição para espaços em branco e novas linhas
            {
                if (input[pos] == '\n')
                    line++; // Incrementa a linha em caso de quebra de linha
                pos++;      // Avança para o próximo caractere
                state = 0;  // Retorna ao estado inicial para buscar o próximo token
            }
            else if (pos >= input.length() || input[pos] == '\0') // Verifica se chegou ao final do arquivo
            {
                return new Token(END_OF_FILE, UNDEF);
            }
            else
            {
                pos++; // Avança para o próximo caractere se for inválido no estado atual
            }
            break;
        }

        case 1: // Identificador (ID)
        {
            lexeme += input[pos];
            pos++;
            while (isalnum(input[pos]) || input[pos] == '_')
            {
                lexeme += input[pos];
                pos++;
            }
            STEntry* reserved = st->get(lexeme);
            // Verificar se o lexema é uma palavra reservada
            if (reserved)
            {
                // Se o lexema é uma palavra reservada, retorna o token correspondente
                tok = new Token(reserved->token->name);
                // Pega o tipo de token da tabela
            }
            else
            {
                // Caso contrário, trata como um identificador (ID)
                tok = new Token(ID, UNDEF, lexeme); // Cria o token como identificador
            }
            return tok;
        }

        case 2: // Inteiro (INTEGER)
        {
            lexeme += input[pos];
            pos++;
            while (isdigit(input[pos]))
            {
                lexeme += input[pos];
                pos++;
            }
            tok = new Token(INTEGER, stoi(lexeme)); // Aqui converte o lexeme para um inteiro
            return tok;
        }

        case 3:
            pos++;       // Avança para o próximo caractere após a aspas simples
            lexeme = ""; // Limpa o lexema antes de adicionar
            if (input[pos] == '\\')
            { // Caractere de escape
                lexeme += input[pos];
                pos++;
                lexeme += input[pos];
            }
            else if (isprint(input[pos]) && input[pos] != '\'' && input[pos] != '\\')
            {
                lexeme += input[pos];
            }
            else
            {
                lexicalError("Caractere não ASCII ou inválido para CHAR");
            }
            pos++; // Move após o caractere
            if (input[pos] == '\'')
            {
                pos++;
                tok = new Token(CHAR, lexeme[0]); // Cria um novo token com o caractere - lexeme[0] é o caractere em si

                return tok;
            }
            else
            {
                lexicalError("Esperado aspas simples após caractere.");
            }
            break;

        case 4:
            if (input[pos] == '\'')
            {
                tok = new Token(CHAR, '\0'); // Cria um novo token com o caractere nulo
                pos++;                       // Avança para o próximo caractere após processar o token
                return tok;
            }
            else
            {
                msg = "Esperado aspas simples '. Recebido: " + std::string(1, input[pos + 1]);
                lexicalError(msg);
            }
            break;

        case 5:
            if (input[pos + 1] == '=')
            {
                tok = new Token(RELOP, EQ_COM, "==");
                tok->lexeme = "=="; // Adiciona lexema
                pos += 2;
            }
            else
            {
                tok = new Token(RELOP, EQUAL, "=");
                pos++;
            }
            return tok;

        case 6:
            if (pos + 1 < input.length() && input[pos + 1] == '=')
            {
                tok = new Token(RELOP, LE, "<=");
                pos += 2; // Avança após '<='
                return tok;
            }
            else
            {
                tok = new Token(RELOP, LT, "<");
                pos++; // Avança para o próximo caractere
                return tok;
            }

        case 7:
            if (pos + 1 < input.length() && input[pos + 1] == '=')
            {
                tok = new Token(RELOP, GE, ">=");
                pos += 2; // Avança após '>='
                return tok;
            }
            else
            {
                tok = new Token(RELOP, GT, ">");
                pos++; // Avança para o próximo caractere
                return tok;
            }

        case 8:
            if (pos + 1 < input.length() && input[pos + 1] == '=')
            {
                tok = new Token(RELOP, DIF, "!=");
                pos += 2; // Avança após '!='
            }
            else
            {
                tok = new Token(OP, NOT, "!");
                tok->lexeme = "!"; // Adiciona lexema
                pos++;
            }
            return tok;

        case 9:
            if (pos + 1 < input.length() && input[pos + 1] == '&')
            {
                tok = new Token(LOGOP, AND, "&&");
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
            if (pos + 1 < input.length() && input[pos + 1] == '|')
            {
                tok = new Token(LOGOP, OR, "||");
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
        default: lexicalError("unexpected case");
        }
    }
} // Fim nextToken

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
