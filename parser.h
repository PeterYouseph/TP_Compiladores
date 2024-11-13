#include "scanner.h"

class Parser
{
private:
	Scanner *scanner;	 // A instância do scanner
	Token *currentToken; // O token atua

	void nextToken();
	void expect(int tokenType);

	void error(const std::string &message)
	{
		throw std::runtime_error(message + " at line " + std::to_string(scanner->getLine()));
	}

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

public:
	Parser(Scanner *scanner); // Construtor

	void parseRun(); // Inicía o programa
};