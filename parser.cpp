#include "parser.h"

// Construtor da classe Parser
Parser::Parser(Scanner *input)
{
	this->scanner = input; // Inicializa o scanner
}

// Obtém o próximo token da entrada
void Parser::nextToken()
{
	currentToken = scanner->nextToken(); // Obtém o próximo token
}

// Verifica se o token atual é igual ao token esperado
void Parser::expect(int t)
{
	if (currentToken->name == t || currentToken->attribute == t)
		nextToken();
	else
		error("Erro inesperado");
}
// void Parser::error(string str)
void Parser::parseRun() // Inicía o programa
{
	nextToken(); // Obtém o primeiro token

	parseProgram(); // Inicia a análise sintática completa do programa

	cout << "Compilação encerrada com sucesso!\n";
}

// Program → (Function)* EOF
void Parser::parseProgram()
{
	while (currentToken->name != END_OF_FILE)
		parseFunction();

	expect(END_OF_FILE);
}

// Function → void ID( ParamTypes ){(Type VarDeclaration(, VarDeclaration)∗;)∗ (Statement)∗} | Type ID( ParamTypes ){(Type VarDeclaration(, VarDeclaration)∗;)∗ (Statement)∗}
void Parser::parseFunction()
{
	if (currentToken->name == VOID) // Function → void ID( ParamTypes ){(Type VarDeclaration(, VarDeclaration)∗;)∗ (Statement)∗}
		nextToken();
	else // Function → Type ID( ParamTypes ){(Type VarDeclaration(, VarDeclaration)∗;)∗ (Statement)∗}
		parseType();
	expect(ID);
	expect(PE);
	parseParamTypes();
	expect(PD);
	expect(BE);
	while (currentToken->name != BD)
	{
		while (currentToken->name == CHAR_R || currentToken->name == INT)
		{
			parseType();
			parseVarDeclaration();
			while (currentToken->name == VIRGULA)
				parseVarDeclaration();
			expect(PONTO_VIGULA);
		}
		parseStatement();
	}
	expect(BD);
}

// VarDeclaration → ID ([integerconstant] )?
void Parser::parseVarDeclaration()
{
	expect(ID);
	if (currentToken->name == CE)
	{
		nextToken();
		expect(INTEGER);
		expect(CD);
	}
}

// Type → char | int
// Função para reconhecer tipos de função e declaração de variáveis
void Parser::parseType()
{
	if (currentToken->name == CHAR_R || currentToken->name == INT)
		nextToken();
	else
		error("Syntax Error - Expected int or char.");
}

// ParamTypes → void | Type ID([])?(,Type ID([])?)∗
void Parser::parseParamTypes()
{
	if (currentToken->name == VOID) // ParamTypes → void
		nextToken();
	else // ParamTypes → Type ID([])?(,Type ID([])?)∗
	{
		parseType();
		expect(ID);
		if (currentToken->name == CE)
		{
			nextToken();
			expect(CD);
		}
		if (currentToken->name == VIRGULA)
		{
			while (currentToken->name == VIRGULA)
			{
				parseType();
				expect(ID);
				if (currentToken->name == CE)
				{
					nextToken();
					expect(CD);
				}
			}
		}
	}
}

// Statement → if ( Expression ) Statement (else Statement)? | while ( Expression ) Statement | for ( (Assign)?;(Expression)?;(Assign)? ) Statement | return (Expression)? | Assign ;
void Parser::parseStatement()
{
	if (currentToken->name == IF) // if ( Expression ) Statement (else Statement)?
	{
		nextToken();
		expect(PE);
		parseExpression();
		expect(PD);
		parseStatement();
		if (currentToken->name == ELSE)
		{
			nextToken();
			parseStatement();
		}
	}
	else if (currentToken->name == WHILE) // while ( Expression ) Statement
	{
		nextToken();
		expect(PE);
		parseExpression();
		expect(PD);
		parseStatement();
	}
	else if (currentToken->name == FOR) // for ( (Assign)?; (Expression)?; (Assign)? ) Statement
	{
		nextToken();
		expect(PE);
		if (currentToken->name == ID)
			parseAssign();
		expect(PONTO_VIGULA);

		if (currentToken->name != PONTO_VIGULA) // Pode ter ou não uma expressão
			parseExpression();
		expect(PONTO_VIGULA);

		if (currentToken->name == ID) // Segunda atribuição opcional
			parseAssign();
		expect(PD);

		parseStatement();
	}
	else if (currentToken->name == RETURN) // return (Expression)?;
	{
		nextToken();
		if (currentToken->name != PONTO_VIGULA)
			parseExpression();
		expect(PONTO_VIGULA);
	}
	else if (currentToken->name == ID) // Pode ser um Assign ou um ID()
	{
		nextToken();
		if (currentToken->name == CE) // ID[ Expression ] = Expression;
		{
			nextToken();
			parseExpression();
			expect(CD);
			expect(EQUAL);
			parseExpression();
		}
		else if (currentToken->name == EQUAL) // ID = Expression;
		{
			nextToken();
			parseExpression();
		}
		else if (currentToken->name == PE) // ID( (Expression (, Expression)*)? );
		{
			nextToken();
			if (currentToken->name != PD)
			{
				parseExpression();
				while (currentToken->name == VIRGULA)
				{
					nextToken();
					parseExpression();
				}
			}
			expect(PD);
		}
		expect(PONTO_VIGULA);
	}
	else if (currentToken->name == BE) // { (Statement)* }
	{
		nextToken();
		while (currentToken->name != BD)
			parseStatement();
		expect(BD); // Sem necessidade de PONTO_VIGULA
	}
	else if (currentToken->name == PONTO_VIGULA) // ;
	{
		nextToken();
	}
	else
	{
		error("Expected statement");
	}
}

