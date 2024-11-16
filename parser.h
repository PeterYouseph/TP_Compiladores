#include "scanner.h"
#include <map>

struct Symbol // Estrutura de um símbolo reservado da linguagem
{
	std::string type; // Tipo do símbolo 
	std::string scope; // Escopo do símbolo 
	int line; // Linha onde o símbolo foi declarado
};

class Parser
{
private:
	std::map<std::string, Symbol> symbolTable; // Tabela de símbolos 
	Scanner *scanner;	 // A instância do scanner
	Token *currentToken; // O token atua

	void nextToken();
	void expect(int tokenType);
	void parseProgram();		// Program → (Function)* EOF - Analisa o programa completo
	void parseFunction();		// Function → Type ID(ParamTypes){(Type VarDeclaration(...)* ;)(Statement)*} - Analisa a função completa
	void parseVarDeclaration(); // VarDeclaration → ID ([integerconstant])? - Valida a declaração de variáveis
	void parseType();			// Type → char | int - Verifica se o token atual é um tipo válido
	void parseParamTypes();		// ParamTypes → void | Type ID ([])? (, Type ID ([])?)* - Valida os tipos dos parâmetros
	void parseAssign();			// Assign → ID ([Expression])? = Expression - Valida a atribuição de variáveis
	void parseStatement();		// Statement → if ( Expression ) Statement (else Statement)? | ... - Analisa as declarações de instruções (Statement)
	void parseExpression();		// Expression → - Expression ExpreLinha | ! Expression  ExpreLinha | ... - Analisa as expressões
	void parseExprLinha();		// ExpreLinha → BinOp Expression ExpreLinha | Relop Expression ExpreLinha | LogOp Expression ExpreLinha | ε
	void parseBinOp();			// BinOp → + | - | * | /
	void parseRelOp();			// RelOp → == | != | <= | < | >= | >
	void parseLogOp();			// LogOp → && | ||
	void checkSymbol(const std::string &name, int line);
	void addSymbol(const std::string &name, const std::string &type, const std::string &scope, int line); // Adiciona um símbolo à tabela de símbolos
	void error(const std::string &msg); // Exibe uma mensagem de erro e lança uma exceção

public:
	Parser(Scanner *scanner); // Construtor

	void parseRun(); // Inicía o programa
};