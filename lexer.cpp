#include "lexer.h"

void Lexer::CharToToken(const std::string &input_string) {
    bool in_comment = false;
    for (size_t num_ch = 0; num_ch < input_string.size(); ++num_ch) {
        auto ch = input_string[num_ch];
        if (in_comment) {
            std::string temp;
            size_t i_temp = num_ch;
            int i = 0;
            while (!(input_string[i_temp] == '*' && input_string[++i_temp] == '/')) {
                temp += input_string[i_temp];
                i_temp++;
                i++;
            }
            tokens_.push_back({TokenType::COM, temp});
            tokens_.push_back({TokenType::END_COM, "/*"});
            in_comment = false;
            num_ch += i;
        } else {
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
                continue;
            }
            if (ch == '{') {
                tokens_.push_back({TokenType::PROGRAM_START, "{"});
            } else if (ch == '}') {
                tokens_.push_back({TokenType::PROGRAM_END, "}"});
            } else if (ch == '!') {
                size_t i_temp = num_ch;
                if (input_string[++i_temp] == '=' &&
                    (input_string[++i_temp] == ' ' || input_string[i_temp + 2] == '\n')) {
                    tokens_.push_back({TokenType::NOT_EQUAL, "!="});
                    num_ch++;
                } else if (tokens_.back().type == TokenType::COLON) {
                    tokens_.push_back({TokenType::TYPE, "!"});
                } else {
                    tokens_.push_back({TokenType::DENIAL, "!"});
                }
            } else if (ch == '(') {
                tokens_.push_back({TokenType::OPEN_BRACKET, "("});
            } else if (ch == ')') {
                tokens_.push_back({TokenType::CLOSE_BRACKET, ")"});
            } else if (ch == ':') {
                size_t i_temp = num_ch;
                if (input_string[++i_temp] == '=' && (input_string[++i_temp] == ' ' || input_string[i_temp] == '\n')) {
                    tokens_.push_back({TokenType::ASSIGNMENT, ":="});
                    num_ch++;
                } else if (input_string[i_temp] == ' ' || input_string[i_temp] == '\n') {
                    tokens_.push_back({TokenType::COLON, ":"});
                }
            } else if (ch == ';') {
                tokens_.push_back({TokenType::SEMICOLON, ";"});
            } else if (ch == '=') {
                size_t i_temp = num_ch;
                if (input_string[++i_temp] == '=') {
                    tokens_.push_back({TokenType::EQUAL, "=="});
                    num_ch++;
                } else {
                    error_ = {true, "Unknown symbol \'=\'\n"};
                }
                tokens_.push_back({TokenType::EQUAL, "="});
            } else if (ch == '+') {
                tokens_.push_back({TokenType::PLUS, "+"});
            } else if (ch == '>') {
                size_t i_temp = num_ch;
                if (input_string[++i_temp] == '=' && (input_string[++i_temp] == ' ' || input_string[i_temp] == '\n')) {
                    tokens_.push_back({TokenType::MORE_OR_EQUAL, ">="});
                    num_ch++;
                } else if (input_string[i_temp] == ' ' || input_string[i_temp] == '\n') {
                    tokens_.push_back({TokenType::MORE, ">"});
                }
            } else if (ch == '<') {
                size_t i_temp = num_ch;
                if (input_string[++i_temp] == '=' && (input_string[++i_temp] == ' ' || input_string[i_temp] == '\n')) {
                    tokens_.push_back({TokenType::LESS_OR_EQUAL, "<="});
                    num_ch++;
                } else if (input_string[i_temp] == ' ' || input_string[i_temp] == '\n') {
                    tokens_.push_back({TokenType::LESS, "<"});
                }
            } else if (ch == ',') {
                tokens_.push_back({TokenType::COMMA, ","});
            } else if (ch == '|') {
                size_t i_temp = num_ch;
                if (input_string[++i_temp] == '|' && (input_string[++i_temp] == ' ' || input_string[i_temp] == '\n')) {
                    tokens_.push_back({TokenType::PLUS, "||"});
                    num_ch++;
                } else {
                    tokens_.clear();
                    error_ = {true, "After \'|\' must go \'|\'\n"};
                    return;
                }
            } else if (ch == '%') {
                tokens_.push_back({TokenType::TYPE, "%"});
            } else if (ch == '$') {
                tokens_.push_back({TokenType::TYPE, "$"});
            } else if (ch == '/') {
                size_t i_temp = num_ch;
                if (input_string[++i_temp] == '*' && (input_string[++i_temp] == ' ' || input_string[i_temp] == '\n')) {
                    tokens_.push_back({TokenType::START_COM, "/*"});
                    in_comment = true;
                    num_ch++;
                } else if (input_string[i_temp] == ' ' || input_string[i_temp] == '\n') {
                    tokens_.push_back({TokenType::MULTIPLY, "*"});
                }
            } else if (ch == '*') {
                size_t i_temp;
                if (input_string[++i_temp] == '/' && (input_string[++i_temp] == ' ' || input_string[i_temp] == '\n')) {
                    tokens_.push_back({TokenType::END_COM, "*/"});
                    in_comment = false;
                    num_ch++;
                } else if (input_string[i_temp] == ' ' || input_string[i_temp] == '\n') {
                    tokens_.push_back({TokenType::MULTIPLY, "*"});
                }
            } else if (ch == '&') {
                size_t i_temp;
                if (input_string[++i_temp] == '&' && (input_string[++i_temp] == ' ' || input_string[i_temp] == '\n')) {
                    tokens_.push_back({TokenType::MULTIPLY, "&&"});
                    num_ch++;
                } else {
                    tokens_.clear();
                    error_ = {true, "After & must go &\n"};
                    return;
                }
            } else if (ch == 'b') {
                std::string temp;
                size_t i_temp = num_ch;
                temp += input_string[num_ch];
                int i = 0;
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                if (temp == "begin") {
                    tokens_.push_back({TokenType::COMPOUND_OPERATOR_BEGIN, temp});
                } else {
                    tokens_.push_back({TokenType::ID, temp});
                }
            } else if (ch == 'e') {
                std::string temp;
                size_t i_temp = num_ch;
                temp += input_string[num_ch];
                int i = 0;
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                if (temp == "end") {
                    tokens_.push_back({TokenType::COMPOUND_OPERATOR_END, temp});
                } else if (temp == "else") {
                    tokens_.push_back({TokenType::CONDITIONAL_TRANSITION_OPERATOR_ELSE, temp});
                } else {
                    tokens_.push_back({TokenType::ID, temp});
                }
            } else if (ch == 'i') {
                std::string temp;
                size_t i_temp = num_ch;
                temp += input_string[num_ch];
                int i = 0;
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                if (temp == "if") {
                    tokens_.push_back({TokenType::CONDITIONAL_TRANSITION_OPERATOR_IF, temp});
                } else {
                    tokens_.push_back({TokenType::ID, temp});
                }
            } else if (ch == 'f') {
                std::string temp;
                size_t i_temp = num_ch;
                temp += input_string[num_ch];
                int i = 0;
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                if (temp == "for") {
                    tokens_.push_back({TokenType::LOOP_OPERATOR_FOR, temp});
                } else {
                    tokens_.push_back({TokenType::ID, temp});
                }
            } else if (ch == 't') {
                std::string temp;
                size_t i_temp = num_ch;
                temp += input_string[num_ch];
                int i = 0;
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                if (temp == "to") {
                    tokens_.push_back({TokenType::LOOP_OPERATOR_TO, temp});
                } else {
                    tokens_.push_back({TokenType::ID, temp});
                }
            } else if (ch == 's') {
                std::string temp;
                size_t i_temp = num_ch;
                temp += input_string[num_ch];
                int i = 0;
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                if (temp == "step") {
                    tokens_.push_back({TokenType::LOOP_OPERATOR_STEP, temp});
                } else {
                    tokens_.push_back({TokenType::ID, temp});
                }
            } else if (ch == 'n') {
                std::string temp;
                size_t i_temp = num_ch;
                temp += input_string[num_ch];
                int i = 0;
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                if (temp == "next") {
                    tokens_.push_back({TokenType::LOOP_OPERATOR_NEXT, temp});
                } else {
                    tokens_.push_back({TokenType::ID, temp});
                }
            } else if (ch == 'w') {
                std::string temp;
                size_t i_temp = num_ch;
                int i = 0;
                temp += input_string[num_ch];
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                if (temp == "while") {
                    tokens_.push_back({TokenType::CONDITIONAL_LOOP_OPERATOR_WHILE, temp});
                } else if (temp == "writeln") {
                    tokens_.push_back({TokenType::OUTPUT_OPERATOR, temp});
                } else {
                    tokens_.push_back({TokenType::ID, temp});
                }
            } else if (ch == 'r') {
                std::string temp;
                size_t i_temp = num_ch;
                int i = 0;
                temp += input_string[num_ch];
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                if (temp == "readln") {
                    tokens_.push_back({TokenType::INPUT_OPERATOR, temp});
                } else {
                    tokens_.push_back({TokenType::ID, temp});
                }
            } else if (isalpha(ch)) {
                std::string temp;
                size_t i_temp = num_ch;
                int i = 0;
                temp += input_string[num_ch];
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                tokens_.push_back({TokenType::ID, temp});
            } else if (isdigit(ch)) {
                std::string temp;
                size_t i_temp = num_ch;
                temp += input_string[num_ch];
                int i = 0;
                if (input_string[++i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                    input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                    while (input_string[i_temp] != ' ' && input_string[i_temp] != '\n' && input_string[i_temp] != '(' && input_string[i_temp] != ',' &&
                           input_string[i_temp] != ')' && input_string[i_temp] != ';') {
                        if (!isdigit(input_string[i_temp])) {
                            error_ = {true, "Incorrect character sequence\n"};
                            tokens_.clear();
                            return;
                        }
                        temp += input_string[i_temp];
                        i_temp++;
                        i++;
                    }
                }
                num_ch += i;
                tokens_.push_back({TokenType::NUM, temp});
            }
        }
    }
}

std::pair<bool, std::string> Lexer::GetErrorStatus() {
    return error_;
}

std::vector<Token> Lexer::GetTokens() const {
    return tokens_;
}
