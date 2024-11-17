#include "parser.h"

// Construtor da classe Parser
Parser::Parser(std::string input)
{
	currentST = globalST = new SymbolTable();
	this->scanner = new Scanner(input, globalST); // Inicializa o scanner
	initSymbolTable();
}

void Parser::initSymbolTable() {
	globalST->add(new STEntry(new Token(INT, RESERVED, "int"), true));
	globalST->add(new STEntry(new Token(CHAR_R, RESERVED, "char"), true));
	globalST->add(new STEntry(new Token(VOID, RESERVED, "void"), true));
	globalST->add(new STEntry(new Token(IF, RESERVED, "if"), true));
	globalST->add(new STEntry(new Token(ELSE, RESERVED, "else"), true));
	globalST->add(new STEntry(new Token(WHILE, RESERVED, "while"), true));
	globalST->add(new STEntry(new Token(FOR, RESERVED, "for"), true));
	globalST->add(new STEntry(new Token(RETURN, RESERVED, "return"), true));
}


// Obtém o próximo token da entrada
void Parser::nextToken()
{
	currentToken = scanner->nextToken(); // Obtém o próximo token
	if (currentToken) {
        std::cout << "[DEBUG] Next Token: " << currentToken->name 
                  << " Lexeme: '" << tokenIdentifier()
                  << "' Line: " << scanner->getLine() << std::endl;
//		cout << "Token: " << currentToken->name << " Lexeme: " << currentToken->lexeme << " Line: " << scanner->getLine() << endl;
    } else {
        std::cout << "[DEBUG] End of file reached." << std::endl;
//		cout << "Token: " << END_OF_FILE << " Lexeme: " << "EOF" << " Line: " << scanner->getLine() << endl;
    }
}

// Verifica se o token atual é igual ao token esperado
void Parser::expectId()
{
	if (match(ID)) {
		currentST->add(new STEntry(currentToken, true));
		nextToken();
	} else error("Erro inesperado");
}
// Verifica se o token atual é igual ao token esperado
// Se for, da nextToken
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

