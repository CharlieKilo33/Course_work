#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <iostream>

enum class TokenType {
  PROGRAM_START,
  PROGRAM_END,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  LESS,
  LESS_OR_EQUAL,
  MORE,
  MORE_OR_EQUAL,
  COMMA,
  COLON,
  SEMICOLON,
  PLUS,
  EQUAL,
  NOT_EQUAL,
  START_COM,
  END_COM,
  TYPE,
  MULTIPLY,
  ASSIGNMENT,
  COMPOUND_OPERATOR_BEGIN,
  COMPOUND_OPERATOR_END,
  CONDITIONAL_TRANSITION_OPERATOR_IF,
  CONDITIONAL_TRANSITION_OPERATOR_ELSE,
  LOOP_OPERATOR_FOR,
  LOOP_OPERATOR_STEP,
  LOOP_OPERATOR_NEXT,
  LOOP_OPERATOR_TO,
  CONDITIONAL_LOOP_OPERATOR_WHILE,
  INPUT_OPERATOR,
  OUTPUT_OPERATOR,
  ID,
  NUM,
  COM,
  DENIAL
};

struct Token {
  TokenType type;
  std::string value;
};

class Lexer{
 private:
  std::vector<Token> tokens_;
  std::pair<bool, std::string> error_ = {false, ""};

 public:
  void CharToToken(const std::string &input_string);
  [[nodiscard]] std::vector<Token> GetTokens() const;
  std::pair<bool, std::string> GetErrorStatus();
};

#endif // LEXER_H