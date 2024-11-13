#include "scanner.h"
#include "token.h"
#include "parser.h"

string *vet;

void print(Token *);
void allocVetor();
void freeVetor();

int main(int argc, char *argv[]) // Função principal do compilador
{
    if (argc != 2)
    {
        cerr << "Uso: " << argv[0] << " <arquivo>" << endl; // Exibe a mensagem de uso correto
        return 1;
    }

    allocVetor(); // Aloca o vetor de tokens

    try
    {
        Scanner scanner(argv[1]); // Cria uma instância do scanner
        Parser parser(&scanner);  // Cria uma instância do parser com o scanner

        parser.parseRun(); // Inicia a análise sintática completa do programa
        cout << "Análise sintática completa sem erros.\n";
    }
    catch (const runtime_error &e) // Captura exceções de erro de sintaxe e exibe a mensagem
    {
        cerr << "Error: " << e.what() << endl; // Exibe a mensagem de erro com a linha onde ocorreu o erro
    }

    freeVetor(); // Libera a memória alocada para o vetor de tokens

    return 0; // Retorna 0 para indicar que o programa foi executado com sucesso
}

// Acrescentar os vetores dos demais token
void allocVetor()
{
    vet = new string[35]; // Ajustado para 35 tokens

    vet[0] = "UNDEF";         // 0
    vet[1] = "ID";            // 1
    vet[2] = "INTEGER";       // 2
    vet[3] = "CHAR";          // 3
    vet[4] = "STRING";        // 4
    vet[5] = "PLUS";          // 5
    vet[6] = "MINUS";         // 6
    vet[7] = "MULT";          // 7
    vet[8] = "DIV";           // 8
    vet[9] = "EQUAL";         // 9
    vet[10] = "EQ_COM";       // 10
    vet[11] = "DIF";          // 11
    vet[12] = "LE";           // 12
    vet[13] = "LT";           // 13
    vet[14] = "GE";           // 14
    vet[15] = "GT";           // 15
    vet[16] = "AND";          // 16
    vet[17] = "OR";           // 17
    vet[18] = "NOT";          // 18
    vet[19] = "PE";           // 19
    vet[20] = "PD";           // 20
    vet[21] = "BE";           // 21
    vet[22] = "BD";           // 22
    vet[23] = "CE";           // 23
    vet[24] = "CD";           // 24
    vet[25] = "VIRGULA";      // 25
    vet[26] = "PONTO_VIGULA"; // 26
    vet[30] = "ELSE";         // 30
    vet[31] = "IF";           // 31
    vet[32] = "WHILE";        // 32
    vet[33] = "FOR";          // 33
    vet[34] = "INT";          // 34
}

void freeVetor()
{
    delete[] vet;
}

void print(Token *t)
{
    if (t->name != END_OF_FILE)
    {
        cout << vet[t->name] << " "; // Imprime o nome do token
        if (t->attribute != UNDEF)
        {
            cout << "(" << t->attribute << ")"; // Imprime o atributo se não for indefinido
        }
        cout << " Lexeme: " << (t->lexeme.empty() ? "-" : t->lexeme) << endl; // Imprime o lexema do token
    }
}
