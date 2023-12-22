#include "parser.h"

int main() {
    std::string our_program;
    std::getline(std::cin, our_program, '}');
    our_program += '}';
    Lexer lexer;
    lexer.CharToToken(our_program);
    Parser parser(lexer.GetTokens());
    parser.StartProgram();

    if (!parser.GetErrorStatus().first && !lexer.GetErrorStatus().first) {
        std::cout << "Successfully parsed!\n";
        return 1;
    } else {
        if(lexer.GetErrorStatus().first){
            std::cout << "Error in lex: " << lexer.GetErrorStatus().second << '\n';
        } else {
            std::cout << "Error in parse: " << parser.GetErrorStatus().second << '\n';
        }

    }
}