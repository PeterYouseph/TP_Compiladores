#include <map>
#include "stentry.h"

//Classe que representa uma tabela de símbolos. A classe utiliza
//a classe map para armazenar os símbolos. map implementa
//uma árvore que associa uma chave a um valor: em nosso casa a chave é uma
//string e o valor é um ponteiro para um objeto da classe STEntry.
class SymbolTable
{
    private:
        SymbolTable* parent;//Referência à tabela pai (escopo imediatamente anterior).
        map<string, STEntry*> symbols;//Armazena os símbolos do escopo corrente.

    public:
        SymbolTable();
        SymbolTable(SymbolTable*);

        bool add(STEntry*);
        bool remove(string);
        void clear();
        bool isEmpty();
        STEntry* get(string);
        SymbolTable* getParent();
};
