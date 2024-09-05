#include "scanner.h"
#include "parser.h"

int main(/*int argc, char* argv[]*/)
{
    string input;

    getline(cin, input);

    Parser *parser = new Parser(input);

    parser->run();

    return 0;
}
