#include "scanner.h"

class Parser
{
private:
    Scanner *scanner;
    Token *lToken;

    void advance();
    void match(int);

    void goal();
    void expr();
    void exprLinha();
    void term();
    void termLinha();
    void factor();
    void factorLinha();
    void call();
    void args();
    void argsLinha();
    void error();

    void error(string);

public:
    Parser(string);

    void run();
};
