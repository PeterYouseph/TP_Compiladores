#include "scanner.h"
#include <map>

class Parser
{
private:
	Scanner *scanner;	 // A instância do scanner
	Token *currentToken; // O token atua

	SymbolTable *globalST;
	SymbolTable *currentST;

	void nextToken();
	void expect(int tokenType);
	bool match(int t);
	void expectId();
	void initSymbolTable();		//  Adds reserved IDs to global ST
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
	void error(const std::string &msg); // Exibe uma mensagem de erro e lança uma exceção
	std::string tokenIdentifier();

public:
	void parseRun(); // Inicía o programa
	Parser(std::string); // Construtor
};
