#include "parser.h"

Parser::Parser(string input)
{
	scanner = new Scanner(input);
	advance();
}

void Parser::run()
{
	goal();
}

void Parser::advance()
{
	lToken = scanner->nextToken();

	// cout << lToken->name << endl;
}

void Parser::match(int t)
{
	if (lToken->name == t || lToken->attribute == t)
		advance();
	else
		error("Erro inesperado");
}

void Parser::goal()
{
	expr();
}

// CONTINUAR

void Parser::expr()
{
	term();
	exprLinha();
}

void Parser::exprLinha()
{
	if (lToken->name == '+')
	{
		match('+');
		term();
		exprLinha();
	}
}

void Parser::term()
{
	factor();
	termLinha();
}

void Parser::termLinha()
{
	if (lToken->name == '*')
	{
		match('*');
		factor();
		termLinha();
	}
}

void Parser::factor()
{
	if (lToken->name == '(')
	{
		match('(');
		expr();
		match(')');
	}
	else if (lToken->name == NUMBER)
	{
		match(NUMBER);
	}
	else if (lToken->name == ID)
	{
		match(ID);
		call();
	}
	else
	{
		error("Erro inesperado");
	}
}

void Parser::call()
{
	if (lToken->name == '(')
	{
		match('(');
		args();
		match(')');
	}
}

void Parser::args()
{
	expr();
	argsLinha();
}

void Parser::argsLinha()
{
	if (lToken->name == ',')
	{
		match(',');
		expr();
		argsLinha();
	}
}

void Parser::error(string str)
{
	cout << str << endl;

	exit(EXIT_FAILURE);
}