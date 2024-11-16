#include "symboltable.h"

//Construtor que instancia uma nova tabela sem especificar uma tabela pai.
SymbolTable::SymbolTable()
{
    parent = 0;
}

//Construtor que instancia uma nova tabela especificando uma tabela pai.
SymbolTable::SymbolTable(SymbolTable* p)
{
    parent = p;
}

//Tenta adicionar um novo símbolo à tabela corrente. Primeiramente, verifica
//se já existe uma entrada para o lexema na tabela corrente e, caso haja,
//a inserção não é realizada e o método retorna false; caso contrário, a 
//inserção é efetivada e true é retornado.
bool 
SymbolTable::add(STEntry* t)
{
    if (!(symbols.find(t->token->lexeme) == symbols.end()))
        return false;
    //cout << "Inseri";
    symbols.insert(make_pair(t->token->lexeme,t));
    return true;
}

//Tenta remover um símbolo da tabela.
bool 
SymbolTable::remove(string name)
{
    return symbols.erase(name) != 0;
}

//Limpa a tabela.
void 
SymbolTable::clear()
{
    symbols.clear();
}

//Verifica se a tabela está vazia.
bool 
SymbolTable::isEmpty()
{
    return symbols.empty();
}

//Busca uma entrada equivalente ao lexema passado como parâmetro. A busca se
//inicia no escopo corrente e sobe na hierarquia enquanto não encontrar o
//símbolo, possivelmente até o escopo global. Retorna um ponteiro para a 
//entrada encontrada ou 0, caso o símbolo não esteja presente.
STEntry* 
SymbolTable::get(string name)
{
    SymbolTable* table = this;

    auto s = table->symbols.find(name);

    while (s == table->symbols.end())
    {
        table = table->parent;
        if (table == 0)
            return 0;

        s = table->symbols.find(name);
    } 
    
    return s->second;
}

SymbolTable*
SymbolTable::getParent()
{
    return parent;
}
