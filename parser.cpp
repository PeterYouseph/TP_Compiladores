#include "parser.h"

// Construtor da classe Parser
Parser::Parser(std::string input)
{
	currentST = globalST = new SymbolTable();
	this->scanner = new Scanner(input, globalST); // Inicializa o scanner
	initSymbolTable();
}

void Parser::initSymbolTable() {
	globalST->add(new STEntry(new Token(INT), true));
	globalST->add(new STEntry(new Token(CHAR_R), true));
	globalST->add(new STEntry(new Token(VOID), true));
	globalST->add(new STEntry(new Token(IF), true));
	globalST->add(new STEntry(new Token(ELSE), true));
	globalST->add(new STEntry(new Token(WHILE), true));
	globalST->add(new STEntry(new Token(FOR), true));
	globalST->add(new STEntry(new Token(RETURN), true));
}


// Obtém o próximo token da entrada
void Parser::nextToken()
{
	currentToken = scanner->nextToken(); // Obtém o próximo token
	if (currentToken) {
        std::cout << "[DEBUG] Next Token: " << currentToken->name 
                  << " Lexeme: '" << currentToken->lexeme 
                  << "' Line: " << scanner->getLine() << std::endl;
		cout << "Token: " << currentToken->name << " Lexeme: " << currentToken->lexeme << " Line: " << scanner->getLine() << endl;
    } else {
        std::cout << "[DEBUG] End of file reached." << std::endl;
		cout << "Token: " << END_OF_FILE << " Lexeme: " << "EOF" << " Line: " << scanner->getLine() << endl;
    }
}


// Verifica se o token atual é igual ao token esperado
void Parser::expectId()
{
	if (currentToken->name == ID) {
		currentST->add(new STEntry(currentToken, true));
		nextToken();
	} else error("Erro inesperado");
}
// Verifica se o token atual é igual ao token esperado
void Parser::expect(int t)
{
	if (currentToken->name == t || currentToken->attribute == t){
		nextToken();
	}
	else{
		// error("Erro inesperado"); 
		nextToken();
	}
}
// void Parser::error(string str)
void Parser::parseRun() // Inicía o programa
{
	std::cout << "[DEBUG] Starting parseRun." << std::endl;

	nextToken(); // Obtém o primeiro token

	parseProgram(); // Inicia a análise sintática completa do programa
	    std::cout << "[DEBUG] Parsing completed successfully!" << std::endl;

	if (currentToken->name != END_OF_FILE) { // Verifica se o token atual é o final do arquivo
		error("Syntax Error - Expected end of file.");
	} else {
		// Final de arquivo compilado com sucesso estilizado com cor verde (para sucesso)
		const std::string GREEN = "\033[1;32m";
		const std::string RESET = "\033[0m"; // Reseta a cor para o padrão
		std::cout << GREEN << "[SUCCESS] Compilation successful!" << RESET << std::endl;
	}
}

// Program → (Function)* EOF
void Parser::parseProgram()
{
	while (currentToken->name != END_OF_FILE){
		std::cout << "[DEBUG] Parsing function at line: " << scanner->getLine() << std::endl;
		parseFunction();
	}
	expect(END_OF_FILE);
	std::cout << "[DEBUG] Program parsed successfully!" << std::endl;

}

// // Function → void ID( ParamTypes ){(Type VarDeclaration(, VarDeclaration)∗;)∗ (Statement)∗} | Type ID( ParamTypes ){(Type VarDeclaration(, VarDeclaration)∗;)∗ (Statement)∗}
void Parser::parseFunction()
{
    std::cout << "[DEBUG] Parsing function at line: " << scanner->getLine() << std::endl;

    // Função de retorno VOID ou tipo específico
    if (currentToken->name == VOID) {
        std::cout << "[DEBUG] Function return type: void" << std::endl;
        nextToken();
    } else if (currentToken->name == CHAR_R || currentToken->name == INT) {
        std::cout << "[DEBUG] Parsing function return type..." << std::endl;
        parseType();
    } else {
        error("Syntax Error - Expected return type.");
    }

    // Nome da função
    if (currentToken->name == ID) {
        std::cout << "[DEBUG] Function name: " << currentToken->lexeme << std::endl;
        addSymbol(currentToken->lexeme, "function", "global", scanner->getLine());
        nextToken();
    } else {
        error("Syntax Error - Expected function name.");
    }

    expect(PE); // Espera o '(' de parâmetros
    parseParamTypes(); // Processa os parâmetros
    expect(PD); // Espera o ')' de fechamento
    expect(BE); // Espera '{' do corpo da função

    // Aqui deve haver uma verificação mais detalhada para declaração de variáveis e instruções
    while (currentToken->name != BD) {  // Enquanto não encontrar o fechamento da função
        if (currentToken->name == CHAR_R || currentToken->name == INT) {
            parseType();  // Lida com o tipo da variável
            parseVarDeclaration();  // Declara a variável
            while (currentToken->name == VIRGULA) { // Caso haja mais variáveis
                nextToken();
                parseVarDeclaration();
            }
            expect(PONTO_VIGULA);  // Espera o ponto e vírgula após a declaração
        } else if (currentToken->name == ID) {
			parseStatement();  // Lida com as instruções
		} else {
			error("Syntax Error - Expected variable declaration or statement.");
		}
    }
    expect(BD);  // Espera o fechamento da função
    std::cout << "[DEBUG] Function parsed successfully!" << std::endl;
}

