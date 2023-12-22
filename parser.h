#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>

class Parser {
private:
    std::vector<Token> tokens_;
    std::pair<bool, std::string> error_ = {false, ""};
    size_t index_ = 0;
    Token current_token_ = tokens_[index_];
    std::vector<bool> compound_operator_amount_;
public:
    explicit Parser(const std::vector<Token> &tokens) : tokens_(tokens) {};

    void StartProgram();
    void ParseProgram();
    void ParseDescription();
    void ParseOperator();
    void Expression();
    void T();
    void L();
    void M();
    void NextToken();
    std::pair<bool, std::string> GetErrorStatus();
};


#endif // PARSER_H