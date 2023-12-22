#include "parser.h"

void Parser::StartProgram() {
    if (current_token_.type != TokenType::PROGRAM_START) {
        error_ = {true, "No \'{\'"};
        return;
    }

    NextToken();
    ParseProgram();

    if(current_token_.type != TokenType::PROGRAM_END){
        error_ = {true, "No end\n"};
        return;
    }
}

void Parser::ParseProgram() {
    while(index_ != tokens_.size() - 1){
        auto temp = index_;
        if (current_token_.type == TokenType::ID && tokens_[++temp].type == TokenType::ASSIGNMENT) {
            ParseOperator();
        } else if (current_token_.type == TokenType::ID) {
            ParseDescription();
        } else if (current_token_.type == TokenType::COMPOUND_OPERATOR_BEGIN) {
            ParseOperator();
        } else if (current_token_.type == TokenType::CONDITIONAL_TRANSITION_OPERATOR_IF) {
            ParseOperator();
        } else if (current_token_.type == TokenType::LOOP_OPERATOR_FOR) {
            ParseOperator();
        } else if (current_token_.type == TokenType::CONDITIONAL_LOOP_OPERATOR_WHILE) {
            ParseOperator();
        } else if (current_token_.type == TokenType::CONDITIONAL_LOOP_OPERATOR_WHILE) {
            ParseOperator();
        } else if (current_token_.type == TokenType::INPUT_OPERATOR) {
            ParseOperator();
        } else if (current_token_.type == TokenType::OUTPUT_OPERATOR) {
            ParseOperator();
        } else {
            error_ = {true, "invalid token\n"};
        }
        NextToken();
    }
}

void Parser::ParseDescription() {
    while (current_token_.type != TokenType::COLON) {
        if (current_token_.type != TokenType::ID) {
            error_ = {true, "First must go ID\n"};
            return;
        }
        NextToken();
        if (current_token_.type == TokenType::COLON) {
            NextToken();
            break;
        }
        if (current_token_.type != TokenType::COMMA) {
            error_ = {true, "Comma must come after ID\n"};
            return;
        }
        NextToken();
    }
    if (current_token_.type != TokenType::TYPE) {
        error_ = {true, "After all IDS must come their type\n"};
        return;
    }
    NextToken();
    if (current_token_.type != TokenType::SEMICOLON) {
        error_ = {true, "No ;\n"};
        return;
    }
}

void Parser::ParseOperator() {
    if (current_token_.type == TokenType::COMPOUND_OPERATOR_BEGIN) {
        NextToken();
        ParseOperator();
        while (current_token_.type == TokenType::SEMICOLON) {
            NextToken();
            ParseOperator();
        }
        if (current_token_.type == TokenType::COMPOUND_OPERATOR_END) {
            NextToken();
        } else {
            error_ = {true, "No end\n"};
            return;
        }
        if (current_token_.type != TokenType::SEMICOLON) {
            error_ = {true, "No ;\n"};
            return;
        }
    } else if (current_token_.type == TokenType::ID) {
        NextToken();
        if (current_token_.type != TokenType::ASSIGNMENT) {
            error_ = {true, ""};
            return;
        }
        NextToken();
        Expression();
        if (current_token_.type != TokenType::SEMICOLON) {
            error_ = {true, "No ;\n"};
            return;
        }
    } else if (current_token_.type == TokenType::CONDITIONAL_TRANSITION_OPERATOR_IF) {
        NextToken();
        Expression();
        ParseOperator();
        NextToken();
        if (current_token_.type == TokenType::CONDITIONAL_TRANSITION_OPERATOR_ELSE) {
            NextToken();
            ParseOperator();
        } else {--index_;}
    } else if (current_token_.type == TokenType::LOOP_OPERATOR_FOR) {
        NextToken();
        if (current_token_.type != TokenType::ID) {
            error_ = {true, "Must be assignment"};
            return;
        }
        ParseOperator();
        NextToken();
        if (current_token_.type != TokenType::LOOP_OPERATOR_TO) {
            error_ = {true, "Must go to\n"};
            return;
        }
        NextToken();
        Expression();
        while (current_token_.type == TokenType::LOOP_OPERATOR_STEP) {
            NextToken();
            Expression();
        }
        ParseOperator();
        NextToken();
        if (current_token_.type == TokenType::LOOP_OPERATOR_NEXT) {
            NextToken();
        } else {
            error_ = {true, "Must go next\n"};
            return;
        }
        if (current_token_.type != TokenType::SEMICOLON) {
            error_ = {true, "No ;\n"};
            return;
        }
    } else if (current_token_.type == TokenType::CONDITIONAL_LOOP_OPERATOR_WHILE) {
        NextToken();
        Expression();
        ParseOperator();
    } else if (current_token_.type == TokenType::INPUT_OPERATOR) {
        NextToken();
        Expression();
        while (current_token_.type == TokenType::COMMA) {
            NextToken();
            if (current_token_.type != TokenType::ID) {
                error_ = {true, "Must be id\n"};
                return;
            }
            NextToken();
        }
        if (current_token_.type != TokenType::SEMICOLON) {
            error_ = {true, "No ;\n"};
            return;
        }
    } else if (current_token_.type == TokenType::OUTPUT_OPERATOR) {
        NextToken();
        Expression();
        while (current_token_.type == TokenType::COMMA) {
            NextToken();
            Expression();
        }
        if (current_token_.type != TokenType::SEMICOLON) {
            error_ = {true, "No ;\n"};
            return;
        }
    }
}

void Parser::Expression() {
    T();
    if (current_token_.type == TokenType::NOT_EQUAL || current_token_.type == TokenType::EQUAL ||
        current_token_.type == TokenType::LESS || current_token_.type == TokenType::LESS_OR_EQUAL ||
        current_token_.type == TokenType::MORE || current_token_.type == TokenType::MORE_OR_EQUAL) {
        NextToken();
        T();
    }
}

void Parser::T() {
    L();
    while (current_token_.type == TokenType::PLUS) {
        NextToken();
        L();
    }
}

void Parser::L() {
    M();
    while (current_token_.type == TokenType::MULTIPLY) {
        NextToken();
        M();
    }
}

void Parser::M() {
    if (current_token_.type == TokenType::ID || current_token_.type == TokenType::NUM) {
        NextToken();
    } else if (current_token_.type == TokenType::DENIAL) {
        NextToken();
        M();
    } else if (current_token_.type == TokenType::OPEN_BRACKET) {
        NextToken();
        Expression();
        if (current_token_.type != TokenType::CLOSE_BRACKET) {
            error_ = {true, "Must go close bracket\n"};
            return;
        }
        NextToken();
    } else {
        error_ = {true, ""};
        return;
    }
}

void Parser::NextToken() {
    if (index_ + 1 < tokens_.size()) {
        index_++;
        current_token_ = tokens_[index_];
    } else {
        current_token_ = Token();
        error_ = {true, "index > tokens.size()"};
    }
}

std::pair<bool, std::string> Parser::GetErrorStatus() {
    return error_;
}