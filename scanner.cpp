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
            if (input[pos] == '\0')
            {
                tok = new Token(END_OF_FILE);
                return tok;
            }
            else if (isalpha(input[pos])){
                lexeme = std::string(1, input[pos]);
                state = 1;
            }
            else if (isdigit(input[pos]))
                state = 2;
            else if (input[pos] == '\'') //verificar se ta certo
                state = 3;
            else if (input[pos] == '+'){
                tok = new Token(OP, PLUS);
                return tok;
            }
            else if (input[pos] == '-'){
                tok = new Token(OP, MINUS);
                return tok;
            }
            else if (input[pos] == '*'){
                tok = new Token(OP, MULT);
                return tok;
            }
            else if (input[pos] == '=')
                state = 5;
            else if (input[pos] == '<')
                state = 6;
            else if (input[pos] == '>')
                state = 7;
            else if (input[pos] == '!')
                state = 8;
            else if (input[pos] == '&')
                state = 9;
            else if (input[pos] == '|')
                state = 10;
            else if (input[pos] == '('){
                tok = new Token(SEP, PE);
                return tok;
            }
            else if (input[pos] == ')'){
                tok = new Token(SEP, PD);
                return tok;
            }
            else if (input[pos] == '{'){
                tok = new Token(SEP, BE);
                return tok;
            }
            else if (input[pos] == '}'){
                tok = new Token(SEP, BD);
                return tok;
            }
            else if (input[pos] == '['){
                tok = new Token(SEP, CE);
                return tok;
            }
            else if (input[pos] == ']'){
                tok = new Token(SEP, CD);
                return tok;
            }
            else if (input[pos] == ','){
                tok = new Token(SEP, VIRGULA);
                return tok;
            }
            else if (input[pos] == ';'){
                tok = new Token(SEP, PONTO_VIGULA);
                return tok;
            }
            else if (isblank(input[pos]))
                state = 11;
            else
                //lexicalError();
            // TO BE CONTINUED

            pos++;
            break;

        case 1:
            if (!(isalnum(input[pos+1]))){
                l = std::string(1, input[pos]);
                lexeme = lexeme + l;
                tok = new Token(ID, lexeme);
                return tok;
            }
            else{
                l = std::string(1, input[pos]);
                lexeme = lexeme + l;
            }
            pos++;
            break;

        case 2:
            if (!(isdigit(input[pos+1]))){
                tok = new Token(INTERGER);
                return tok;
            }
            pos++;
            break;
        
        case 3:
            if (!(isprint(input[pos])) || input[pos] == '\\' || input[pos] == '\''){
                msg = "Character recebido não é ASCII imprimível ou é \\ ou \'.";
                lexicalError(msg);
            }
            state = 4;
            pos++;
            break;
        
        case 4:
            if (input[pos] == '\''){
                tok = new Token(CHAR);
                return tok;
            }
            else{
                msg = "Esperado aspas simples '. Recebeu: " + std::string(1, input[pos]);
                lexicalError(msg);
            }
        
        case 5:
            if (input[pos] == '='){
                tok = new Token(RELOP, EQUAL);
                return tok;
            }
            else{
                tok = new Token(RELOP, EQ_COM);
                // pos--; VERIFICAR SE PRECISA
                return tok;
            }

        case 6:
            if (input[pos] == '='){
                tok = new Token(RELOP, LE);
                return tok;
            }
            else{
                tok = new Token(RELOP, LT);
                // pos--; VERIFICAR SE PRECISA
                return tok;
            }

        case 7:
            if (input[pos] == '='){
                tok = new Token(RELOP, GE);
                return tok;
            }
            else{
                tok = new Token(RELOP, GT);
                // pos--; VERIFICAR SE PRECISA
                return tok;
            }

        case 8:
            if (input[pos] == '='){
                tok = new Token(RELOP, DIF);
                return tok;
            }
            else{
                tok = new Token(RELOP, NOT);
                // pos--; VERIFICAR SE PRECISA
                return tok;
            }
        
        case 9:
            if (input[pos] == '&'){
                tok = new Token(RELOP, AND);
                return tok;
            }
            else{
                msg = "Caracter esperado &. Recebido: " + std::string(1, input[pos]);
                lexicalError(msg);
            }
        
        case 10:
            if (input[pos] == '|'){
                tok = new Token(RELOP, OR);
                return tok;
            }
            else{
                msg = "Caracter esperado |. Recebido: " + std::string(1, input[pos]);
                lexicalError(msg);
            }

        case 11:
            if (!isspace(input[pos]))
                state = 0;
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