// VarDeclaration → ID[integerconstant]
// VarDeclaration → ID
void Parser::parseVarDeclaration()
{
    std::string name = currentToken->lexeme;
    std::cout << "[DEBUG] Declaring variable: " << name 
              << " at line: " << scanner->getLine() << std::endl;
    expect(ID);  // Espera pelo identificador da variável
    addSymbol(name, currentToken->lexeme, "global", scanner->getLine());
    
    // Se for um array, lida com a declaração do tamanho do array
    if (currentToken->name == CE) {
        std::cout << "[DEBUG] Declaring array size..." << std::endl;
        nextToken();
        expect(INTEGER);  // Espera um valor inteiro para o tamanho do array
        expect(CD);  // Fecha o colchete
    }

    if (currentToken->name == VIRGULA) {
		while (currentToken->name != PONTO_VIGULA) {
			nextToken();
			std::string name = currentToken->lexeme;
			std::cout << "[DEBUG] Declaring variable: " << name 
					  << " at line: " << scanner->getLine() << std::endl;
			expect(ID);
			addSymbol(name, currentToken->lexeme, "global", scanner->getLine());
			if (currentToken->name == CE) {
				std::cout << "[DEBUG] Declaring array size..." << std::endl;
				nextToken();
				expect(INTEGER);
				expect(CD); 
			}  else if (currentToken->name == VIRGULA) {
				nextToken();
			} else if (currentToken->name == PONTO_VIGULA) {
				nextToken();
			} else {
				error("Syntax Error - Expected ',' or ';'.");
			}
		
		} 
	}
	else if (currentToken->name == PONTO_VIGULA) {
			nextToken();
	} else {
			error("Syntax Error - Expected ',' or ';'.");
	}
}


// Type → char | int
// Função para reconhecer tipos de função e declaração de variáveis
void Parser::parseType()
{
    std::cout << "[DEBUG] Parsing type at line: " << scanner->getLine() 
              << " Current token: " << currentToken->name << std::endl;
    
    if (currentToken->name == CHAR_R || currentToken->name == INT) {
        std::cout << "[DEBUG] Type found: " << currentToken->name << std::endl;
        nextToken();
    }
    else if (currentToken->name == VOID) {
        std::cout << "[DEBUG] Void type found: " << currentToken->name << std::endl;
        nextToken();
    }
    else {
			// error("Syntax Error - Expected int, char or void.");
			nextToken();
    }
}

// ParamTypes → void | Type ID([])?(,Type ID([])?)∗
void Parser::parseParamTypes()
{
	if (currentToken->name == VOID) {// ParamTypes → void
		nextToken();
		if (currentToken->name == PD) { // Apenas 'void)', sem parâmetros
            std::cout << "[DEBUG] Function with void parameter type." << std::endl;
            return;
        } else {
            error("Syntax Error - 'void' must be the only parameter type or part of the return type.");
        }
	}
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
				nextToken();
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
	} else if (currentToken->name == CHAR_R || currentToken->name == INT) {
		parseType();
		parseVarDeclaration();
		expect(PONTO_VIGULA);
	} else if (currentToken->name == ID) {
		parseAssign();
		expect(PONTO_VIGULA);
	} else if (currentToken->name == END_OF_FILE) {
		return;
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
	std::string name = currentToken->lexeme;
	checkSymbol(name, scanner->getLine());
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

void Parser::error(const std::string &msg) // Exibe uma mensagem de erro e lança uma exceção
{
    // Código ANSI para cor vermelha (para o erro)
    const std::string RED = "\033[1;31m";
    const std::string RESET = "\033[0m"; // Reseta a cor para o padrão

    std::cerr << RED << "[ERROR] " << msg << " at line " << scanner->getLine() << RESET << std::endl;
    throw std::runtime_error(msg);
}