// Assign → ID([Expression])? = Expression
void Parser::parseAssign()
{
	expect(ID);
	if (currentToken->name == CE) // ID[ Expression ] = Expression
	{
		nextToken();
		parseExpression();
		expect(CD);
	}
	expect(EQUAL);
	parseExpression();
}

// Expression → - Expression | ! Expression | Expression BinOp Expression | ...
void Parser::parseExpression() // Analisa as expressões
{
	if (currentToken->name == MINUS) // Expression → - Expression
	{
		nextToken();
		parseExpression();
		parseExprLinha();
	}
	else if (currentToken->name == NOT) // Expression → ! Expression
	{
		nextToken();
		parseExpression();
		parseExprLinha();
	}
	// TODO E BinOp E | E RelOp E | E LogOp E
	else if (currentToken->name == ID) // Expression → ID(((Expression ( , Expression)∗)?)|[ Expression ])?
	{
		nextToken();
		if (currentToken->name == PE) // Expression → ID ()
		{
			nextToken();
			if (currentToken->name != PD) // Expression → ID ( Expression )
			{
				parseExpression();
				if (currentToken->name == VIRGULA) // Expression → ID ( Expression, Expression )
					parseExpression();
			}
			expect(PD);
		}
		else if (currentToken->name == CE) // Expression → ID [ Expression ]
		{
			nextToken();
			parseExpression();
			expect(CD);
		}
		parseExprLinha();
	}
	else if (currentToken->name == PE) // Expression →  (Expression)
	{
		nextToken();
		parseExpression();
		expect(PD);
		parseExprLinha();
	}
	else if (currentToken->name == INTEGER) // Expression → integerconstant
	{
		nextToken();
		parseExprLinha();
	}
	else if (currentToken->name == CHAR) // Expression → charconstant
	{
		nextToken();
		parseExprLinha();
	}
	else if (currentToken->name == STRING) // Expression → stringconstant
	{
		nextToken();
		parseExprLinha();
	}
}

void Parser::parseExprLinha() // ExpreLinha → BinOp Expression ExpreLinha | Relop Expression ExpreLinha | LogOp Expression ExpreLinha | ε
{
	if (currentToken->name == OP)
	{
		parseBinOp();
		parseExpression();
		parseExprLinha();
	}
	else if (currentToken->name == RELOP)
	{
		parseRelOp();
		parseExpression();
		parseExprLinha();
	}
	else if (currentToken->name == LOGOP)
	{
		parseLogOp();
		parseExpression();
		parseExprLinha();
	}
}

// BinOp → + | - | * | /
void Parser::parseBinOp()
{
	if (currentToken->name == PLUS) // BinOp → +
		nextToken();
	else if (currentToken->name == MINUS) // BinOp → -
		nextToken();
	else if (currentToken->name == MULT) // BinOp → *
		nextToken();
	else if (currentToken->name == DIV) // BinOp → /
		nextToken();
	else
		error("Expected +, -, * or /");
}

// RelOp → == | != | <= | < | >= | >
void Parser::parseRelOp()
{
	if (currentToken->name == EQ_COM) // RelOp → ==
		nextToken();
	else if (currentToken->name == DIF) // RelOp → !=
		nextToken();
	else if (currentToken->name == LE) // RelOp → <=
		nextToken();
	else if (currentToken->name == LT) // RelOp → <
		nextToken();
	else if (currentToken->name == GE) // RelOp → >=
		nextToken();
	else if (currentToken->name == GT) // RelOp → >
		nextToken();
	else
		error("Expected ==, !=, <=, <, >= or >");
}

// LopOp → && | ||
void Parser::parseLogOp()
{
	if (currentToken->name == AND) // LopOp → &&
		nextToken();
	else if (currentToken->name == OR) // LopOp → ||
		nextToken();
	else
		error("Expected && or ||.");
}

// TODO

// void
// Parser::error(string str)
// {
// 	cout << "Linha " << scanner->getLine() << ": " << str << endl;

// 	exit(EXIT_FAILURE);
// }