// Verifica se o token atual é igual ao token esperado
bool Parser::match(int t)
{
	return currentToken->name == t || currentToken->attribute == t;
}
// void Parser::error(string str)
void Parser::parseRun() // Inicía o programa
{
	std::cout << "[DEBUG] Starting parseRun." << std::endl;

	nextToken(); // Obtém o primeiro token

	parseProgram(); // Inicia a análise sintática completa do programa
	    std::cout << "[DEBUG] Parsing completed successfully!" << std::endl;

	if (!match(END_OF_FILE)) { // Verifica se o token atual é o final do arquivo
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
	while (!match(END_OF_FILE)){
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
    if (match(VOID)) {
        std::cout << "[DEBUG] Function return type: void" << std::endl;
        nextToken();
    } else if (match(CHAR_R) || match(INT)) {
        std::cout << "[DEBUG] Parsing function return type..." << std::endl;
        parseType();
    } else {
        error("Syntax Error - Expected return type, got " + tokenIdentifier());
    }

    // Nome da função
    if (match(ID)) {
        std::cout << "[DEBUG] Function name: " << currentToken->lexeme << std::endl;
        nextToken();
    } else {
        error("Syntax Error - Expected function name, got " + tokenIdentifier());
    }

    expect(PE); // Espera o '(' de parâmetros
    parseParamTypes(); // Processa os parâmetros
    expect(PD); // Espera o ')' de fechamento
    expect(BE); // Espera '{' do corpo da função

    // Aqui deve haver uma verificação mais detalhada para declaração de variáveis e instruções
    while (!match(BD)) {  // Enquanto não encontrar o fechamento da função
        if (match(CHAR_R) || match(INT)) {
            parseType();  // Lida com o tipo da variável
            parseVarDeclaration();  // Declara a variável
            while (match(VIRGULA)) { // Caso haja mais variáveis
                nextToken();
                parseVarDeclaration();
            }
            expect(PONTO_VIGULA);  // Espera o ponto e vírgula após a declaração
        } else if (match(ID) || match(RESERVED)) {
			parseStatement();  // Lida com as instruções
		} else {
			error("Syntax Error - Expected variable declaration or statement, got " + tokenIdentifier());
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
    
    // Se for um array, lida com a declaração do tamanho do array
    if (match(CE)) {
        std::cout << "[DEBUG] Declaring array size..." << std::endl;
        nextToken();
        expect(INTEGER);  // Espera um valor inteiro para o tamanho do array
        expect(CD);  // Fecha o colchete
    }

    if (match(VIRGULA)) {
		while (!match(PONTO_VIGULA)) {
			nextToken();
			std::string name = currentToken->lexeme;
			std::cout << "[DEBUG] Declaring variable: " << name 
					  << " at line: " << scanner->getLine() << std::endl;
			expect(ID);
			if (match(CE)) {
				std::cout << "[DEBUG] Declaring array size..." << std::endl;
				nextToken();
				expect(INTEGER);
				expect(CD); 
			}  else if (match(VIRGULA)) {
				nextToken();
			} else if (match(PONTO_VIGULA)) {
				break;
			} else {
				error("Syntax Error - Expected ',' or ';'.");
			}
		
		} 
	}
	else if (match(PONTO_VIGULA)) {
			return;
	} else {
			error("Syntax Error - Expected ',' or ';'.");
	}
}


// Type → char | int
// Função para reconhecer tipos de função e declaração de variáveis
void Parser::parseType()
{
    std::cout << "[DEBUG] Parsing type at line: " << scanner->getLine() 
              << " Current token: " << currentToken->lexeme << std::endl;
    
    if (match(CHAR_R) || match(INT)) {
        std::cout << "[DEBUG] Type found: " << currentToken->lexeme << std::endl;
        nextToken();
    }
    else if (match(VOID)) {
        std::cout << "[DEBUG] Void type found: " << currentToken->lexeme << std::endl;
        nextToken();
    }
    else {
		 error("Syntax Error - Expected int, char or void.");
    }
}

// ParamTypes → void | Type ID([])?(,Type ID([])?)∗
void Parser::parseParamTypes()
{
	std::cout << "[DEBUG] Parsing paramTypes." << std::endl;
	if (match(VOID)) {// ParamTypes → void
		nextToken();
		if (match(PD)) { // Apenas 'void)', sem parâmetros
            std::cout << "[DEBUG] Function with void parameter type." << std::endl;
            return;
        } else {
            error("Syntax Error - 'void' must be the only parameter type or part of the return type.");
        }
	}
    else {
    	parseType();
    	expect(ID);
    	if (match(CE))
    	{
    		nextToken();
    		expect(CD);
    	}
    	if (match(VIRGULA))
    	{
    		while (match(VIRGULA))
    		{
    			nextToken();
    			parseType();
    			expect(ID);
    			if (match(CE))
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
  	cout << "[DEBUG] Parsing statement." << std::endl;
	if (match(IF)) // if ( Expression ) Statement (else Statement)?
	{
		nextToken();
		expect(PE);
		parseExpression();
		expect(PD);
		parseStatement();
		if (match(ELSE))
		{
			nextToken();
			parseStatement();
		}
	}
	else if (match(WHILE)) // while ( Expression ) Statement
	{
		nextToken();
		expect(PE);
		parseExpression();
		expect(PD);
		parseStatement();
	}
	else if (match(FOR)) // for ( (Assign)?; (Expression)?; (Assign)? ) Statement
	{
		nextToken();
		expect(PE);
		if (match(ID))
			parseAssign();
		expect(PONTO_VIGULA);

		if (!match(PONTO_VIGULA)) // Pode ter ou não uma expressão
			parseExpression();
		expect(PONTO_VIGULA);

		if (match(ID)) // Segunda atribuição opcional
			parseAssign();
		expect(PD);

		parseStatement();
	}
	else if (match(RETURN)) // return (Expression)?;
	{
		nextToken();
		if (!match(PONTO_VIGULA))
			parseExpression();
		expect(PONTO_VIGULA);
	}
	else if (match(ID)) // Pode ser um Assign ou um ID()
	{
		nextToken();
		if (match(CE)) // ID[ Expression ] = Expression;
		{
			nextToken();
			parseExpression();
			expect(CD);
			expect(EQUAL);
			parseExpression();
		}
		else if (match(EQUAL)) // ID = Expression;
		{
			nextToken();
			parseExpression();
		}
		else if (match(PE)) // ID( (Expression (, Expression)*)? );
		{
			nextToken();
			if (!match(PD))
			{
				parseExpression();
				while (match(VIRGULA))
				{
					nextToken();
					parseExpression();
				}
			}
			expect(PD);
		}
		expect(PONTO_VIGULA);
	}
	else if (match(BE)) // { (Statement)* }
	{
		nextToken();
		while (!match(BD))
			parseStatement();
		expect(BD); // Sem necessidade de PONTO_VIGULA
	}
	else if (match(PONTO_VIGULA)) // ;
	{
		nextToken();
	} else if (match(CHAR_R) || match(INT)) {
		parseType();
		parseVarDeclaration();
		expect(PONTO_VIGULA);
	} else if (match(ID)) {
		parseAssign();
		expect(PONTO_VIGULA);
	} else if (match(END_OF_FILE)) {
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
//	checkSymbol(name, scanner->getLine());
	if (match(CE)) // ID[ Expression ] = Expression
	{
		nextToken();
		parseExpression();
		expect(CD);
	}
	expect(EQUAL);
	parseExpression();
}

// Expression → - Expression
// Expression → ! Expression
// Expression →  Expression BinOp Expression
// Expression →
// Expression →
void Parser::parseExpression() // Analisa as expressões
{
	cout << "parseExpression with:" + currentToken->lexeme << endl;
	if (match(MINUS)) // Expression → - Expression
	{
		nextToken();
		parseExpression();
		parseExprLinha();
	}
	else if (match(NOT)) // Expression → ! Expression
	{
		nextToken();
		parseExpression();
		parseExprLinha();
	}
	// TODO E BinOp E | E RelOp E | E LogOp E
	else if (match(ID)) // Expression → ID(((Expression ( , Expression)∗)?)|[ Expression ])?
	{
		nextToken();
		if (match(PE)) // Expression → ID ()
		{
			nextToken();
			if (!match(PD)) // Expression → ID ( Expression )
			{
				parseExpression();
				if (match(VIRGULA)) // Expression → ID ( Expression, Expression )
					parseExpression();
			}
			expect(PD);
		}
		else if (match(CE)) // Expression → ID [ Expression ]
		{
			nextToken();
			parseExpression();
			expect(CD);
		}
		parseExprLinha();
	}
	else if (match(PE)) // Expression →  (Expression)
	{
		nextToken();
		parseExpression();
		expect(PD);
		parseExprLinha();
	}
	else if (match(INTEGER)) // Expression → integerconstant
	{
		nextToken();
		parseExprLinha();
	}
	else if (match(CHAR)) // Expression → charconstant
	{
		nextToken();
		parseExprLinha();
	}
	else if (match(STRING)) // Expression → stringconstant
	{
		nextToken();
		parseExprLinha();
	}
}

// ExpreLinha → BinOp Expression ExpreLinha
// ExpreLinha -> Relop Expression ExpreLinha
// ExpreLinha -> LogOp Expression ExpreLinha
// ExpreLinha -> ε
void Parser::parseExprLinha()
{
  cout << "parseExprLinha with:" + currentToken->lexeme << endl;
	if (match(OP))
	{
		parseBinOp();
		parseExpression();
		parseExprLinha();
	}
	else if (match(RELOP))
	{
		parseRelOp();
		parseExpression();
		parseExprLinha();
	}
	else if (match(LOGOP))
	{
		parseLogOp();
		parseExpression();
		parseExprLinha();
	}
}

// BinOp → + | - | * | /
void Parser::parseBinOp()
{
	if (match(PLUS)) // BinOp → +
		nextToken();
	else if (match(MINUS)) // BinOp → -
		nextToken();
	else if (match(MULT)) // BinOp → *
		nextToken();
	else if (match(DIV)) // BinOp → /
		nextToken();
	else
		error("Expected +, -, * or /");
}

// RelOp → == | != | <= | < | >= | >
void Parser::parseRelOp()
{
	if (match(EQ_COM)) // RelOp → ==
		nextToken();
	else if (match(DIF)) // RelOp → !=
		nextToken();
	else if (match(LE)) // RelOp → <=
		nextToken();
	else if (match(LT)) // RelOp → <
		nextToken();
	else if (match(GE)) // RelOp → >=
		nextToken();
	else if (match(GT)) // RelOp → >
		nextToken();
	else
		error("Expected ==, !=, <=, <, >= or >");
}

// LopOp → && | ||
void Parser::parseLogOp()
{
	if (match(AND)) // LopOp → &&
		nextToken();
	else if (match(OR)) // LopOp → ||
		nextToken();
	else
		error("Expected && or ||.");
}

std::string Parser::tokenIdentifier() {
  return currentToken->lexeme + " (" +  std::to_string(currentToken->name) + ") - " + std::to_string(currentToken->attribute);
}

void Parser::error(const std::string &msg) // Exibe uma mensagem de erro e lança uma exceção
{
    // Código ANSI para cor vermelha (para o erro)
    const std::string RED = "\033[1;31m";
    const std::string RESET = "\033[0m"; // Reseta a cor para o padrão

    std::cerr << RED << "[ERROR] " << msg << " at line " << scanner->getLine() << RESET << std::endl;
    throw std::runtime_error(msg);
}

